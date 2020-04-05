// copyright file="CommandLine.h" company="Visualisierungsinstitut der Universität Stuttgart" >
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once


/// <summary>
/// Contains all options obtained from the command line or the default values
/// for these options.
/// </summary>
class CommandLine {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    CommandLine(const TCHAR *cmdLine);

    /// <summary>
    /// Gets the address family the server should use.
    /// </summary>
    /// <remarks>
    /// The default address family is IPv4.
    /// </remarks>
    inline std::int32_t GetAddressFamily(void) const {
        return this->_addressFamily;
    }

    /// <summary>
    /// Gets the port the server should bind to.
    /// </summary>
    /// <remarks>
    /// The default port is 47596.
    /// </remarks>
    inline std::uint16_t GetPort(void) const {
        return this->_port;
    }

private:

    std::int32_t _addressFamily;
    std::uint16_t _port;

};
