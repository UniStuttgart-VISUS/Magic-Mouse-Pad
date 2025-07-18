﻿// <copyright file="server.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <atomic>
#include <memory>
#include <mmpmsg.h>
#include <mutex>
#include <stdexcept>
#include <set>
#include <thread>
#include <vector>

#include "client.h"
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

    /// <summary>
    /// Sends the specified datagram to all connected clients.
    /// </summary>
    /// <param name="data"></param>
    /// <param name="cnt"></param>
    void send(_In_reads_(cnt) const char *data, _In_ const int cnt) noexcept;

    /// <summary>
    /// Sends the specified message to all connected clients.
    /// </summary>
    /// <remarks>
    /// The method will update the sequence number in the message before sending
    /// it.
    /// </remarks>
    /// <typeparam name="TMessage"></typeparam>
    /// <param name="message"></param>
    template<class TMessage>
    inline void send(_In_ TMessage& message) noexcept {
        message.sequence_number = ::htonl(this->_sequence_number++);
        this->send(reinterpret_cast<const char *>(std::addressof(message)),
            static_cast<int>(sizeof(TMessage)));
    }

private:

    static std::vector<sockaddr_storage> addresses(void);

    static void copy_port(_In_ sockaddr_storage& dst,
        _In_ const sockaddr *src);

    static std::uint16_t get_port(_In_ const sockaddr *src);

    static void set_port(_In_ sockaddr *dst, _In_ const std::uint16_t port);

    static std::wstring to_string(_In_ const sockaddr_storage& address);

    //sockaddr_storage address(_In_ const sockaddr_storage& peer);

    void serve(_In_ settings settings);

    std::set<client> _clients;
    std::mutex _lock;
    std::atomic<bool> _running;
    std::atomic<mmp_seq_no> _sequence_number;
    wil::unique_socket _socket;
    std::thread _server;
    HWND _window;

};
