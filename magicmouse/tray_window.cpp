// <copyright file="tray_window.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "tray_window.h"

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <cstdlib>
#include <fstream>

#include <mmpmsg.h>
#include <shellapi.h>

#include <wil/registry.h>
#include <wil/result.h>

#include "resource.h"


#define WM_SYSTRAYICON (WM_USER + 1)


/*
 * tray_window::create
 */
std::unique_ptr<tray_window> tray_window::create(_In_ HINSTANCE instance) {
    register_window_class(instance);

    auto exStyle = 0;
    auto style = WS_POPUP;
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

    auto retval = std::unique_ptr<tray_window>(new tray_window(instance));
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

    // Add the icon to the systray.
    retval->add_icon();

    // Load the configuration.
    wil::unique_hkey key;
    if (SUCCEEDED(wil::reg::create_unique_key_nothrow(HKEY_LOCAL_MACHINE,
            L"SOFTWARE\\VISUS\\Magic Mouse", key))) {
        THROW_IF_WIN32_ERROR(::mmp_configure_from_registry_key(
            &retval->_config, key.get()));
    }

    retval->_config.on_mouse_button = &tray_window::on_mouse_button;
    retval->_config.on_mouse_move = &tray_window::on_mouse_move;

    return retval;
}


/*
 * tray_window::~tray_window
 */
tray_window::~tray_window(void) noexcept {
    ::Shell_NotifyIcon(NIM_DELETE, &this->_nid);
    this->_window.reset();
    unregister_window_class(this->_instance);
}


/*
 * tray_window::register_window_class
 */
void tray_window::register_window_class(_In_ HINSTANCE instance) {
    assert(instance != NULL);
    WNDCLASSEX wnd_class;

    if (!::GetClassInfoEx(instance, class_name, &wnd_class)) {
        ::ZeroMemory(&wnd_class, sizeof(wnd_class));

        wnd_class.cbSize = sizeof(wnd_class);
        wnd_class.style = CS_CLASSDC;
        wnd_class.lpfnWndProc = tray_window::wnd_proc;
        wnd_class.hCursor = ::LoadCursor(instance, IDC_ARROW);
        wnd_class.hInstance = instance;
        wnd_class.lpszClassName = class_name;
        THROW_LAST_ERROR_IF(!::RegisterClassExW(&wnd_class));
    }
}


/*
 * tray_window::unregister_window_class
 */
void tray_window::unregister_window_class(_In_ HINSTANCE instance) {
    assert(instance != NULL);
    ::UnregisterClass(class_name, instance);
}


/*
 * tray_window::wnd_proc
 */
LRESULT tray_window::wnd_proc(HWND hWnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam) {
    auto that = reinterpret_cast<tray_window *>(::GetWindowLongPtrW(hWnd,
        GWLP_USERDATA));

    switch (msg) {
        case WM_CREATE: {
            auto cs = reinterpret_cast<CREATESTRUCTW *>(lParam);
            that = reinterpret_cast<tray_window *>(cs->lpCreateParams);
            assert(that != nullptr);
            auto ud = reinterpret_cast<LONG_PTR>(that);
            if ((::SetWindowLongPtrW(hWnd, GWLP_USERDATA, ud) == 0)) {
                return ::GetLastError();
            }
        } break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_MENU_SUBSCRIBE:
                    try {
                        that->_mouse_pad = visus::mmp::connect(that->_config);
                    } catch (std::exception& ex) {
                        ::MessageBoxA(NULL, ex.what(), nullptr,
                            MB_OK | MB_ICONERROR);
                    }
                    break;

                case ID_MENU_EXIT:
                    ::PostQuitMessage(0);
                    break;

                default:
                    break;
            }
            return 0;

        case WM_SYSTRAYICON:
            if (LOWORD(lParam) == WM_RBUTTONDOWN) {
                POINT point;
                ::GetCursorPos(&point);

                auto menu = ::LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU));
                THROW_LAST_ERROR_IF(menu == NULL);
                menu = ::GetSubMenu(menu, 0);
                assert(menu != NULL);

                ::SetForegroundWindow(hWnd);
                ::TrackPopupMenu(menu, TPM_RIGHTBUTTON, point.x, point.y, 0,
                    hWnd, NULL);
                ::PostMessage(hWnd, WM_NULL, 0, 0);
            }
            return 0;
    }

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}


/*
 * tray_window::tray_window
 */
tray_window::tray_window(_In_ HINSTANCE instance)
        : _instance(instance), _nid({ 0 }) {
    if (this->_instance == NULL) {
        this->_instance = ::GetModuleHandle(nullptr);
    }
}


/*
 * tray_window::add_icon
 */
void tray_window::add_icon(void) {
    assert(this->_window);
    this->_nid.cbSize = sizeof(this->_nid);
    this->_nid.hWnd = this->_window.get();
    this->_nid.uID = IDI_MAGICMOUSE;
    this->_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    this->_nid.hIcon = ::LoadIcon(this->_instance,
        MAKEINTRESOURCE(IDI_MAGICMOUSE));
    this->_nid.uCallbackMessage = WM_SYSTRAYICON;

    THROW_LAST_ERROR_IF(::LoadString(this->_instance,
        IDS_TOOLTIP,
        this->_nid.szTip,
        static_cast<int>(std::size(this->_nid.szTip)))
        <= 0);

    THROW_LAST_ERROR_IF(!::Shell_NotifyIcon(NIM_ADD, &this->_nid));
}


/*
 * tray_window::on_mouse_button
 */
void tray_window::on_mouse_button(_In_ const mmp_mouse_button button,
        _In_ const bool down,
        _In_ const int32_t x,
        _In_ const int32_t y,
        _In_opt_ void *context) noexcept {
    INPUT input { 0 };
    input.type = INPUT_MOUSE;

    switch (button) {
        case mmp_mouse_button_left:
            input.mi.dwFlags = down
                ? MOUSEEVENTF_LEFTDOWN
                : MOUSEEVENTF_LEFTUP;
            break;

        case mmp_mouse_button_middle:
            input.mi.dwFlags = down
                ? MOUSEEVENTF_MIDDLEDOWN
                : MOUSEEVENTF_MIDDLEUP;
            break;

        case mmp_mouse_button_right:
            input.mi.dwFlags = down
                ? MOUSEEVENTF_RIGHTDOWN
                : MOUSEEVENTF_RIGHTUP;
            break;

        // TODO: Xdown

        default:
            return;
    }

    ::SendInput(1, &input, sizeof(input));
}

#include <sstream>

/*
 * tray_window::on_mouse_move
 */
void tray_window::on_mouse_move(
        _In_ const int32_t x,
        _In_ const int32_t y,
        _In_opt_ void *context) noexcept {
    std::stringstream ss;
    ss << "Mouse moved to: " << x << ", " << y << std::endl;
    auto s = ss.str();
    ::OutputDebugStringA(s.c_str());
    //::SetCursorPos(x, y);
}
