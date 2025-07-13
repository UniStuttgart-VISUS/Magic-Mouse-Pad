// <copyright file="mmpmsg.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_MMPMSG_H)
#define _MMPMSG_H
#pragma once

#include <inttypes.h>
#include <string.h>

#include "mmpapi.h"
#include "mmp_key.h"
#include "mmp_mouse_button.h"


/// <summary>
/// The type used to identify the message type.
/// </summary>
typedef uint32_t mmp_msg_id;


/// <summary>
/// The type used for sequence numbers.
/// </summary>
typedef uint32_t mmp_seq_no;


#define mmp_msgid_discover ((mmp_msg_id) 0x00000001)

/// <summary>
/// A message for discovering the magic mouse pad.
/// </summary>
typedef struct MMPCLI_API mmp_msg_discover_t {
    /// <summary>
    /// The message ID, which must be in network-byte order on the wire.
    /// </summary>
    mmp_msg_id id;

#if defined(__cplusplus)
    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline mmp_msg_discover_t(void) noexcept
        : id(::htonl(mmp_msgid_discover)) { }
#endif /* defined(__cplusplus) */
} mmp_msg_discover;


#define mmp_msgid_announce ((mmp_msg_id) 0x00000002)

/// <summary>
/// The response the magic mouse pad sends when it receives a
/// <see cref="mmp_msg_discover"/> message.
/// </summary>
typedef struct MMPCLI_API mmp_msg_announce_t {
    /// <summary>
    /// The message ID, which must be in network-byte order on the wire.
    /// </summary>
    mmp_msg_id id;

    /// <summary>
    /// The sequence number of the server at the time when the announcement was
    /// sent, in network-byte order.
    /// </summary>
    mmp_seq_no sequence_number;

#if defined(__cplusplus)
    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline mmp_msg_announce_t(void) noexcept : id(::htonl(mmp_msgid_announce)),
        sequence_number(0) { }
#endif /* defined(__cplusplus) */
} mmp_msg_announce;


#define mmp_msgid_connect ((mmp_msg_id) 0x00000100)

/// <summary>
/// The client sends this as the first message to make itself known to the
/// magic mouse pad.
/// </summary>
typedef struct MMPCLI_API mmp_msg_connect_t {
    /// <summary>
    /// The message ID, which must be in network-byte order on the wire.
    /// </summary>
    mmp_msg_id id;

#if defined(__cplusplus)
    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline mmp_msg_connect_t(void) noexcept : id(::htonl(mmp_msgid_connect)) { }
#endif /* defined(__cplusplus) */
} mmp_msg_connect;


#define mmp_msgid_mouse_move ((mmp_msg_id) 0x00001000)

/// <summary>
/// The server sends this datagram to all clients to update the mouse cursor
/// relative to the virtual screen.
/// </summary>
typedef struct MMPCLI_API mmp_msg_mouse_move_t {
    /// <summary>
    /// The message ID, which must be in network-byte order on the wire.
    /// </summary>
    mmp_msg_id id;

    /// <summary>
    /// The sequence number of the server, in network-byte order. Clients can
    /// use this information to discard outdated messages.
    /// </summary>
    mmp_seq_no sequence_number;

    /// <summary>
    /// The horizontal position in pixels, in network-byte order.
    /// </summary>
    int32_t x;

    /// <summary>
    /// The horizontal position in pixels, in network-byte order.
    /// </summary>
    int32_t y;

#if defined(__cplusplus)
    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline mmp_msg_mouse_move_t(void) noexcept
        : id(::htonl(mmp_msgid_mouse_move)), sequence_number(0), x(0), y(0) { }
#endif /* defined(__cplusplus) */
} mmp_msg_mouse_move;


#define mmp_msgid_mouse_button ((mmp_msg_id) 0x00001001)

/// <summary>
/// The server sends this datagram to all clients to notify them about a mouse
/// button being pressed or released.
/// </summary>
typedef struct MMPCLI_API mmp_msg_mouse_button_t {
    /// <summary>
    /// The message ID, which must be in network-byte order on the wire.
    /// </summary>
    mmp_msg_id id;

    /// <summary>
    /// The sequence number of the server, in network-byte order. Clients can
    /// use this information to discard outdated messages.
    /// </summary>
    mmp_seq_no sequence_number;

    /// <summary>
    /// Identifies the button that was pressed or released.
    /// </summary>
    mmp_mouse_button button;

    /// <summary>
    /// Indicates whether the button was pressed (<see langword="true"/>) or
    /// released (<see langword="false"/>).
    /// </summary>
    bool down;

    /// <summary>
    /// The horizontal position in pixels, in network-byte order.
    /// </summary>
    int32_t x;

    /// <summary>
    /// The horizontal position in pixels, in network-byte order.
    /// </summary>
    int32_t y;

#if defined(__cplusplus)
    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline mmp_msg_mouse_button_t(void) noexcept
        : id(::htonl(mmp_msgid_mouse_button)),
        sequence_number(0),
        button(mmp_mouse_button_none),
        down(false),
        x(0),
        y(0) { }
#endif /* defined(__cplusplus) */
} mmp_msg_mouse_button;

#endif /* !defined(_MMPMSG_H) */
