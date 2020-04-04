// <copyright file="State.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>


#include "pch.h"
#include "State.h"


/*
 * State::Attach
 */
void State::Attach(State& state, HWND hWnd) {
    assert(hWnd != NULL);
    auto data = reinterpret_cast<LONG_PTR>(std::addressof(state));

    if (::SetWindowLongPtr(hWnd, GWLP_USERDATA, data) == 0) {
        auto error = ::GetLastError();
        if (error != NO_ERROR) {
            throw std::system_error(error, std::system_category(),
                "Failed to attach application state to window handle");
        }
    }
}


/*
 * State::Retrieve
 */
State *State::Retrieve(HWND hWnd) {
    assert(hWnd != NULL);
    auto data = ::GetWindowLongPtr(hWnd, GWLP_USERDATA);
    return reinterpret_cast<State *>(data);
}


/*
 * State::State
 */
State::State(HWND hWnd) : _d2d(nullptr), _dwrite(nullptr),
        _escapeKey(VK_ESCAPE), _hWnd(hWnd), _isActive(false),
        _rt(nullptr), _scaleX(1.0f), _scaleY(1.0f) {
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

    this->CreateRenderTarget();
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
        UINT dpi = ::GetDpiForWindow(this->_hWnd);
        this->_scaleX = dpi / 96.0f;
        this->_scaleY = dpi / 96.0f;
    }

    // Attach as user data to the window handle.
    State::Attach(*this, hWnd);

    // If all succeeded, cancel all undos.
    undoD2D.Cancel();
    undoRt.Cancel();
    undoDwrite.Cancel();
    //undoBrushActive.Cancel();
    //undoBrushInactive.Cancel();
}


/*
 * State::~State
 */
State::~State(void) {
    assert(this->_d2d != nullptr);
    this->_d2d->Release();
    assert(this->_dwrite != nullptr);
    this->_dwrite->Release();
    assert(this->_rt != nullptr);
    this->_rt->Release();
}


/*
 * State::OnDraw
 */
void State::OnDraw(void) {
    assert(this->_rt != nullptr);

    this->_rt->BeginDraw();

    auto backgroundColour = this->_isActive
        ? D2D1::ColorF(D2D1::ColorF::LightGray)
        : D2D1::ColorF(D2D1::ColorF::DarkGray);
    this->_rt->Clear(backgroundColour);


    this->_rt->EndDraw();
}


/*
 * State::OnKeyDown
 */
void State::OnKeyDown(WPARAM key) {
    if (this->_isActive && (key == this->_escapeKey)) {
        ::OutputDebugString(_T("Mouse released by mouse pad.\r\n"));
        ::ReleaseCapture();
        this->_isActive = false;
    }
}


/*
 * State::OnKeyUp
 */
void State::OnKeyUp(WPARAM key) { }


/*
 * State::OnMouseDown
 */
void State::OnMouseDown(const MagicMousePad::MouseButton button) {
    if (!this->_isActive) {
        ::OutputDebugString(_T("Mouse captured by mouse pad.\r\n"));
        this->_isActive = true;
        ::SetCapture(this->_hWnd);
    }
}


/*
 * State::OnMouseMove
 */
void State::OnMouseMove(const INT x, const INT y) {
    if (this->_isActive) {
        auto isWrap = false;
        POINT pos { x ,y };
        auto size = this->GetSize();

        if (pos.x < 0) {
            ::OutputDebugString(_T("Wrap left.\r\n"));
            pos.x += static_cast<INT>(size.width);
            assert(pos.x < size.width);
            isWrap = true;
        }

        if (pos.x > size.width) {
            ::OutputDebugString(_T("Wrap right.\r\n"));
            pos.x -= static_cast<INT>(size.width);
            isWrap = true;
        }

        if (pos.y < 0) {
            ::OutputDebugString(_T("Wrap top.\r\n"));
            pos.y += static_cast<INT>(size.height);
            isWrap = true;
        }

        if (pos.y > size.height) {
            ::OutputDebugString(_T("Wrap bottom.\r\n"));
            pos.y -= static_cast<INT>(size.height);
            isWrap = true;
        }

        if (isWrap) {
            ::ClientToScreen(this->_hWnd, &pos);
            ::SetCursorPos(pos.x, pos.y);
        }
    }
}


/*
 * State::OnMouseUp
 */
void State::OnMouseUp(const MagicMousePad::MouseButton button) {
}


/*
 * State::OnSize
 */
void State::OnSize(void) {
    assert(this->_rt != nullptr);
    this->_rt->Resize(this->GetSize());
}


/*
 * State::CreateRenderTarget
 */
void State::CreateRenderTarget(void) {
    assert(this->_rt == nullptr);

    auto size = this->GetSize();

    auto hr = this->_d2d->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(this->_hWnd, size),
        &this->_rt);
    if (FAILED(hr)) {
        throw std::system_error(hr, std::system_category(),
            "Failed to create Direct2D render target");
    }
}


/*
 * State::GetSize
 */
D2D1_SIZE_U State::GetSize(void) const {
    assert(this->_hWnd != NULL);

    RECT wndRect;
    if (!::GetClientRect(this->_hWnd, &wndRect)) {
        throw std::system_error(::GetLastError(), std::system_category(),
            "Failed to retrieve window client area");
    }

    return D2D1::SizeU(wndRect.right - wndRect.left,
        wndRect.bottom - wndRect.top);
}
