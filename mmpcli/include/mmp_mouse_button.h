// <copyright file="mmp_mouse_button.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_MMP_MOUSE_BUTTON_H)
#define _MMP_MOUSE_BUTTON_H
#pragma once


/// <summary>
/// Enumerates possible mouse buttons that the magic mouse pad can report to the
/// client.
/// </summary>
typedef enum mmp_mouse_button_t {
    /// <summary>
    /// None of the buttons is selected. This should never be reported.
    /// </summary>
    mmp_mouse_button_none = 0,

    /// <summary>
    /// Identifies the left mouse button.
    /// </summary>
    mmp_mouse_button_left = 0x0001,

    /// <summary>
    /// Identifies the right mouse button.
    /// </summary>
    mmp_mouse_button_right = 0x0002,

    /// <summary>
    /// Identifies the middle mouse button.
    /// </summary>
    mmp_mouse_button_middle = 0x0010,

    /// <summary>
    /// Identifies the X1 button.
    /// </summary>
    mmp_mouse_button_x1= 0x0020,

    /// <summary>
    /// Identifies the X2 button.
    /// </summary>
    mmp_mouse_button_x2 = 0x0040
} mmp_mouse_button;

#endif /* !defined(_MMP_MOUSE_BUTTON_H) */
