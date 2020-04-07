// <copyright file="endpoint.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "MagicMousePad/endpoint.h"

#include <codecvt>
#include <system_error>

#include <sdkddkver.h>
#include <WinSock2.h>
#include <ws2ipdef.h>
#include <WinDNS.h>
#include <iphlpapi.h>


/*
 * MagicMousePad::ParseEndPoint
 */
sockaddr_storage MagicMousePad::ParseEndPoint(const wchar_t *string) {
    NET_ADDRESS_INFO address;
    sockaddr_storage retval;

    {
        auto status = ::ParseNetworkString(string, NET_STRING_ANY_SERVICE,
            &address, nullptr, nullptr);
        if (status != ERROR_SUCCESS) {
            throw std::system_error(status, std::system_category(),
                "Failed to parse network service address");
        }
    }

    ::ZeroMemory(&retval, sizeof(retval));

    switch (address.Format) {
        case NET_ADDRESS_IPV4:
            ::memcpy(&retval, &address.Ipv4Address,
                sizeof(address.Ipv4Address));
            break;

        case NET_ADDRESS_IPV6:
            ::memcpy(&retval, &address.Ipv6Address,
                sizeof(address.Ipv6Address));
            break;

        default:
            throw std::invalid_argument("The specified end point address is "
                "not supported");
    }

    return retval;
}


/*
 * MagicMousePad::ParseEndPoint
 */
sockaddr_storage MagicMousePad::ParseEndPoint(const char *string) {
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
    auto wstring = cvt.from_bytes(string);
    return ParseEndPoint(wstring.c_str());
}
