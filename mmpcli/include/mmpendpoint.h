// <copyright file="mmpendpoint.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_MMPENDPOINT_H)
#define _MMPENDPOINT_H
#pragma once

#include "mmpapi.h"

/// <summary>
/// Parses the given string as end point (service) address, either IPv4 or
/// IPv6 or DNS name.
/// <summary>
/// <param name="end_point">The end point to be filled in case of success.
/// </param>
/// <param name="string">The string to be parsed.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
extern "C" int MMPCLI_API mmp_parse_end_pointa(
    _Out_ sockaddr_storage *end_point,
    _In_z_ const char *string);

/// <summary>
/// Parses the given string as end point (service) address, either IPv4 or
/// IPv6 or DNS name.
/// <summary>
/// <param name="end_point">The end point to be filled in case of success.
/// </param>
/// <param name="string">The string to be parsed.</param>
/// <returns>Zero in case of success, a system error code otherwise.</returns>
extern "C" int MMPCLI_API mmp_parse_end_pointw(
    _Out_ sockaddr_storage *end_point,
    _In_z_ const wchar_t *string);


#if (defined(UNICODE) || defined(_UNICODE))
#define mmp_parse_end_point mmp_parse_end_pointw
#else /* (defined(UNICODE) && !defined(_UNICODE)) */
#define mmp_parse_end_point mmp_parse_end_pointa
#endif /* (defined(UNICODE) && !defined(_UNICODE)) */

#endif /* !defined(_MMPENDPOINT_H) */
