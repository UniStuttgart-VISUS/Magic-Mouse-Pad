// <copyright file="State.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "CommandLine.h"
#include "Renderer.h"
#include "Server.h"


/// <summary>
/// Container for the application state, which is attached as user data to the
/// window handle of the mouse pad.
/// </summary>
class State {

public:

    static void Attach(State& state, HWND hWnd);

    static State *Retrieve(HWND hWnd);

    State(HWND hWnd, const CommandLine& cmdLine);

    ~State(void);

    inline void OnDraw(void) {
        this->_renderer.Draw();
    }

    void OnKeyDown(WPARAM key);

    void OnKeyUp(WPARAM key);

    void OnMouseDown(const MagicMousePad::MouseButton button);

    void OnMouseMove(const INT x, const INT y);

    void OnMouseUp(const MagicMousePad::MouseButton button);

    inline void OnSize(void) {
        this->_renderer.Resize(this->_hWnd);
    }

private:

    WPARAM _escapeKey;
    HWND _hWnd;
    bool _isActive;
    Renderer _renderer;
    Server _server;
};
