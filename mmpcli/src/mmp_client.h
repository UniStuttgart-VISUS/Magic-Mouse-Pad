// <copyright file="mmp_client.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "mmpcli.h"

#include <cassert>
#include <thread>
#include <vector>

#include <mmpmsg.h>

#include <wil/resource.h>


/// <summary>
/// Represents the magic mouse pad client.
/// </summary>
struct mmp_client final {

public:

    /// <summary>
    /// If no specific IP address is specified in the given
    /// <see cref="mmp_configuration"/>.
    /// </summary>
    /// <param name="config"></param>
    /// <returns></returns>
    static _Success_(return == 0) int discover(_In_ mmp_configuration& config);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="config">The configuration of the client. The object will
    /// create a copy of this structure for future use.</param>
    explicit mmp_client(_In_ const mmp_configuration& config);

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~mmp_client(void) noexcept;

private:

    /// <summary>
    /// Answer the contents of <paramref name="buffer"/> as a pointer to the
    /// specified <typeparamref name="TType"/>.
    /// </summary>
    /// <typeparam name="TType"></typeparam>
    /// <param name="buffer"></param>
    /// <returns></returns>
    template<class TType>
    static const TType *as(_In_ const std::vector<char>& buffer) {
        assert(buffer.size() >= sizeof(TType));
        return reinterpret_cast<const TType *>(buffer.data());
    }

    /// <summary>
    /// Find the broadcast addresses using the given <paramref name="port"/> for
    /// all active adapters on the system.
    /// </summary>
    /// <param name="port"></param>
    /// <returns></returns>
    static std::vector<sockaddr_in> bcast_addresses(
        _In_ const std::uint16_t port);

    /// <summary>
    /// Bind the given <paramref name="socket"/> to the specified
    /// <paramref name="address"/>.
    /// </summary>
    /// <param name="socket">The socket to be bound.</param>
    /// <param name="address">The address to bind to. If this is neither IPv4
    /// nor IPv6, a valid choice will be forced.</param>
    /// <returns>Zero in case of success, a system error code otherwise.
    /// </returns>
    static int bind(_In_ wil::unique_socket& socket,
        _In_ sockaddr_storage& address);

    /// <summary>
    /// Creates a new datagram socket.
    /// </summary>
    /// <param name="config">The configuration for which the socket should be
    /// used.</param>
    /// <returns></returns>
    static wil::unique_socket make_socket(_In_ const mmp_configuration& config);

    /// <summary>
    /// Sends an announcement message to the mouse pad server in order to
    /// receive updates from it.
    /// </summary>
    /// <returns>Zero in case of success, a system error code otherwise.
    /// </returns>
    int announce(void);

    //void on_mouse_button(_In_ const mmp_msg_mouse_button);

    /// <summary>
    /// Processes a mouse move message.
    /// </summary>
    void on_mouse_move(_In_ const mmp_msg_mouse_move *msg);

    /// <summary>
    /// Receives messages from the magic mouse pad in a separate thread.
    /// </summary>
    void receive(void);

    mmp_configuration _config;
    std::thread _receiver;
    std::atomic<bool> _running;
    wil::unique_socket _socket;
};
