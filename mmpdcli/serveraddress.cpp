// <copyright file="serveraddress.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "MagicMousePad/serveraddress.h"

#include <cassert>
#include <stdexcept>

#if defined(_WIN32)
#include <WS2tcpip.h>
#endif /* defined(_WIN32) */

#include "MagicMousePad/byteorder.h"


/*
 * MagicMousePad::InitialiseServerAddress
 */
std::pair<sockaddr *, int> MagicMousePad::InitialiseServerAddress(
        sockaddr_storage &dst, const int addressFamily,
        const std::uint16_t port) {
    switch (addressFamily) {
            case AF_INET: {
                auto addr = static_cast<std::uint32_t>(INADDR_ANY);
                auto a = reinterpret_cast<sockaddr_in *>(&dst);
                a->sin_family = AF_INET;
                a->sin_addr.S_un.S_addr = ToNetworkOrder(addr);
                a->sin_port = ToNetworkOrder(port);
                auto b = reinterpret_cast<sockaddr *>(a);
                assert(sizeof(*a) < (std::numeric_limits<int>::max)());
                return std::make_pair(b, static_cast<int>(sizeof(*a)));
            }

            case AF_INET6: {
                auto a = reinterpret_cast<sockaddr_in6 *>(&dst);
                a->sin6_family = AF_INET6;
                a->sin6_addr = ::in6addr_any;
                a->sin6_port = ToNetworkOrder(port);
                auto b = reinterpret_cast<sockaddr *>(a);
                assert(sizeof(*a) < (std::numeric_limits<int>::max)());
                return std::make_pair(b, static_cast<int>(sizeof(*a)));
            }

        default:
            throw std::invalid_argument("Only IPv4 and IPv6 are supported");
    }
}
