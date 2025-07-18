﻿// <copyright file="mmpcli.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_MMPCLI_H)
#define _MMPCLI_H
#pragma once

#if defined(__cplusplus)
#include <memory>
#include <system_error>
#endif define(__cplusplus)

#include "mmp_configuration.h"


/// <summary>
/// The handle to a magic mouse pad client.
/// </summary>
typedef struct mmp_client *mmp_handle;


#if defined(__cplusplus)
extern "C" {
#endif define(__cplusplus)

/// <summary>
/// Connects to the magic mouse pad configured in
/// <paramref name="configuration"/> and returns a handle to the connection
/// in case of success.
/// </summary>
/// <param name="handle">Receives the handle for the client.</param>
/// <param name="configuration">The configuration for the mouse pad.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
_Success_(return == 0) MMPCLI_API int mmp_connect(
    _Out_ mmp_handle *handle,
    _In_ mmp_configuration *configuration);


/// <summary>
/// Disconnects from the magic mouse pad and releases all client-side resources.
/// </summary>
/// <param name="handle">The handle representing the client connection to be
/// terminated.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
_Success_(return == 0) MMPCLI_API int mmp_disconnect(
    _In_ mmp_handle handle);

#if defined(__cplusplus)
} /* extern "C" */
#endif define(__cplusplus)


#if defined(__cplusplus)
namespace visus {
namespace mmp {
namespace detail {

    /// <summary>
    /// A deleter for magic mouse pad handles.
    /// </summary>
    struct MMPCLI_API delete_mmp_handle final {
        void operator ()(_In_opt_ mmp_handle handle) const noexcept;
    };

} /* namespace detail */

    /// <summary>
    /// A unique pointer that automatically disconnects the client when being
    /// destroyed.
    /// </summary>
    typedef std::unique_ptr<mmp_client, detail::delete_mmp_handle> unique_handle;


    /// <summary>
    /// Connects to the magic mouse pad configured in
    /// <paramref name="configuration"/> and returns a handle to the connection
    /// in case of success.
    /// </summary>
    /// <param name="configuration">The configuration for the mouse pad.</param>
    /// <returns>A handle for the connection.</returns>
    /// <exception cref="std::system_error">If the connection could not be
    /// established.</exception>
    inline unique_handle connect(_In_ mmp_configuration& configuration) {
        mmp_handle handle;
        auto status = ::mmp_connect(&handle, &configuration);

        if (status != 0) {
            throw std::system_error(status, std::system_category());
        }

        return unique_handle(handle);
    }

} /* namespace mmp */
} /* namespace visus */
#endif define(__cplusplus)

#endif /* !defined(_MMPCLI_H) */
