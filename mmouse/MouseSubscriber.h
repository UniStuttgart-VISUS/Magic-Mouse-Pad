// <copyright file="MouseSubscriber.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "CommandLine.h"


/// <summary>
/// Subscribes to updates from the magic mouse pad and moves the mouse pointer
/// accordingly.
/// </summary>
class MouseSubscriber : MagicMousePad::MouseSubscriber {

public:

    MouseSubscriber(const CommandLine& cmdLine);

    inline void Stop(void) {
        this->Unsubscribe();
    }

protected:

    /// <inheritdoc />
    void OnMouseMove(const std::int32_t x, const std::int32_t y) override;

    /// <inheritdoc />
    void OnMouseVisibilityChanged(const bool isVisible) override;
};
