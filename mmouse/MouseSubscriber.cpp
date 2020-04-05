// <copyright file="MouseSubscriber.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"
#include "MouseSubscriber.h"


/*
 * MouseSubscriber::OnMouseMove
 */
void MouseSubscriber::OnMouseMove(const std::int32_t x,
        const std::int32_t y) {
    ::SetCursorPos(x, y);
}
