// <copyright file="mmp_client.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "mmp_client.h"

#include <atomic>
#include <cassert>
#include <chrono>
#include <cstring>
#include <limits>
#include <memory>

#include <mmpcli.h>
#include <mmpmsg.h>
#include <WinSock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#include "trace.h"


/*
 * mmp_client::discover
 */
_Success_(return == 0) int mmp_client::discover(
        _In_ mmp_client& client) {
    auto& config = client._config;
    std::int16_t port = 0;

    switch (config.address.ss_family) {
        case AF_INET: {
            auto a = reinterpret_cast<sockaddr_in&>(config.address);
            if (a.sin_addr.s_addr == INADDR_ANY) {
                port = a.sin_port;
            } else {
                MMP_TRACE(L"An IPv4 address was provided for the magic mouse "
                    L"pad, so no discovery is necessary.");
                return 0;
            }
            } break;

        case AF_INET6: {
            auto a = reinterpret_cast<sockaddr_in6&>(config.address);
            if (::memcmp(&a.sin6_addr, &in6addr_any, sizeof(in6addr_any))
                    == 0) {
                port = a.sin6_port;
            } else {
                MMP_TRACE(L"An IPv6 address was provided for the magic mouse "
                    L"pad, so no discovery is necessary.");
                return 0;
            }
            } break;

        default:
            MMP_TRACE(L"Either IPv4 or IPv6 must be specified as the address "
                L"family, but %d was provided.", config.address.ss_family);
            return ERROR_INVALID_PARAMETER;
    }

    assert(port != 0);

    // Get all possible bradcast addresses for the given port.
    WSADATA wsa_data;
    RETURN_IF_WIN32_ERROR(::WSAStartup(MAKEWORD(2, 2), &wsa_data));
    auto wsa_cleanup = wil::scope_exit([](void) { ::WSACleanup(); });

    std::vector<sockaddr_in> addresses;
    try {
        addresses = bcast_addresses(port);
    } catch (const wil::ResultException& e) {
        MMP_TRACE(L"Failed to retrieve broadcast addresses: %hs", e.what());
        return e.GetErrorCode();
    }


    // Prepare a socket that we will solely use for discovery.
    wil::unique_socket socket(::WSASocket(AF_INET,
        SOCK_DGRAM,
        IPPROTO_UDP,
        nullptr,
        0,
        WSA_FLAG_OVERLAPPED));
    if (!socket) {
        auto retval = ::WSAGetLastError();
        MMP_TRACE(L"Failed to create a UDP socket for discovery: 0x%x.",
            retval);
        return retval;
    }

    // Start a thread that receives the responses.
    std::atomic<bool> found(false);
    std::thread([&found, &socket, &client](void) {
        constexpr int cnt_buffer = (std::numeric_limits<std::uint16_t>::max)();
        std::vector<char> buffer(cnt_buffer);
        sockaddr_storage peer;

        while (true) {
            int cnt_peer = sizeof(peer);
            auto len = ::recvfrom(socket.get(),
                buffer.data(),
                cnt_buffer,
                0,
                reinterpret_cast<sockaddr *>(&peer),
                &cnt_peer);
            if (len == SOCKET_ERROR) {
                MMP_TRACE(L"The receive failed with error 0x%x, so the "
                    L"discovery receiver is leaving now.", ::WSAGetLastError());
                return;
            }
            if (len < sizeof(mmp_msg_discover)) {
                MMP_TRACE(L"Received an invalid datagram.");
                continue;
            }

            const auto id = ::ntohl(*reinterpret_cast<const mmp_msg_id *>(
                buffer.data()));
            MMP_TRACE(L"Received message 0x%x.", id);

            if (id == mmp_msgid_announce) {
                MMP_TRACE(L"Received a mouse pad announcement.");
                client._config.address = peer;

                auto announce = reinterpret_cast<const mmp_msg_announce *>(
                    buffer.data());
                if (announce->port != 0) {
                    assert(client._config.address.ss_family == AF_INET);
                    auto& a = reinterpret_cast<sockaddr_in &>(
                        client._config.address);
                    MMP_TRACE(L"Using announced port %d instead of %d.",
                        announce->port, a.sin_port);
                    a.sin_port = announce->port;
                }

                found.store(true, std::memory_order_release);
                return;
            }
        }
    }).detach();

    // Compute the deadline until which we need to be ready.
    const std::chrono::milliseconds rate_limit(config.rate_limit);
    auto deadline = std::chrono::system_clock::now();
    if (config.timeout > 0) {
        deadline += std::chrono::milliseconds(config.timeout);
    } else {
        deadline = (decltype(deadline)::max)();
    }

    while ((std::chrono::system_clock::now() < deadline)
            && !found.load(std::memory_order_acquire)) {
        mmp_msg_discover msg;

        for (auto it = addresses.begin(); it != addresses.end();) {
            MMP_TRACE(L"Sending discovery message.");
            if (::sendto(socket.get(),
                    reinterpret_cast<const char *>(&msg),
                    sizeof(msg),
                    0,
                    reinterpret_cast<const sockaddr *>(&*it),
                    sizeof(sockaddr_in))
                    == SOCKET_ERROR) {
                it = addresses.erase(it);
            } else {
                ++it;
            }
        }

        if (addresses.empty()) {
            MMP_TRACE(L"No working broadcast addresses available.");
            return ::WSAGetLastError();
        }

        std::this_thread::sleep_for(rate_limit);
    }

    return found ? 0 : ERROR_TIMEOUT;
}


/*
 * mmp_client::bcast_addresses
 */
std::vector<sockaddr_in> mmp_client::bcast_addresses(
        _In_ const std::uint16_t port) {
    std::vector<sockaddr_in> retval;

    const auto family = AF_INET;
    const auto flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST
        | GAA_FLAG_SKIP_DNS_SERVER;
    ULONG len = 0;

    {
        auto status = ::GetAdaptersAddresses(family, flags, nullptr, nullptr,
            &len);
        THROW_WIN32_IF(status, status != ERROR_BUFFER_OVERFLOW);
    }

    std::vector<BYTE> buffer(len);
    auto adapters = reinterpret_cast<IP_ADAPTER_ADDRESSES *>(buffer.data());

    THROW_IF_WIN32_ERROR(::GetAdaptersAddresses(family, flags, nullptr,
        adapters, &len));

    for (auto adapter = adapters;
        adapter != nullptr;
        adapter = adapter->Next) {
        if (adapter->OperStatus != IfOperStatusUp) {
            // Skip adapters that are not up, because the client cannot connect
            // to them.
            MMP_TRACE(L"Skipping adapter %hs because it is not up.",
                adapter->AdapterName);
            continue;
        }

        for (auto address = adapter->FirstUnicastAddress;
                address != nullptr;
                address = address->Next) {
            if (address->Address.lpSockaddr == nullptr) {
                // Skip adapters that do not have a valid address.
                MMP_TRACE(L"Skipping invalid address on adapter %hs.",
                    adapter->AdapterName);
                continue;
            }
            if (address->Address.lpSockaddr->sa_family != family) {
                // Broadcast works only with IPv4.
                MMP_TRACE(L"Skipping address on adapter %hs because it is not "
                    L"an IPv4 address.", adapter->AdapterName);
                continue;
            }

            // Determine the network mask for the address.
            const auto prefix = address->OnLinkPrefixLength;
            auto mask = (prefix == 0) ? 0 : (~0UL << (32 - prefix));
            mask = ::ntohl(mask);
            MMP_TRACE(L"Prefix length %d corresponds to netmask 0x%08x in "
                L"network byte order.", prefix, mask);

            // Compute the broadcast address.
            retval.emplace_back();
            retval.back().sin_family = family;
            auto dst = std::addressof(retval.back().sin_addr);
            auto src = std::addressof(reinterpret_cast<sockaddr_in *>(
                address->Address.lpSockaddr)->sin_addr);
            ::memcpy(dst, src, sizeof(in_addr));
            dst->s_addr |= ~mask;
            retval.back().sin_port = port;
        }
    }

    return retval;
}
