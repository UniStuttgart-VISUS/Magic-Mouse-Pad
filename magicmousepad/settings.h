// <copyright file="settings.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <string>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <nlohmann/json.hpp>


/// <summary>
/// The application settings loaded from the configuration file.
/// </summary>
class settings final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    settings(void) noexcept;

    /// <summary>
    /// Gets the address the server should be bound to.
    /// </summary>
    /// <returns></returns>
    inline const sockaddr_storage& address(void) const noexcept {
        return this->_address;
    }

    /// <summary>
    /// Gets the height of the mouse pad in pixels. If this height is zero,
    /// the scrolling area is unbounded vertically.
    /// </summary>
    /// <returns></returns>
    inline std::size_t height(void) const noexcept {
        return this->_height;
    }

    /// <summary>
    /// Gets the width of the mouse pad in pixels. If this width is zero,
    /// the scrolling area is unbounded horizontally.
    /// </summary>
    /// <returns></returns>
    inline std::size_t width(void) const noexcept {
        return this->_width;
    }

private:

    sockaddr_storage _address;
    std::size_t _height;
    std::size_t _width;

    friend struct nlohmann::adl_serializer<settings>;
};


/// <summary>
/// Specialisation of the JSON serialiser for the <see cref="settings"/> class.
/// </summary>
template<> struct nlohmann::adl_serializer<settings> {

    /// <inheritdoc />
    static nlohmann::json to_json(_In_ const settings& value) noexcept;

    /// <inheritdoc />
    static settings from_json(_In_ const nlohmann::json& json) noexcept;
};
