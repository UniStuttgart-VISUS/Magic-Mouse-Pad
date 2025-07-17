// <copyright file="renderer.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "renderer.h"

#include "resource.h"


/*
 * renderer::size
 */
D2D1_SIZE_U renderer::size(_In_ HWND hWnd) {
    THROW_WIN32_IF(ERROR_INVALID_PARAMETER, hWnd == NULL);

    RECT rect;
    THROW_LAST_ERROR_IF(!::GetClientRect(hWnd, &rect));

    return D2D1::SizeU(std::abs(rect.right - rect.left),
        std::abs(rect.bottom - rect.top));
}


/*
 * renderer::renderer
 */
renderer::renderer(_In_ HWND wnd)
        : _active(false),
        _background_active(D2D1::ColorF(D2D1::ColorF::LightSlateGray)),
        _background_inactive(D2D1::ColorF(D2D1::ColorF::SlateGray)) {
    wil::com_ptr<ID2D1Factory> d2d;
    wil::com_ptr<IDWriteFactory> dwrite;
    wil::com_ptr<IDWriteFontCollection> fonts;
    wil::com_ptr<IDWriteTextFormat> fmt_primary;
    wil::com_ptr<ID2D1HwndRenderTarget> rt;
    wil::com_ptr<ID2D1SolidColorBrush> text_brush;

    // Create the factories and the render target.
    THROW_IF_FAILED(::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
        d2d.put()));
    THROW_IF_FAILED(d2d->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(wnd, size(wnd)),
        rt.put()));
    THROW_IF_FAILED(::DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown **>(dwrite.put())));

    // Compute scaling of mouse coordinates to DIPs.
    {
        UINT dpi = ::GetDpiForWindow(wnd);
        this->_scale.first = dpi / 96.0f;
        this->_scale.second = dpi / 96.0f;
    }

    // Prepare the text rendering stuff.
    THROW_IF_FAILED(dwrite->GetSystemFontCollection(fonts.put(), false));
    THROW_IF_FAILED(dwrite->CreateTextFormat(
        L"Segoe UI",
        fonts.get(),
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        20.0f,
        L"de-de",
        fmt_primary.put()));
    fmt_primary->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    fmt_primary->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    THROW_IF_FAILED(rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White),
        text_brush.put()));

    // If everything succeeded, persist the stuff in the object.
    this->_d2d_factory = std::move(d2d);
    this->_dwrite_factory = std::move(dwrite);
    this->_fmt_primary = std::move(fmt_primary);
    this->_render_target = std::move(rt);
    this->_text_brush = std::move(text_brush);
}


/*
 * renderer::draw
 */
void renderer::draw(void) {
    this->_render_target->BeginDraw();

    if (this->_active) {
        wchar_t *msg;
        const auto len = ::LoadStringW(NULL, IDS_ESCAPE_PAUSE,
            reinterpret_cast<LPWSTR>(&msg), 0);

        this->_render_target->Clear(this->_background_active);

        if (len > 0) {
            auto size = this->_render_target->GetSize();
            auto layout_rect = D2D1::RectF(0.0f, 0.0f,
                size.width, size.height);
            wil::com_ptr<IDWriteTextLayout> layout;

            THROW_IF_FAILED(this->_dwrite_factory->CreateTextLayout(
                msg, len,
                this->_fmt_primary.get(),
                size.width,
                size.height,
                layout.put()));

            this->_render_target->DrawTextLayout(D2D1::Point2F(), layout.get(),
                this->_text_brush.get());
        }

    } else {
        this->_render_target->Clear(this->_background_inactive);
    }

    this->_render_target->EndDraw();
}


/*
 * renderer::resize
 */
void renderer::resize(_In_ const D2D1_SIZE_U& size) {
    THROW_IF_FAILED(this->_render_target->Resize(size));
}


/*
 * renderer::create_device_dependent_resources
 */
void renderer::create_device_dependent_resources(void) {}


/*
 * renderer::release_device_dependent_resources
 */
void renderer::release_device_dependent_resources(void) {}
