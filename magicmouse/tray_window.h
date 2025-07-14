// <copyright file="tray_window.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <memory>

#include <WinSock2.h>
#include <mmpcli.h>
#include <Windows.h>
#include <tchar.h>

#include <wil/resource.h>


/// <summary>
/// The tray window is an invisible dummy window that receives events from the
/// systray icon.
/// </summary>
class tray_window final {

public:

    /// <summary>
    /// Creates a new instance of the tray window and installs the tray icon.
    /// </summary>
    /// <param name="instance"></param>
    /// <returns></returns>
    static std::unique_ptr<tray_window> create(_In_ HINSTANCE instance);

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    ~tray_window(void) noexcept;

private:

    /// <summary>
    /// The name of the window class we register for the mouse pad.
    /// </summary>
    static constexpr const TCHAR *class_name = _T("MagicMouseTray");

    /// <summary>
    /// Registers the window class if not yet registered.
    /// </summary>
    /// <param name="mirror"></param>
    /// <param name="instance"></param>
    static void register_window_class(_In_ HINSTANCE instance);

    /// <summary>
    /// Unregisters the window class.
    /// </summary>
    /// <param name="instance"></param>
    static void unregister_window_class(_In_ HINSTANCE instance);

    /// <summary>
    /// The windows procedure for the mouse pad.
    /// </summary>
    /// <param name="hWnd"></param>
    /// <param name="msg"></param>
    /// <param name="wParam"></param>
    /// <param name="lParam"></param>
    /// <returns></returns>
    static LRESULT CALLBACK wnd_proc(HWND hWnd, UINT msg, WPARAM wParam,
        LPARAM lParam);

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="instance"></param>
    tray_window(_In_ HINSTANCE instance);

    /// <summary>
    /// Adds an icon to the system tray.
    /// </summary>
    /// <param name=""></param>
    void add_icon(void);

    /// <summary>
    /// Handles a mouse button being pressed or released.
    /// </summary>
    /// <param name="button"></param>
    /// <param name="down"></param>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="context"></param>
    /// <returns></returns>
    static void WINAPIV on_mouse_button(
        _In_ const mmp_mouse_button button,
        _In_ const bool down,
        _In_ const int32_t x,
        _In_ const int32_t y,
        _In_opt_ void *context) noexcept;

    /// <summary>
    /// Handles the mouse being moved.
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="context"></param>
    /// <returns></returns>
    static void WINAPIV on_mouse_move(
        _In_ const int32_t x,
        _In_ const int32_t y,
        _In_opt_ void *context) noexcept;

    mmp_configuration _config;
    HINSTANCE _instance;
    visus::mmp::unique_handle _mouse_pad;
    NOTIFYICONDATA _nid;
    wil::unique_hwnd _window;
};
