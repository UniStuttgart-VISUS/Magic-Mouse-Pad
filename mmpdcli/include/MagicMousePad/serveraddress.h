// <copyright file="serveraddress.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once

#if defined(_WIN32)
#include <WinSock2.h>
#endif /* defined(_WIN32) */

#include <cinttypes>
#include <utility>


namespace MagicMousePad {

    /// <summary>
    /// Initialises an end point address for the specified port.
    /// </summary>
    extern std::pair<sockaddr *, int> InitialiseServerAddress(
        sockaddr_storage &dst, const int addressFamily,
        const std::uint16_t port);

}
