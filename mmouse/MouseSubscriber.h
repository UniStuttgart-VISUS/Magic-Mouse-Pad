// <copyright file="MouseSubscriber.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once


/// <summary>
/// Subscribes to updates from the magic mouse pad and moves the mouse pointer
/// accordingly.
/// </summary>
class MouseSubscriber : MagicMousePad::MouseSubscriber {

    /// <inheritdoc />
    void OnMouseMove(const std::int32_t x, const std::int32_t y) override;
};
