// <copyright file="State.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once


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

    void OnDraw(void);

    void OnKeyDown(WPARAM key);

    void OnKeyUp(WPARAM key);

    void OnMouseDown(const MagicMousePad::MouseButton button);

    void OnMouseMove(const INT x, const INT y);

    void OnMouseUp(const MagicMousePad::MouseButton button);

    void OnSize(void);

private:

    void CreateRenderTarget(void);

    D2D1_SIZE_U GetSize(void) const;


    ID2D1Factory *_d2d;
    IDWriteFactory *_dwrite;
    WPARAM _escapeKey;
    HWND _hWnd;
    bool _isActive;
    ID2D1HwndRenderTarget *_rt;
    float _scaleX;
    float _scaleY;

};
