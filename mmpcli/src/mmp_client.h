// <copyright file="mmp_client.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "mmpcli.h"

#include <cassert>
#include <string>
#include <thread>
#include <vector>

#include "mmpmsg.h"
#include "mmptrace.h"

#include <wil/resource.h>


/// <summary>
/// Represents the magic mouse pad client.
/// </summary>
struct mmp_client final {

public:

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

    /// <summary>
    /// If no specific IP address is specified in the
    /// <see cref="mmp_configuration"/> of the client, try to discover the magic
    /// mouse pad using UDP broadcast messages.
    /// </summary>
    /// <param name="config"></param>
    /// <returns></returns>
    _Success_(return == 0) int discover(void);

    /// <summary>
    /// Announces the client to the configured server and starts the receiver
    /// thread.
    /// </summary>
    /// <returns>Zero in case of success, a system error code otherwise.
    /// </returns>
    _Success_(return == 0) int start(void) noexcept;

#if defined(_WIN32)
    /// <summary>
    /// Answer the Winsock initialisation data used by the client. The caller
    /// must initialise Winsock using this structure before calling any other
    /// method that might use sockets.
    /// </summary>
    _Ret_valid_ operator WSADATA *(void) noexcept {
        return std::addressof(this->_wsa_data);
    }
#endif /* defined(_WIN32) */

private:

    /// <summary>
    /// The type used for receiving messages.
    /// </summary>
    typedef std::vector<char> buffer_type;

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
    /// Tests whether the given position <paramref name="x"/> and
    /// <paramref name="y"/> is within the given bounds and if not, clips it
    /// to the <paramref name="width"/> and <paramref name="height"/>.
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="width"></param>
    /// <param name="height"></param>
    /// <returns><see langword="true" /> if the position was clipped,
    /// <see langword="false" /> if it was already in the specified range.
    /// </returns>
    static bool clip(_Inout_ std::int32_t& x,
        _Inout_ std::int32_t& y,
        _In_ const std::uint32_t width,
        _In_ const std::uint32_t height) noexcept;

    /// <summary>
    /// Convert the given socket <paramref name="address"/> into a
    /// human-readable representation.
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    static std::wstring to_string(_In_ const sockaddr_storage& address);

    /// <summary>
    /// Sends an announcement message to the mouse pad server in order to
    /// receive updates from it.
    /// </summary>
    /// <returns>Zero in case of success, a system error code otherwise.
    /// </returns>
    int connect(void);

    /// <summary>
    /// Processes a button press or release message.
    /// </summary>
    void on_mouse_button(_In_ const mmp_msg_mouse_button *msg);

    /// <summary>
    /// Processes a mouse move message.
    /// </summary>
    void on_mouse_move(_In_ const mmp_msg_mouse_move *msg);

    /// <summary>
    /// Receives messages from the magic mouse pad in a separate thread.
    /// </summary>
    void receive(void);

    /// <summary>
    /// Receives a datagram into <paramref name="buffer"/>.
    /// </summary>
    _Success_(return == true) bool receive_from(_In_ std::vector<char>& buffer,
        _Out_ DWORD&size, _Out_ sockaddr_storage& peer);

    /// <summary>
    /// Tracks whether the sequence number in the given
    /// <paramref name="message"/> is not from the past and updates the current
    /// state.
    /// </summary>
    /// <typeparam name="TMessage"></typeparam>
    /// <param name="message"></param>
    /// <returns></returns>
    template<class TMessage>
    bool track_sequence_number(_In_ const TMessage *message);

    /// <summary>
    /// Transforms the position in the given <paramref name="message"/>
    /// according to the rules specified in <see cref="_config"/>.
    /// </summary>
    /// <typeparam name="TMessage"></typeparam>
    /// <param name="message"></param>
    /// <returns></returns>
    template<class TMessage>
    std::pair<std::int32_t, std::int32_t> xform_position(
        _In_ const TMessage *message);

    mmp_configuration _config;
    wil::unique_event_nothrow _event;
    std::thread _receiver;
    std::vector<buffer_type> _reordering_buffer;
    std::atomic<bool> _running;
    std::atomic<mmp_seq_no> _sequence_number;
    wil::unique_socket _socket;
    WSADATA _wsa_data;

};

#include "mmp_client.inl"
