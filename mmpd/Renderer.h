// <copyright file="Renderer.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once


/// <summary>
/// The Direct2D renderer for the overlay graphics and texts.
/// </summary>
class Renderer {

public:

    static D2D1_SIZE_U GetSize(HWND hWnd);

    Renderer(HWND hWnd);

    ~Renderer(void);

    void Draw(void);

    inline void Resize(HWND hWnd) {
        assert(this->_rt != nullptr);
        this->_rt->Resize(GetSize(hWnd));
    }

    inline void SetBackgroundColour(const D2D1::ColorF::Enum colour) {
        this->SetBackgroundColour(D2D1::ColorF(colour));
    }

    inline void SetBackgroundColour(const D2D1::ColorF &colour) {
        this->_backgroundColour = colour;
    }

private:

    D2D1::ColorF _backgroundColour;
    ID2D1Factory *_d2d;
    IDWriteFactory *_dwrite;
    ID2D1HwndRenderTarget *_rt;
    float _scaleX;
    float _scaleY;

};
