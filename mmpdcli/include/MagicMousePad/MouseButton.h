// <copyright file="MouseButton.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once

#include <cinttypes>


namespace MagicMousePad {

    /// <summary>
    /// Defines possible mouse buttons.
    /// </summary>
    enum class MouseButton : std::uint16_t {

        /// <summary>
        /// Represents the left mouse button.
        /// </summary>
        Left = 0x01,

        /// <summary>
        /// Represents the middle mouse button.
        /// </summary>
        Middle = 0x02,

        /// <summary>
        /// Represents the right mouse button.
        /// </summary>
        Right = 0x03
    };

}
