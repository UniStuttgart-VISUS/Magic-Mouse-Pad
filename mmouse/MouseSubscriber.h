// <copyright file="MouseSubscriber.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once

#include "CommandLine.h"


/// <summary>
/// Subscribes to updates from the magic mouse pad and moves the mouse pointer
/// accordingly.
/// </summary>
class MouseSubscriber : MagicMousePad::MouseSubscriber {

public:

    MouseSubscriber(const CommandLine& cmdLine);

    void Announce(void);

    inline void Disable(void) {
        this->_isDisabled = true;
    }

    inline void Enable(void) {
        this->_isDisabled = false;
    }

    inline void Stop(void) {
        this->Unsubscribe();
    }

protected:

    /// <inheritdoc />
    void OnMouseDown(const MagicMousePad::MouseButton button) override;

    /// <inheritdoc />
    void OnMouseMove(const std::int32_t x, const std::int32_t y) override;

    /// <inheritdoc />
    void OnMouseUp(const MagicMousePad::MouseButton button) override;

    /// <inheritdoc />
    void OnMouseVisibilityChanged(const bool isVisible) override;

private:

    CommandLine _cmdLine;
    std::atomic<bool> _isDisabled;
};
