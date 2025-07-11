// <copyright file="server.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "server.h"

#include <array>
#include <limits>

#include <mmpmsg.h>
#include <mmpthreadname.h>
#include <iphlpapi.h>
#include <Windows.h>


/*
 * server::server
 */
server::server(_In_ const settings& settings, _In_opt_ HWND window)
        : _running(true), _window(window) {
    this->_server = std::thread(&server::serve, this, settings);
}


/*
 * server::~server
 */
server::~server(void) noexcept {
    this->_running.store(false, std::memory_order_release);
    this->_socket.reset();
    if (this->_server.joinable()) {
        this->_server.join();
    }
}


/*
 * server::send
 */
void server::send(_In_reads_(cnt) const char *data,
        _In_ const int cnt) noexcept {
    std::lock_guard<std::mutex> lock(this->_lock);
    for (auto it = this->_clients.begin(); it != this->_clients.end();) {
        if (::sendto(this->_socket.get(),
                data, cnt,
                0,
                *it, it->address_length()) == SOCKET_ERROR) {
            // We just remove all clients that have failed.
            it = this->_clients.erase(it);
        } else {
            ++it;
        }
    }
}


/*
 * server::addresses
 */
std::vector<sockaddr_storage> server::addresses(void) {
    std::vector<sockaddr_storage> retval;

    const auto family = AF_UNSPEC;
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
            continue;
        }

        for (auto address = adapter->FirstUnicastAddress;
                address != nullptr;
                address = address->Next) {
            if (address->Address.lpSockaddr == nullptr) {
                // Skip adapters that do not have a valid address.
                continue;
            }

            switch (address->Address.lpSockaddr->sa_family) {
                case AF_INET:
                    retval.emplace_back();
                    ::memcpy(&retval.back(),
                        address->Address.lpSockaddr,
                        sizeof(sockaddr_in));
                    break;

                case AF_INET6:
                    retval.emplace_back();
                    ::memcpy(&retval.back(),
                        address->Address.lpSockaddr,
                        sizeof(sockaddr_in6));
                    break;
            }
        }
    }

    return retval;
}


/*
 * server::copy_port
 */
void server::copy_port(_In_ sockaddr_storage& dst,
        _In_ const sockaddr *src) {
    switch (dst.ss_family) {
        case AF_INET: {
            auto&a = reinterpret_cast<sockaddr_in&>(dst);
            a.sin_port = get_port(src);
            } break;

        case AF_INET6: {
            auto& a = reinterpret_cast<sockaddr_in6&>(dst);
            a.sin6_port = get_port(src);
            } break;
    }
}


/*
 * server::get_port
 */
std::uint16_t server::get_port(_In_ const sockaddr *src) {
    assert(src != nullptr);

        switch (src->sa_family) {
        case AF_INET: {
            auto a = reinterpret_cast<const sockaddr_in *>(src);
            return a->sin_port;
            } break;

        case AF_INET6: {
            auto a = reinterpret_cast<const sockaddr_in6 *>(src);
            return a->sin6_port;
            } break;

        default:
            return 0;
    }
}


///*
// * server::address
// */
//sockaddr_storage server::address(_In_ const sockaddr_storage& peer) {
//    // Determines the common prefix length of 'lhs' and 'rhs'.
//    auto common_prefix = [](const sockaddr_storage& lhs,
//            const sockaddr_storage& rhs) {
//        std::size_t retval = 0;
//        if (lhs.ss_family != rhs.ss_family) {
//            return retval;
//        }
//
//        const std::uint8_t *l = nullptr;
//        std::size_t len = 0;
//        const std::uint8_t *r = nullptr;
//
//        switch (lhs.ss_family) {
//            case AF_INET: {
//                auto& a = reinterpret_cast<const sockaddr_in&>(lhs);
//                l = reinterpret_cast<const std::uint8_t *>(&a.sin_addr);
//                len = sizeof(a.sin_addr);
//                } break;
//
//            case AF_INET6: {
//                auto& a = reinterpret_cast<const sockaddr_in6&>(lhs);
//                l = reinterpret_cast<const std::uint8_t *>(&a.sin6_addr);
//                len = sizeof(a.sin6_addr);
//                } break;
//
//            default:
//                return retval;
//        }
//        assert(l != nullptr);
//        assert(r != nullptr);
//
//        for (std::size_t i = 0; i < len; ++i) {
//            if (l[i] == r[i]) {
//                retval += 8;
//
//            } else {
//                for (int bit = 7; (bit >= 0); --bit) {
//                    if ((l[i] & (1 << bit)) == (r[i] & (1 << bit))) {
//                        ++retval;
//                    } else {
//                        break;
//                    }
//                }
//            }
//        }
//
//        return retval;
//    };
//
//    // Sorts the addresses such that the one with the longest common prefix
//    // comes first.
//    std::sort(this->_addresses.begin(), this->_addresses.end(),
//        [&peer, common_prefix](const sockaddr_storage& l,
//                const sockaddr_storage& r) {
//            return (common_prefix(l, peer) > common_prefix(r, peer));
//        });
//
//    return this->_addresses.front();
//}


/*
 * server::serve
 */
void server::serve(_In_ const settings settings) {
    mmp_set_thread_name(-1, "Magic mouse pad receiver");

    try {
        std::array<char, (std::numeric_limits<std::uint16_t>::max)()> buffer;
        sockaddr_storage peer;
        int cnt_peer = sizeof(peer);
        WSADATA wsa_data;

        {
            DWORD status = ::WSAStartup(MAKEWORD(2, 2), &wsa_data);
            THROW_WIN32_IF(status, status != NO_ERROR);
        }

        auto wsa_cleanup = wil::scope_exit([](void) { ::WSACleanup(); });

        // Create a datagram socket using the configured address.
        this->_socket.reset(::WSASocket(
            settings.address()->sa_family,
            SOCK_DGRAM,
            IPPROTO_UDP,
            nullptr,
            0,
            WSA_FLAG_OVERLAPPED));
        THROW_LAST_ERROR_IF(!this->_socket);

        // Bind the socket to the configured port such that it can wait for
        // incoming connections and discovery requests.
        THROW_LAST_ERROR_IF(::bind(this->_socket.get(),
            settings.address(),
            settings.address_length())
            == SOCKET_ERROR);

        while (this->_running.load(std::memory_order_acquire)) {
            auto cnt = ::recvfrom(this->_socket.get(),
                buffer.data(),
                static_cast<int>(buffer.size()),
                0,
                reinterpret_cast<sockaddr *>(&peer),
                &cnt_peer);
            if (cnt < sizeof(mmp_msg_id)) {
                continue;
            }

            const auto id = ::ntohl(*reinterpret_cast<const mmp_msg_id *>(
                buffer.data()));

            switch (id) {
                case mmp_msgid_discover: {
                    // If the server received a discovery request, it sends its
                    // address back to the requestor. If the server has multiple
                    // addresses, it sends the one that is in the same subnet as
                    // the requestor.
                    mmp_msg_announce response;
                    response.port = server::get_port(settings.address());
                    ::sendto(this->_socket.get(),
                        reinterpret_cast<const char *>(&response),
                        sizeof(response),
                        0,
                        reinterpret_cast<const sockaddr *>(&peer),
                        cnt_peer);
                    } break;

                case mmp_msgid_connect: {
                    // In case of a connect request, we register the peer
                    // address as a client.
                    std::lock_guard<std::mutex> l(this->_lock);
                    this->_clients.emplace(peer);
                    ::SendMessage(this->_window, WM_PAINT, 0, 0);
                    } break;
            }
        }/* while (this->_running.load(std::memory_order_acquire)) */

    } catch (wil::ResultException e) {
        ::MessageBoxA(NULL, e.what(), nullptr, MB_ICONERROR | MB_OK);
    }
}
