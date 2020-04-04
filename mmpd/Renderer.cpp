// <copyright file="Renderer.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"
#include "Renderer.h"


/*
 * Renderer::GetSize
 */
D2D1_SIZE_U Renderer::GetSize(HWND hWnd) {
    assert(hWnd != NULL);

    RECT wndRect;
    if (!::GetClientRect(hWnd, &wndRect)) {
        throw std::system_error(::GetLastError(), std::system_category(),
            "Failed to retrieve window client area");
    }

    return D2D1::SizeU(wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top);
}


/*
 * Renderer::Renderer
 */
Renderer::Renderer(HWND hWnd)
        : _backgroundColour(D2D1::ColorF(D2D1::ColorF::White)),  _d2d(nullptr),
        _dwrite(nullptr), _rt(nullptr), _scaleX(1.0f), _scaleY(1.0f) {
    using namespace MagicMousePad;
    {
        auto hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &this->_d2d);
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category(),
                "Failed to create Direct2D factory");
        }
    }
    auto undoD2D = OnExit([this]() { this->_d2d->Release(); });

    {
        auto hr = this->_d2d->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hWnd, Renderer::GetSize(hWnd)),
            &this->_rt);
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category(),
                "Failed to create Direct2D render target");
        }
    }
    auto undoRt = OnExit([this]() { this->_rt->Release(); });

    {
        auto hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown **>(&this->_dwrite));
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category(),
                "Failed to create DirectWrite factory");
        }
    }
    auto undoDwrite = OnExit([this]() { this->_dwrite->Release(); });

    //{
    //    auto hr = this->_rt->CreateSolidColorBrush(
    //        D2D1::ColorF(D2D1::ColorF::LightGray),
    //        &this->_brushActive);
    //}
    //auto undoBrushActive = OnExit([this]() { this->_brushActive->Release(); });

    //{
    //    auto hr = this->_rt->CreateSolidColorBrush(
    //        D2D1::ColorF(D2D1::ColorF::DarkGray),
    //        &this->_brushInactive);
    //}
    //auto undoBrushInactive = OnExit([this]() { this->_brushInactive->Release(); });

    // Compute scaling of mouse coordinates to DIPs.
    {
        UINT dpi = ::GetDpiForWindow(hWnd);
        this->_scaleX = dpi / 96.0f;
        this->_scaleY = dpi / 96.0f;
    }

    // If all succeeded, cancel all undos.
    undoD2D.Cancel();
    undoRt.Cancel();
    undoDwrite.Cancel();
    //undoBrushActive.Cancel();
    //undoBrushInactive.Cancel();
}


/*
 * Renderer::~Renderer
 */
Renderer::~Renderer(void) {
    assert(this->_d2d != nullptr);
    this->_d2d->Release();
    assert(this->_dwrite != nullptr);
    this->_dwrite->Release();
    assert(this->_rt != nullptr);
    this->_rt->Release();
}


/*
 * Renderer::Draw
 */
void Renderer::Draw(void) {
    assert(this->_rt != nullptr);

    this->_rt->BeginDraw();
    this->_rt->Clear(this->_backgroundColour);

    this->_rt->EndDraw();
}
