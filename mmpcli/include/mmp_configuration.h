// <copyright file="mmp_configuration.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_MMP_CONFIGURATION_H)
#define _MMP_CONFIGURATION_H
#pragma once

#include <inttypes.h>
#include <WinSock2.h>

#include "mmpapi.h"
#include "mmp_mouse_button.h"
#include "mmp_key.h"


/// <summary>
/// If this flag is set in the <see cref="mmp_configuration"/>, the client
/// library will clip the mouse coordinates to the virtual screen size of
/// <see cref="mmp_configuration::width"/> and
/// <see cref="mmp_configuration::height"/>.
/// </summary>
#define mmp_flag_clip ((uint32_t) 0x00000001)

/// <summary>
/// If this flag is set in the <see cref="mmp_configuration"/>, the client
/// library will offset the mouse position by the local offset specified in
/// <see cref="mmp_configuration::offset_x"/> and
/// <see cref="mmp_configuration::offset_y"/>. If <see cref="mmp_flag_clip" />
/// is also set, the coordinates are additionally clipped against the size
/// of the local desktop.
/// </summary>
#define mmp_flag_local ((uint32_t) 0x00000002)

/// <summary>
/// If this flag is set in the <see cref="mmp_configuration"/>, the client will
/// hide the cursor when it is not visible on the local desktop.
/// </summary>
#define mmp_flag_hide_remote ((uint32_t) 0x00000004)


/// <summary>
/// The default port number to be used for the magic mouse pad.
/// </summary>
/// <remarks>
/// The port number is in host-byte order.
/// </remarks>
#define mmp_default_port ((uint16_t) 14753)


/// <summary>
/// Configures a magic mouse pad client.
/// </summary>
typedef struct MMPCLI_API mmp_configuration_t {

    /// <summary>
    /// The address the client binds to.
    /// </summary>
    struct sockaddr_storage client;

    /// <summary>
    /// A user-defined context pointer that is passed to callbacks.
    /// </summary>
    void *context;

    /// <summary>
    /// A set of flags the control the behaviour of the client library.
    /// </summary>
    uint32_t flags;

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
    int32_t offset_x;

    /// <summary>
    /// The vertical offset of the local instance in the overall range of pixels
    /// the mouse can travel. The client subtracts this offset from the global
    /// mouse position it receives from the magic mouse pad to determine the
    /// location of the mouse on the local desktop.
    /// </summary>
    int32_t offset_y;

    /// <summary>
    /// The callback that is invoked when a mouse button is pressed or released.
    /// </summary>
    void (CALLBACK *on_mouse_button)(_In_ const mmp_mouse_button,
        _In_ const bool, _In_ const int32_t, _In_ const int32_t,
        _In_opt_ void *);

    /// <summary>
    /// The callback that is invoked when the mouse is moved.
    /// </summary>
    void (CALLBACK *on_mouse_move)(_In_ const int32_t, _In_ const int32_t,
        _In_opt_ void *);

    /// <summary>
    /// The time in milliseconds before retrying discovery. This should be less
    /// than <paramref name="timeout"/>, but definitely greater than zero to
    /// prevent the network from being flooded with discovery requests.
    /// </summary>
    uint32_t rate_limit;

    /// <summary>
    /// The address of the magic mouse pad to connect to. If the address itself
    /// is the any address, the client will attempt to discover the mouse pad
    /// instance via the port specified in the address.
    /// </summary>
    struct sockaddr_storage server;

    /// <summary>
    /// The horizontal starting position when the first position from the magic
    /// mouse pad is received. Typically, callers would set this to the centre
    /// of the virtual screen for best user experience.
    /// </summary>
    int32_t start_x;

    /// <summary>
    /// The vertical starting position when the first position from the magic
    /// mouse pad is received. Typically, callers would set this to the centre
    /// of the virtual screen for best user experience.
    /// </summary>
    int32_t start_y;

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

#if defined(__cplusplus)
    /// <summary>
    /// Default constructor for the configuration.
    /// </summary>
    inline mmp_configuration_t(void) noexcept
        : client({ 0 }),
        context(nullptr),
        flags(0),
        height(0),
        offset_x(0),
        offset_y(0),
        on_mouse_button(nullptr),
        on_mouse_move(nullptr),
        rate_limit(0),
        timeout(0),
        server({ 0 }),
        start_x(0),
        start_y(0),
        width(0) { }
#endif /* defined(__cplusplus) */

} mmp_configuration;


#if defined(__cplusplus)
extern "C" {
#endif define(__cplusplus)

/// <summary>
/// Adjusts the <paramref name="configuration"/> to bind to the given client
/// address.
/// </summary>
/// <param name="configuration">The configuration object to be adjusted.</param>
/// <param name="client">The address to bind the client to.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
_Success_(return == 0) MMPCLI_API int mmp_configure_client4(
    _In_ mmp_configuration *configuration,
    _In_ const struct sockaddr_in *client);


/// <summary>
/// Adjusts the <paramref name="configuration"/> to bind to the given client
/// address.
/// </summary>
/// <param name="configuration">The configuration object to be adjusted.</param>
/// <param name="client">The address to bind the client to.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
_Success_(return == 0) MMPCLI_API int mmp_configure_client6(
    _In_ mmp_configuration *configuration,
    _In_ const struct sockaddr_in6 *client);


/// <summary>
/// Adjusts the <paramref name="configuration"/> to discover the magic mouse pad
/// via UDP broadcasts on the given <paramref name="port"/>.
/// </summary>
/// <param name="configuration"></param>
/// <param name="port"></param>
/// <param name="timeout">The timeout in milliseconds within which the discovery
/// must succeed. A value of zero indicates an infinite timeout.</param>
/// <param name="rate_limit">The time in milliseconds before retrying discovery.
/// This should be less than <paramref name="timeout"/>, but definitely greater
/// than zero to prevent the network from being flooded with discovery requests.
/// <returns></returns>
_Success_(return == 0) MMPCLI_API int mmp_configure_discovery(
    _In_ mmp_configuration *configuration,
    _In_ const uint16_t port,
    _In_ const uint32_t timeout,
    _In_ const uint32_t rate_limit);


#if defined(_WIN32)
/// <summary>
/// Loads the configuration from the given Windows registry key.
/// </summary>
/// <param name="configuration">The configuration object to receive the data
/// from the registry.</param>
/// <param name="key">The registry key to load the data from. The caller remains
/// owner of this key and must release it once the function returned.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
_Success_(return == 0) MMPCLI_API int mmp_configure_from_registry_key(
    _In_ mmp_configuration *configuration,
    _In_ HKEY key);
#endif /* defined(_WIN32) */


/// <summary>
/// Adjusts the <paramref name="configuration"/> to use connect to the given
/// server using IPv4.
/// </summary>
/// <param name="configuration">The configuration object to be adjusted.</param>
/// <param name="server">The address of the server to connect to.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
_Success_(return == 0) MMPCLI_API int mmp_configure_server4(
    _In_ mmp_configuration *configuration,
    _In_ const struct sockaddr_in *server);


/// <summary>
/// Adjusts the <paramref name="configuration"/> to use connect to the given
/// server using IPv6.
/// </summary>
/// <param name="configuration">The configuration object to be adjusted.</param>
/// <param name="server">The address of the server to connect to.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
_Success_(return == 0) MMPCLI_API int mmp_configure_server6(
    _In_ mmp_configuration *configuration,
    _In_ const struct sockaddr_in6 *server);


/// <summary>
/// Adjusts the <paramref name="configuration"/> to use connect to the server
/// end point specified by the <paramref name="server"/> parameter.
/// </summary>
/// <param name="configuration">The configuration object to be adjusted.</param>
/// <param name="server">The end point address of the server.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
_Success_(return == 0) MMPCLI_API int mmp_configure_servera(
    _In_ mmp_configuration *configuration,
    _In_z_ const char *server);


/// <summary>
/// Adjusts the <paramref name="configuration"/> to use connect to the server
/// end point specified by the <paramref name="server"/> parameter.
/// </summary>
/// <param name="configuration">The configuration object to be adjusted.</param>
/// <param name="server">The end point address of the server.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
_Success_(return == 0) MMPCLI_API int mmp_configure_serverw(
    _In_ mmp_configuration *configuration,
    _In_z_ const wchar_t *server);


#if (defined(UNICODE) || defined(_UNICODE))
#define mmp_configure_server mmp_configure_serverw
#else /* !defined(UNICODE) && !defined(_UNICODE) */
#define mmp_configure_server mmp_configure_servera
#endif /* (defined(UNICODE) || defined(_UNICODE)) */

#if defined(__cplusplus)
} /* extern "C" */
#endif define(__cplusplus)

#endif /* !defined(_MMP_CONFIGURATION_H) */
