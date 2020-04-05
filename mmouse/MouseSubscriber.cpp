// <copyright file="MouseSubscriber.cpp" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#include "pch.h"
#include "MouseSubscriber.h"


/*
 * MouseSubscriber::OnMouseMove
 */
void MouseSubscriber::OnMouseMove(const std::int32_t x,
        const std::int32_t y) {
    ::SetCursorPos(x, y);
}