// <copyright file="client.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <atomic>
#include <chrono>
#include <utility>

#include <WinSock2.h>
#include <ws2ipdef.h>

#include <wil/resource.h>

#include "settings.h"


/// <summary>
/// The server keeps track of all the clients connected to the mouse pad that
/// receive updates from it.
/// </summary>
class client final {

public:

    /// <summary>
    /// The clock used for the update timestamps.
    /// </summary>
    typedef std::chrono::steady_clock clock;

    /// <summary>
    /// The type used for the update timestamps.
    /// </summary>
    typedef clock::time_point time_point;

    /// <summary>
    /// The type used to measure the age of the client.
    /// </summary>
    typedef decltype(std::declval<time_point>().time_since_epoch()) duration;

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="address">The address of the client.</param>
    client(_In_ const sockaddr_storage& address);

    /// <summary>
    /// Gets the address of the client.
    /// </summary>
    /// <returns>The address of the client.</returns>
    inline const sockaddr_storage& address(void) const noexcept {
        return this->_address;
    }

    /// <summary>
    /// Answer the time elapsed since the last update of the client.
    /// </summary>
    /// <returns></returns>
    inline duration age(void) const noexcept {
        return (clock::now() - this->last_update());
    }

    /// <summary>
    /// Gets the length of the address in bytes.
    /// </summary>
    /// <returns>The length of the address.</returns>
    int address_length(void) const noexcept;

    /// <summary>
    /// Gets the timestamp of the last update to the client.
    /// </summary>
    /// <returns></returns>
    time_point last_update(void) const noexcept;

    /// <summary>
    /// Updates the timestamp of when the client was last seen.
    /// </summary>
    void update(void) noexcept;

    /// <summary>
    /// Gets a pointer to the address of the client.
    /// </summary>
    /// <returns>The address of the client.</returns>
    inline operator const sockaddr *(void) const noexcept {
        return reinterpret_cast<const sockaddr *>(&this->_address);
    }

private:

    typedef duration::rep timestamp;

    sockaddr_storage _address;
    std::atomic<timestamp> _last_update;

    friend struct std::less<client>;
};


/// <summary>
/// Compares two clients for using them in a set or map.
/// </summary>
template<> struct std::less<client> {

    bool operator ()(_In_ const client& lhs,
        _In_ const client& rhs) const noexcept;
};
