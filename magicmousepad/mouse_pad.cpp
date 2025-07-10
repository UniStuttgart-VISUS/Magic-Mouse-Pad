// <copyright file="mouse_pad.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "mouse_pad.h"

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <cstdlib>
#include <fstream>

#include <wil/result.h>

#include "resource.h"


/*
 * mouse_pad::create
 */
std::unique_ptr<mouse_pad> mouse_pad::create(_In_ HINSTANCE instance,
        _In_z_ const TCHAR *command_line) {
    register_window_class(instance);

    auto exStyle = WS_EX_APPWINDOW;
    auto style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    auto x = CW_USEDEFAULT;
    auto y = CW_USEDEFAULT;
    auto width = CW_USEDEFAULT;
    auto height = CW_USEDEFAULT;

    TCHAR *buf_title = nullptr;
    auto cnt_title = ::LoadString(instance,
        IDS_APPTITLE,
        reinterpret_cast<TCHAR *>(&buf_title),
        0);
    THROW_LAST_ERROR_IF(cnt_title <= 0);
    std::basic_string<TCHAR> title(buf_title, cnt_title);

    auto retval = std::unique_ptr<mouse_pad>(new mouse_pad(instance));
    retval->_window.reset(::CreateWindowEx(exStyle,
        class_name,
        title.c_str(),
        style,
        x,
        y,
        width,
        height,
        nullptr,
        nullptr,
        instance,
        retval.get()));
    THROW_LAST_ERROR_IF(!retval->_window);

    // Parse the configuration file.
    std::ifstream stream;
    if ((command_line != nullptr) && (*command_line != 0)) {
        stream.open(command_line);
    } else {
        stream.open("appsettings.json");
    }

    nlohmann::json json;
    stream >> json;
    retval->_settings = json.get<settings>();

    // Start the server.
    retval->_server = std::make_unique<server>(retval->_settings);

    return retval;
}


/*
 * mouse_pad::register_window_class
 */
void mouse_pad::register_window_class(_In_ HINSTANCE instance) {
    assert(instance != NULL);
    WNDCLASSEX wnd_class;

    if (!::GetClassInfoEx(instance, class_name, &wnd_class)) {
        ::ZeroMemory(&wnd_class, sizeof(wnd_class));

        wnd_class.cbSize = sizeof(wnd_class);
        wnd_class.style = CS_CLASSDC;
        wnd_class.lpfnWndProc = mouse_pad::wnd_proc;
        wnd_class.hCursor = ::LoadCursor(instance, IDC_ARROW);
        wnd_class.hInstance = instance;
        wnd_class.lpszClassName = class_name;
        THROW_LAST_ERROR_IF(!::RegisterClassExW(&wnd_class));
    }
}


/*
 * mouse_pad::unregister_window_class
 */
void mouse_pad::unregister_window_class(_In_ HINSTANCE instance) {
    assert(instance != NULL);
    ::UnregisterClass(class_name, instance);
}


/*
 * mouse_pad::wnd_proc
 */
LRESULT mouse_pad::wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto that = reinterpret_cast<mouse_pad *>(::GetWindowLongPtrW(hWnd,
        GWLP_USERDATA));

    switch (msg) {
        case WM_CREATE: {
            auto cs = reinterpret_cast<CREATESTRUCTW *>(lParam);
            that = reinterpret_cast<mouse_pad *>(cs->lpCreateParams);
            assert(that != nullptr);
            auto ud = reinterpret_cast<LONG_PTR>(that);
            if ((::SetWindowLongPtrW(hWnd, GWLP_USERDATA, ud) == 0)) {
                return ::GetLastError();
            }
        } break;

        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;

        case WM_SIZE:
            assert(that != nullptr);
            return that->on_resize(LOWORD(lParam), HIWORD(lParam));

        case WM_PAINT:
            assert(that != nullptr);
            return that->on_paint();

        case WM_MOUSEMOVE:
            assert(that != nullptr);
            return that->on_mouse_move(LOWORD(lParam), HIWORD(lParam));

        #pragma region Mouse down
        case WM_LBUTTONDOWN:
            assert(that != nullptr);
            return that->on_mouse_down(mmp_mouse_button_left,
                LOWORD(lParam), HIWORD(lParam));

        case WM_MBUTTONDOWN:
            assert(that != nullptr);
            return that->on_mouse_down(mmp_mouse_button_middle,
                LOWORD(lParam), HIWORD(lParam));

        case WM_RBUTTONDOWN:
            assert(that != nullptr);
            return that->on_mouse_down(mmp_mouse_button_right,
                LOWORD(lParam), HIWORD(lParam));

        case WM_XBUTTONDOWN:
            assert(that != nullptr);
            switch (HIWORD(wParam)) {
                case XBUTTON1:
                    return that->on_mouse_down(mmp_mouse_button_x1,
                        LOWORD(lParam), HIWORD(lParam));
                case XBUTTON2:
                    return that->on_mouse_down(mmp_mouse_button_x2,
                        LOWORD(lParam), HIWORD(lParam));
            } break;
        #pragma endregion

        #pragma region Mouse up
        case WM_LBUTTONUP:
            assert(that != nullptr);
            return that->on_mouse_up(mmp_mouse_button_left,
                LOWORD(lParam), HIWORD(lParam));

        case WM_MBUTTONUP:
            assert(that != nullptr);
            return that->on_mouse_up(mmp_mouse_button_middle,
                LOWORD(lParam), HIWORD(lParam));

        case WM_RBUTTONUP:
            assert(that != nullptr);
            return that->on_mouse_up(mmp_mouse_button_right,
                LOWORD(lParam), HIWORD(lParam));

        case WM_XBUTTONUP:
            assert(that != nullptr);
            switch (HIWORD(wParam)) {
                case XBUTTON1:
                    return that->on_mouse_up(mmp_mouse_button_x1,
                        LOWORD(lParam), HIWORD(lParam));
                case XBUTTON2:
                    return that->on_mouse_up(mmp_mouse_button_x2,
                        LOWORD(lParam), HIWORD(lParam));
            } break;
        #pragma endregion

        #pragma region Keyboard
        case WM_KEYDOWN:
            assert(that != nullptr);
            return that->on_key_down(
                static_cast<mmp_key>(wParam),
                static_cast<int>(lParam & 0xFFFF));

        case WM_KEYUP:
            assert(that != nullptr);
            return that->on_key_up(
                static_cast<mmp_key>(wParam),
                static_cast<int>(lParam & 0xFFFF));
            break;
        #pragma endregion
    }

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}


/*
 * mouse_pad::mouse_pad
 */
mouse_pad::mouse_pad(_In_ HINSTANCE instance)
        : _active(false),
        _dx(0),
        _dy(0),
        _height(0),
        _instance(instance),
        _width(0) {
    if (this->_instance == NULL) {
        this->_instance = ::GetModuleHandle(nullptr);
    }
}


/*
 * mouse_pad::on_key_down
 */
LRESULT mouse_pad::on_key_down(_In_ const mmp_key key,
        _In_ const int repeat) noexcept {
    if (this->_active) {
        if (key == mmp_key_pause) {
            ::ReleaseCapture();
            this->_active = false;
        }
    }

    return 0;
}


/*
 * mouse_pad::on_key_up
 */
LRESULT mouse_pad::on_key_up(_In_ const mmp_key key,
        _In_ const int repeat) noexcept {
    // TODO
    return 0;
}


/*
 * mouse_pad::on_mouse_down
 */
LRESULT mouse_pad::on_mouse_down(
        _In_ const mmp_mouse_button button,
        _In_ const std::int16_t x,
        _In_ const std::int16_t y) noexcept {
    if (!this->_active && ((this->_width == 0) || (this->_height == 0))) {
        RECT rect;
        ::GetClientRect(this->_window.get(), &rect);
        this->_width = std::abs(rect.right - rect.left);
        this->_height = std::abs(rect.bottom - rect.top);
    }

    this->_active = true;
    ::SetCapture(this->_window.get());

    // TODO

    return 0;
}


/*
 * mouse_pad::on_mouse_move
 */
LRESULT mouse_pad::on_mouse_move(_In_ const std::int16_t x,
        _In_ const std::int16_t y) noexcept {
    POINT pos { x, y };

    if (x < 0) {
        pos.x = this->_width - 1;
        this->_dx -= this->_width;

    } else if (x >= this->_width) {
        pos.x = 0;
        this->_dx += this->_width;
    }

    if (y < 0) {
        pos.y = this->_height - 1;
        this->_dy -= this->_height;

    } else if (y >= this->_height) {
        pos.y = 0;
        this->_dy += this->_height;
    }

    if ((pos.x != x) || (pos.y != y)) {
        ::ClientToScreen(this->_window.get(), &pos);
        ::SetCursorPos(pos.x, pos.y);
    }

    // TODO:

    return 0;
}


/*
 * mouse_pad::on_mouse_up
 */
LRESULT mouse_pad::on_mouse_up(_In_ const mmp_mouse_button button,
        _In_ const std::int16_t x,
        _In_ const std::int16_t y) noexcept {
    if (this->_active) {
        // TODO: report
    }

    return 0;
}


/*
 * mouse_pad::on_paint
 */
LRESULT mouse_pad::on_paint(void) noexcept {
    PAINTSTRUCT ps;
    auto hdc = ::BeginPaint(this->_window.get(), &ps);

    ::EndPaint(this->_window.get(), &ps);
    return 0;
}


/*
 * mouse_pad::on_resize
 */
LRESULT mouse_pad::on_resize(_In_ const std::uint16_t width,
        _In_ const std::uint16_t height) noexcept {
    this->_width = width;
    this->_height = height;
    return 0;
}
