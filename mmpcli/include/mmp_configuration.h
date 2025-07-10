// <copyright file="mmp_configuration.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_MMP_CONFIGURATION_H)
#define _MMP_CONFIGURATION_H
#pragma once

#include <cinttypes>

#include "mmpapi.h"


/// <summary>
/// Configures a magic mouse pad client.
/// </summary>
typedef struct MMPCLI_API mmp_configuration_t {

    /// <summary>
    /// The address of the magic mouse pad to connect to. If the address itself
    /// is the any address, the client will attempt to discover the mouse pad
    /// instance via the port specified in the address.
    /// </summary>
    struct sockaddr_storage address;

    /// <summary>
    /// A timeout im milliseconds for a connection attempt. If this value is
    /// zero, the client will wait indefinitely.
    /// </summary>
    uint32_t timeout;

    /// <summary>
    /// The width of the overall range the mouse can travel horizontally in
    /// number of pixels.
    /// </summary>
    uint32_t width;

    /// <summary>
    /// The height of the overall range the mouse can travel vertically in
    /// number of pixels.
    /// </summary>
    uint32_t height;

    /// <summary>
    /// The horizontal offset of the local instance in the overall range of
    /// pixels the mouse can travel. The client subtracts this offset from the
    /// global mouse position it receives from the magic mouse pad to determine
    /// the location of the mouse on the local desktop.
    /// </summary>
    int32_t x;

    /// <summary>
    /// The vertical offset of the local instance in the overall range of pixels
    /// the mouse can travel. The client subtracts this offset from the global
    /// mouse position it receives from the magic mouse pad to determine the
    /// location of the mouse on the local desktop.
    /// </summary>
    int32_t y;

#if defined(__cplusplus)
    /// <summary>
    /// Default constructor for the configuration.
    /// </summary>
    inline mmp_configuration_t(void) noexcept
            : timeout(0), width(0), height(0), x(0), y(0) {
        ::memset(&this->address, 0, sizeof(this->address));
    }
#endif /* defined(__cplusplus) */

} mmp_configuration;

#endif /* !defined(_MMP_CONFIGURATION_H) */
