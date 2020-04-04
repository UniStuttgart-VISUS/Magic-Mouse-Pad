// <copyright file="State.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once

#include "Renderer.h"


/// <summary>
/// Container for the application state, which is attached as user data to the
/// window handle of the mouse pad.
/// </summary>
class State {

public:

    static void Attach(State& state, HWND hWnd);

    static State *Retrieve(HWND hWnd);

    State(HWND hWnd);

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
        this->_renderer.Resize(Renderer::GetSize(this->_hWnd));
    }

private:

    inline D2D1_SIZE_U GetSize(void) const {
        return Renderer::GetSize(this->_hWnd);
    }

    WPARAM _escapeKey;
    HWND _hWnd;
    bool _isActive;
    Renderer _renderer;
};
