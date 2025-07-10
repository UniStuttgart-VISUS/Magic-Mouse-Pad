// <copyright file="mmpapi.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_MMPAPI_H)
#define _MMPAPI_H
#pragma once

#if defined(_WIN32)
#include <sal.h>
#include <WinSock2.h>
#include <Ws2def.h>
#include <Ws2ipdef.h>
#include <Windows.h>


#if defined(MMPCLI_EXPORTS)
#define MMPCLI_API __declspec(dllexport)
#else /* defined(MMPCLI_EXPORTS) */
#define MMPCLI_API __declspec(dllimport)
#endif /* defined(MMPCLI_EXPORTS) */

#else /* defined(_WIN32) */

#define MMPCLI_API

#endif /* defined(_WIN32) */

#endif /* !defined(_MMPAPI_H) */
