// <copyright file="mmp_client.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "mmp_client.h"

#include <atomic>
#include <chrono>
#include <cstring>
#include <limits>
#include <memory>

#include <WinSock2.h>
#include <Windows.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#include "mmpcli.h"
#include "mmpmsg.h"
#include "mmpthreadname.h"
#include "trace.h"


/*
 * mmp_client::discover
 */
_Success_(return == 0) int mmp_client::discover(
        _In_ mmp_configuration& config) {
    std::int16_t port = 0;

    switch (config.server.ss_family) {
        case AF_INET: {
            auto a = reinterpret_cast<sockaddr_in&>(config.server);
            if (a.sin_addr.s_addr == INADDR_ANY) {
                port = a.sin_port;

            } else if (config.client.ss_family == 0) {
                MMP_TRACE(L"An IPv4 address was provided for the magic mouse "
                    L"pad, but none for the client. We therefore coerce the "
                    L"address families to be the same.");
                ::memset(&config.client, 0, sizeof(config.client));
                config.client.ss_family = config.server.ss_family;
                return 0;

            } else if (config.client.ss_family != config.server.ss_family) {
                MMP_TRACE(L"An IPv4 (%d) address was provided for the magic "
                    L"mouse pad, but the client address family is %d.",
                    config.server.ss_family, config.client.ss_family);
                return WSAEAFNOSUPPORT;

            } else {
                MMP_TRACE(L"An IPv4 address was provided for the magic mouse "
                    L"pad, so no discovery is necessary.");
                return 0;
            }
            } break;

        case AF_INET6: {
            auto a = reinterpret_cast<sockaddr_in6&>(config.server);
            if (::memcmp(&a.sin6_addr, &in6addr_any, sizeof(in6addr_any))
                    == 0) {
                port = a.sin6_port;

            } else if (config.client.ss_family == 0) {
                MMP_TRACE(L"An IPv6 address was provided for the magic mouse "
                    L"pad, but none for the client. We therefore coerce the "
                    L"address families to be the same.");
                ::memset(&config.client, 0, sizeof(config.client));
                config.client.ss_family = config.server.ss_family;
                return 0;

            } else if (config.client.ss_family != config.server.ss_family) {
                MMP_TRACE(L"An IPv6 (%d) address was provided for the magic "
                    L"mouse pad, but the client address family is %d.",
                    config.server.ss_family, config.client.ss_family);
                return WSAEAFNOSUPPORT;

            } else {
                MMP_TRACE(L"An IPv6 address was provided for the magic mouse "
                    L"pad, so no discovery is necessary.");
                return 0;
            }
            } break;

        default:
            MMP_TRACE(L"Either IPv4 or IPv6 must be specified as the address "
                L"family, but %d was provided.", config.server.ss_family);
            return ERROR_INVALID_PARAMETER;
    }

    assert(port != 0);

    // Get all possible bradcast addresses for the given port.
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

    MMP_TRACE(L"Discovering magic mouse pad at port %d.", ::ntohs(port));
    RETURN_IF_WIN32_ERROR(!bind(socket, config.client));

    // Start a thread that receives the responses.
    std::atomic<bool> found(false);
    std::thread([&found, &socket, &config](void) {
        ::mmp_set_thread_name(-1, "Magic mouse pad discovery");
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
                MMP_TRACE(L"The receive failed with error %d, so the "
                    L"discovery receiver is leaving now.", ::WSAGetLastError());
                return;
            }
            if (len < sizeof(mmp_msg_discover)) {
                MMP_TRACE(L"Received an invalid datagram.");
                continue;
            }

            const auto id = ::ntohl(*as<mmp_msg_id>(buffer));
            MMP_TRACE(L"Received message 0x%x.", id);

            if (id == mmp_msgid_announce) {
                MMP_TRACE(L"Received a mouse pad announcement.");
                config.server = peer;

                auto announce = as<mmp_msg_announce>(buffer);
                if (announce->port != 0) {
                    assert(config.server.ss_family == AF_INET);
                    auto& a = reinterpret_cast<sockaddr_in&>(config.server);
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
 * mmp_client::mmp_client
 */
mmp_client::mmp_client(_In_ const mmp_configuration& config)
        : _config(config),
        _running(true),
        _socket(make_socket(config)) {
    MMP_TRACE(L"Client initialised, starting client receiver thread.");
    this->_receiver = std::thread(&mmp_client::receive, this);
}


/*
 * mmp_client::~mmp_client
 */
mmp_client::~mmp_client(void) noexcept {
    MMP_TRACE(L"Stopping client receiver thread.");
    this->_running.store(false, std::memory_order_release);
    this->_socket.reset();
    if (this->_receiver.joinable()) {
        this->_receiver.join();
    }
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


/*
 * mmp_client::bind
 */
int mmp_client::bind(_In_ wil::unique_socket& socket,
        _In_ sockaddr_storage& address) {
    assert(socket);

    switch (address.ss_family) {
        case AF_INET:
            MMP_TRACE(L"Binding to IPv4 address on port %u.", ::ntohs(
                reinterpret_cast<const sockaddr_in&>(address).sin_port));
            RETURN_LAST_ERROR_IF(::bind(socket.get(),
                reinterpret_cast<const sockaddr *>(&address),
                sizeof(sockaddr_in)) == SOCKET_ERROR);
            return 0;

        case AF_INET6:
            MMP_TRACE(L"Binding to IPv6 address on port %u.", ::ntohs(
                reinterpret_cast<const sockaddr_in6&>(address).sin6_port));
            RETURN_LAST_ERROR_IF(::bind(socket.get(),
                reinterpret_cast<const sockaddr *>(&address),
                sizeof(sockaddr_in6)) == SOCKET_ERROR);
            return 0;

        default:
            MMP_TRACE(L"Incompatible address family %d.", address.ss_family);
            return WSAEAFNOSUPPORT;
    }
}


/*
 * mmp_client::make_socket
 */
wil::unique_socket mmp_client::make_socket(
        _In_ const mmp_configuration& config) {
    auto address_family = config.client.ss_family;
    switch (address_family) {
        case AF_INET:
        case AF_INET6:
            break;

        default:
            MMP_TRACE(L"No valid client address family was specified, so use "
                L"the one of the server address.");
            address_family = config.server.ss_family;
            break;
    }

    MMP_TRACE(L"Creating socket of family %d.", address_family);
    wil::unique_socket retval(::WSASocket(
        address_family,
        SOCK_DGRAM,
        IPPROTO_UDP,
        nullptr,
        0,
        WSA_FLAG_OVERLAPPED));
    THROW_LAST_ERROR_IF(!retval);
    return retval;
}


/*
 * mmp_client::announce
 */
int mmp_client::announce(void) {
    MMP_TRACE("Announcing client to server.");
    const auto server_addr = reinterpret_cast<const sockaddr *>(
        std::addressof(this->_config.server));
    const auto server_addr_len = static_cast<int>(
        sizeof(sockaddr_storage));

    mmp_msg_announce msg;
    msg.port = 0;

    if (::sendto(this->_socket.get(),
            reinterpret_cast<const char *>(&msg),
            sizeof(msg),
            0,
            server_addr,
            server_addr_len)
            == SOCKET_ERROR) {
        auto retval = ::WSAGetLastError();
        MMP_TRACE("Announcement failed with error code %d.", retval);
        return retval;
    }

    return 0;
}


/*
 * mmp_client::on_mouse_button
 */
void mmp_client::on_mouse_button(_In_ const mmp_msg_mouse_button *msg) {
    assert(msg != nullptr);
    assert(msg->id == mmp_msgid_mouse_button);
    if (this->_config.on_mouse_button != nullptr) {
        auto p = this->xform_position(msg);
        this->_config.on_mouse_button(msg->button, msg->down, p.first,
            p.second, this->_config.context);
    }
}


/*
 * mmp_client::on_mouse_move
 */
void mmp_client::on_mouse_move(_In_ const mmp_msg_mouse_move *msg) {
    assert(msg != nullptr);
    assert(msg->id == mmp_msgid_mouse_move);
    if (this->_config.on_mouse_move != nullptr) {
        auto p = this->xform_position(msg);
        this->_config.on_mouse_move(p.first, p.second, this->_config.context);
    }
}


/*
 * mmp_client::receive
 */
void mmp_client::receive(void) {
    ::mmp_set_thread_name(-1, "Magic mouse pad receiver");

    constexpr int cnt_buffer = (std::numeric_limits<std::uint16_t>::max)();
    std::vector<char> buffer(cnt_buffer);
    WSADATA wsa_data;

    MMP_TRACE(L"The client receiver thread 0x%08x is running. Initialising "
        L"Winsock for this thread.", ::GetCurrentThreadId());
    {
        auto status = ::WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (status != 0) {
            MMP_TRACE(L"WSAStartup failed with error %d.", status);
            return;
        }
    }
    auto wsa_cleanup = wil::scope_exit([](void) { ::WSACleanup(); });

    if (bind(this->_socket, this->_config.client) != 0) {
        MMP_TRACE(L"Failed to bind receiver socket.");
        return;
    }

#if (defined(_DEBUG) || defined(DEBUG))
    {
        sockaddr_storage addr { 0 };
        int cnt_addr = sizeof(addr);
        ::getsockname(this->_socket.get(),
            reinterpret_cast<sockaddr *>(&addr),
            &cnt_addr);
        switch (addr.ss_family) {
            case AF_INET:
                MMP_TRACE(L"Receiver bound to port %u.", ::ntohs(
                    reinterpret_cast<const sockaddr_in&>(addr).sin_port));
                break;

            case AF_INET6:
                MMP_TRACE(L"Receiver bound to port %u.", ::ntohs(
                    reinterpret_cast<const sockaddr_in6&>(addr).sin6_port));
                break;
        }
    }
#endif /* (defined(_DEBUG) || defined(DEBUG)) */

    if (this->announce() != 0) {
        MMP_TRACE(L"Failed to announce client to server, so the receiver "
            L"thread is leaving now.");
        return;
    }

    MMP_TRACE(L"Entering the receive loop.");
    while (this->_running.load(std::memory_order_acquire)) {
        sockaddr_storage peer;
        int cnt_peer = sizeof(peer);

        auto len = ::recvfrom(this->_socket.get(),
            buffer.data(),
            cnt_buffer,
            0,
            reinterpret_cast<sockaddr *>(&peer),
            &cnt_peer);
        if (len == SOCKET_ERROR) {
            MMP_TRACE(L"The receive failed with error %d, so the receiver "
                L"thread is leaving now.", ::WSAGetLastError());
            return;
        }
        if (len < sizeof(mmp_msg_id)) {
            MMP_TRACE(L"Received an invalid datagram.");
            continue;
        }

        const auto id = ::ntohl(*as<mmp_msg_id>(buffer));
        switch (id) {
            case mmp_msgid_mouse_move:
                this->on_mouse_move(as<mmp_msg_mouse_move>(buffer));
                break;
        }
    }
}
