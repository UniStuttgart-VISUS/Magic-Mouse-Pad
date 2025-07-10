// <copyright file="server.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <atomic>
#include <mutex>
#include <stdexcept>
#include <set>
#include <thread>
#include <vector>

#include <WinSock2.h>

#include <wil/resource.h>

#include "settings.h"


/// <summary>
/// The server keeps track of all the clients connected to the mouse pad that
/// receive updates from it.
/// </summary>
class server final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="settings"></param>
    /// <param name="window">The handle of a window that will be invalidated
    /// whenever the client list changes.</param>
    explicit server(_In_ const settings& settings, _In_opt_ HWND window = NULL);

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~server(void) noexcept;

private:

    static std::vector<sockaddr_storage> addresses(void);

    static void copy_port(_In_ sockaddr_storage& dst,
        _In_ const sockaddr *src);

    static std::uint16_t get_port(_In_ const sockaddr *src);

    sockaddr_storage address(_In_ const sockaddr_storage& peer);

    void serve(_In_ const settings settings);

    std::vector<sockaddr_storage> _addresses;
    std::set<sockaddr_storage> _clients;
    std::mutex _lock;
    std::atomic<bool> _running;
    wil::unique_socket _socket;
    std::thread _server;
    HWND _window;

};
