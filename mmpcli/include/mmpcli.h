// <copyright file="mmpcli.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_MMPCLI_H)
#define _MMPCLI_H
#pragma once

#include "mmp_configuration.h"


/// <summary>
/// The handle to a magic mouse pad client.
/// </summary>
typedef struct mmp_client *mmp_handle;


/// <summary>
/// Connects to the magic mouse pad configured in
/// <paramref name="configuration"/> and returns a handle to the connection
/// in case of success.
/// </summary>
/// <param name="handle">Receives the handle for the client.</param>
/// <param name="configuration">The configuration for the mouse pad.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
_Success_(return == 0) extern "C" MMPCLI_API int mmp_connect(
    _Out_ mmp_handle *handle,
    _In_ const mmp_configuration *configuration);

/// <summary>
/// Disconnects from the magic mouse pad and releases all client-side resources.
/// </summary>
/// <param name="handle">The handle representing the client connection to be
/// terminated.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
_Success_(return == 0) extern "C" MMPCLI_API int mmp_disconnect(
    _In_ mmp_handle handle);

#endif /* !defined(_MMPCLI_H) */
