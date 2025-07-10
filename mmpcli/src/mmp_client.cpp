// <copyright file="mmp_client.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "mmp_client.h"

#include <cassert>
#include <cstring>

#include <WinSock2.h>
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
                L"family, but % d was provided.", config.address.ss_family);
            return ERROR_INVALID_PARAMETER;
    }

    assert(port != 0);



    return -1;
}
