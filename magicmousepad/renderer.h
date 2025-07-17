// <copyright file="renderer.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <utility>

#include <d2d1.h>
#include <dwrite.h>

#include <wil/com.h>
#include <wil/resource.h>
#include <wil/result.h>


/// <summary>
/// A Direct2D based renderer for displaying information on the mouse pad.
/// </summary>
class renderer final {

public:

    static D2D1_SIZE_U size(_In_ HWND hWnd);

    explicit renderer(_In_ HWND wnd);

    inline void active(_In_ const bool active) noexcept {
        this->_active = active;
    }

    void draw(void);

    void resize(_In_ const D2D1_SIZE_U& size);

    inline void resize(_In_ const std::uint32_t width,
            _In_ const std::uint32_t height) {
        this->resize(D2D1::SizeU(width, height));
    }

private:

    void create_device_dependent_resources(void);

    void release_device_dependent_resources(void);

    bool _active;
    D2D1_COLOR_F _background_active;
    D2D1_COLOR_F _background_inactive;
    wil::com_ptr<ID2D1Factory> _d2d_factory;
    wil::com_ptr<IDWriteFactory> _dwrite_factory;
    wil::com_ptr<IDWriteTextFormat> _fmt_primary;
    wil::com_ptr<ID2D1HwndRenderTarget> _render_target;
    std::pair<float, float> _scale;
    wil::com_ptr<ID2D1Brush> _text_brush;
};
