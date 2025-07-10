// <copyright file="mmpmsg.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_MMPMSG_H)
#define _MMPMSG_H
#pragma once

#include <cinttypes>

#include "mmpapi.h"


/// <summary>
/// The type used to identify the message type.
/// </summary>
typedef int32_t mmp_msg_id;


/// <summary>
/// A message for discovering the magic mouse pad.
/// </summary>
typedef struct MMPCLI_API mmp_msg_discover_t {
    /// <summary>
    /// The message ID.
    /// </summary>
    mmp_msg_id id;
} mmp_msg_discover;


/// <summary>
/// The response the magic mouse pad sends when it receives a
/// <see cref="mmp_msg_discover"/> message.
/// </summary>
typedef struct MMPCLI_API mmp_msg_announce_t {
    /// <summary>
    /// The message ID.
    /// </summary>
    mmp_msg_id id;

    /// <summary>
    /// The address of the magic mouse pad.
    /// </summary>
    struct sockaddr_storage address;
} mmp_msg_announce;


/// <summary>
/// The client sends this as the first message to make itself known to the
/// magic mouse pad.
/// </summary>
typedef struct MMPCLI_API mmp_msg_connect_t {
    /// <summary>
    /// The message ID.
    /// </summary>
    mmp_msg_id id;

} mmp_msg_connect;

#endif /* !defined(_MMPMSG_H) */
