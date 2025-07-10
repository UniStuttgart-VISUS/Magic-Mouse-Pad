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


/// <summary>
/// The type used to identify the message type.
/// </summary>
typedef int32_t mmp_msg_id;


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
    /// The message ID, which must be in network-byte order on the wire..
    /// </summary>
    mmp_msg_id id;

    /// <summary>
    /// The port the maging mouse pad is listening on. The receiver should use
    /// address it got this message from, but if a port other than zero is 
    /// specified here, it should switch the port to this value.
    /// </summary>
    /// <remarks>
    /// The port is in network-byte order and can directly be copied into a
    /// socket address.
    /// </remarks>
    uint16_t port;

#if defined(__cplusplus)
    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    inline mmp_msg_announce_t(void) noexcept : id(::htonl(mmp_msgid_announce)),
        port(0) { }
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



#endif /* !defined(_MMPMSG_H) */
