// <copyright file="settings.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <cinttypes>
#include <functional>
#include <string>

#include <WinSock2.h>
#include <Windows.h>
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
    inline const sockaddr *address(void) const noexcept {
        return reinterpret_cast<const sockaddr *>(&this->_address);
    }

    /// <summary>
    /// Answer the size of the server address in bytes.
    /// </summary>
    /// <returns></returns>
    int address_length(void) const noexcept;

    /// <summary>
    /// Gets the height of the mouse pad in pixels. If this height is zero,
    /// the scrolling area is unbounded vertically.
    /// </summary>
    /// <returns></returns>
    inline std::uint32_t height(void) const noexcept {
        return this->_height;
    }

    /// <summary>
    /// Loads the settings from the given registry key.
    /// </summary>
    /// <param name="key">A registry key holding the configuration values to
    /// be loaded.</param>
    void load(_In_ HKEY key);

    /// <summary>
    /// Gets the width of the mouse pad in pixels. If this width is zero,
    /// the scrolling area is unbounded horizontally.
    /// </summary>
    /// <returns></returns>
    inline std::uint32_t width(void) const noexcept {
        return this->_width;
    }

private:

    sockaddr_storage _address;
    std::uint32_t _height;
    std::uint32_t _width;

    friend struct nlohmann::adl_serializer<settings>;
};


/// <summary>
/// Specialisation of the JSON serialiser for the <see cref="settings"/> class.
/// </summary>
template<> struct nlohmann::adl_serializer<settings> {

    static nlohmann::json to_json(_In_ const settings& value) noexcept;

    static settings from_json(_In_ const nlohmann::json& json) noexcept;
};


/// <summary>
/// Compares two socket addresses for sorting, which allows addresses to be used
/// in sets.
/// </summary>
template<> struct std::less<sockaddr_storage> {

    bool operator ()(_In_ const sockaddr_storage& lhs,
        _In_ const sockaddr_storage& rhs) const noexcept;
};
