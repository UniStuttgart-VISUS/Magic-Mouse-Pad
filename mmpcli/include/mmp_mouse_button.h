// <copyright file="mmp_mouse_button.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_MMP_MOUSE_BUTTON_H)
#define _MMP_MOUSE_BUTTON_H
#pragma once

#include <inttypes.h>   


/// <summary>
/// Identifies mouse buttons that the magic mouse pad can report to the client.
/// </summary>
typedef uint8_t mmp_mouse_button;


/// <summary>
/// None of the buttons is selected. This should never be reported.
/// </summary>
#define mmp_mouse_button_none ((mmp_mouse_button) 0)

/// <summary>
/// Identifies the left mouse button.
/// </summary>
#define mmp_mouse_button_left ((mmp_mouse_button) 0x01)

/// <summary>
/// Identifies the right mouse button.
/// </summary>
#define mmp_mouse_button_right ((mmp_mouse_button) 0x02)

/// <summary>
/// Identifies the middle mouse button.
/// </summary>
#define mmp_mouse_button_middle ((mmp_mouse_button) 0x10)

/// <summary>
/// Identifies the X1 button.
/// </summary>
#define mmp_mouse_button_x1 ((mmp_mouse_button) 0x20)

/// <summary>
/// Identifies the X2 button.
/// </summary>
#define mmp_mouse_button_x2 ((mmp_mouse_button) 0x40)

#endif /* !defined(_MMP_MOUSE_BUTTON_H) */
