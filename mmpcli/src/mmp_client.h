// <copyright file="mmp_client.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "mmpcli.h"

#include <thread>
#include <vector>

#include <wil/resource.h>


/// <summary>
/// Represents the magic mouse pad client.
/// </summary>
struct mmp_client final {

public:

    /// <summary>
    /// If no specific IP address is specified in the
    /// <see cref="mmp_configuration"/> of the <paramref name="client"/>,
    /// tries to discover the address via UDP.
    /// </summary>
    /// <param name="client"></param>
    /// <returns></returns>
    static _Success_(return == 0) int discover(_In_ mmp_client& client);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="config"></param>
    inline explicit mmp_client(_In_ const mmp_configuration& config) noexcept
        : _config(config) { }

private:

    /// <summary>
    /// Find the broadcast addresses using the given <paramref name="port"/> for
    /// all active adapters on the system.
    /// </summary>
    /// <param name="port"></param>
    /// <returns></returns>
    static std::vector<sockaddr_in> bcast_addresses(
        _In_ const std::uint16_t port);

    mmp_configuration _config;
    wil::unique_socket _socket;
};
