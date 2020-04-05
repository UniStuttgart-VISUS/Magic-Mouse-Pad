// copyright file="CommandLine.h" company="Visualisierungsinstitut der Universität Stuttgart" >
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once


/// <summary>
/// Holds the options passed to the programme from the command line or the
/// default values for that options.
/// </summary>
class CommandLine {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    CommandLine(const TCHAR *cmdLine);

    /// <summary>
    /// Gets the port the client should bind to.
    /// </summary>
    /// <remarks>
    /// A value of 0, which is the default, indicates that the client should
    /// use an ephemeral port.
    /// </remarks>
    inline std::uint16_t GetPort(void) const {
        return this->_port;
    }

    /// <summary>
    /// Gets the address of the server to connect to.
    /// </summary>
    /// <remarks>
    /// No server was specified if the address is all-zero, most notably if the
    /// protocol is neither IPv4 nor IPv6.
    /// </remarks>
    inline const sockaddr_storage& GetServer(void) const {
        return this->_server;
    }

private:

    std::uint16_t _port;
    sockaddr_storage _server;
};
