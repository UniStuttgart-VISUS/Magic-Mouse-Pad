// <copyright file="mmpendpoint.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "mmpendpoint.h"

#include <codecvt>

#include <WinDNS.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <wil/result.h>
#include <wil/resource.h>

#include "trace.h"


/*
 * ::parse_end_pointa
 */
int parse_end_pointa(_Out_ sockaddr_storage *end_point,
        _In_z_ const char *string) {
    if (string == nullptr) {
        MMP_TRACE("The input string to be parsed is invalid.");
    }

    static std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    auto s = cvt.from_bytes(string);
    return parse_end_pointw(end_point, s.c_str());
}


/*
 * ::parse_end_pointw
 */
int parse_end_pointw(_Out_ sockaddr_storage *end_point,
        _In_z_ const wchar_t *string) {
    if (end_point == nullptr) {
        MMP_TRACE("The output pointer for the end point is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    if (string == nullptr) {
        MMP_TRACE("The input string to be parsed is invalid.");
    }

    NET_ADDRESS_INFO address;
    RETURN_IF_WIN32_ERROR(::ParseNetworkString(string, NET_STRING_ANY_SERVICE,
        &address, nullptr, nullptr));

    ::ZeroMemory(end_point, sizeof(*end_point));

    switch (address.Format) {
        case NET_ADDRESS_IPV4:
            MMP_TRACE("Parsed an IPv4 address.");
            ::memcpy(end_point,
                &address.Ipv4Address,
                sizeof(address.Ipv4Address));
            return 0;

        case NET_ADDRESS_IPV6:
            MMP_TRACE("Parsed an IPv6 address.");
            ::memcpy(end_point,
                &address.Ipv6Address,
                sizeof(address.Ipv6Address));
            return 0;

        case NET_ADDRESS_DNS_NAME: {
            MMP_TRACE("Performing a name lookup.");
            ADDRINFOW *addresses = nullptr;

            ADDRINFOW hints = { 0 };
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_DGRAM;
            hints.ai_protocol = IPPROTO_UDP;

            RETURN_IF_WIN32_ERROR(::GetAddrInfoW(
                address.NamedAddress.Address,
                address.NamedAddress.Port,
                &hints,
                &addresses));
            auto free_addresses = wil::scope_exit(
                [&addresses](void) { ::FreeAddrInfoW(addresses); });

            for (auto *a = addresses; a != nullptr; a = a->ai_next) {
                switch (a->ai_family) {
                    case AF_INET:
                    case AF_INET6:
                        MMP_TRACE("Found end point from name.");
                        ::memcpy(end_point, a->ai_addr, a->ai_addrlen);
                        return 0;
                }
            }
            } break;
    }

    MMP_TRACE("The input string does not designate a valid address.");
    return ERROR_INVALID_PARAMETER;
}

