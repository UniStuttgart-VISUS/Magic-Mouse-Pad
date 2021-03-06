// <copyright file="endpoint.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once

#if defined(_WIN32)
#include <WinSock2.h>
#endif /* defined(_WIN32) */


namespace MagicMousePad {

    /// <summary>
    /// Parses the given string as end point (service) address, either IPv4 or
    /// IPv6 or DNS name.
    /// <summary>
    extern sockaddr_storage ParseEndPoint(const wchar_t *string);

    /// <summary>
    /// Parses the given string as end point (service) address, either IPv4 or
    /// IPv6 or DNS name.
    /// <summary>
    extern sockaddr_storage ParseEndPoint(const char *string);

}
