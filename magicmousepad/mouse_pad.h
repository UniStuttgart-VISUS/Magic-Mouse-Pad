// <copyright file="mouse_pad.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <memory>
#include <stdexcept>

#include <mmp_key.h>
#include <mmp_mouse_button.h>
#include <WinSock2.h>
#include <Windows.h>
#include <tchar.h>

#include <wil/resource.h>

#include "server.h"
#include "settings.h"


/// <summary>
/// Implements the actual magic mouse pad, which is a window that captures the
/// mouse and resets it like a treadmill such that it can move farther than the
/// actual screen size.
/// </summary>
class mouse_pad final {

public:

    /// <summary>
    /// Creates a new instance of the magic mouse pad.
    /// </summary>
    /// <param name="instance">The instance handle of the application. If this
    /// parameter is <c>NULL</c>, the current module handle will be used.
    /// </param>
    /// <param name="command_line">The command line arguments passed to the
    /// application. If any, this will be interpreted as the path to the JSON
    /// configuration file.</param>
    /// <returns>The magic mouse pad.</returns>
    static std::unique_ptr<mouse_pad> create(_In_ HINSTANCE instance,
        _In_z_ const TCHAR *command_line);

private:

    /// <summary>
    /// The name of the window class we register for the mouse pad.
    /// </summary>
    static constexpr const TCHAR *class_name = _T("MagicMousePad");

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
    mouse_pad(_In_ HINSTANCE instance);

    /// <summary>
    /// Handles a key down event.
    /// </summary>
    /// <param name="key"></param>
    /// <param name="repeat"></param>
    /// <returns></returns>
    LRESULT on_key_down(_In_ const mmp_key key, _In_ const int repeat) noexcept;

    /// <summary>
    /// Handles a key up event.
    /// </summary>
    /// <param name="key"></param>
    /// <param name="repeat"></param>
    /// <returns></returns>
    LRESULT on_key_up(_In_ const mmp_key key, _In_ const int repeat) noexcept;

    /// <summary>
    /// Handles a mouse button being pressed at the specified location.
    /// </summary>
    /// <param name="button"></param>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <returns></returns>
    LRESULT on_mouse_down(_In_ const mmp_mouse_button button,
        _In_ const std::int16_t x,
        _In_ const std::int16_t y) noexcept;

    /// <summary>
    /// Handles the mouse being moved over the mouse pad window.
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <returns></returns>
    LRESULT on_mouse_move(_In_ const std::int16_t x,
        _In_ const std::int16_t y) noexcept;

    /// <summary>
    /// Handles a mouse button being released at the specified location.
    /// </summary>
    /// <param name="button"></param>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <returns></returns>
    LRESULT on_mouse_up(_In_ const mmp_mouse_button button,
        _In_ const std::int16_t x,
        _In_ const std::int16_t y) noexcept;

    /// <summary>
    /// Draws the window contents.
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    LRESULT on_paint(void) noexcept;

    /// <summary>
    /// Handles the window being resized.
    /// </summary>
    /// <param name="width"></param>
    /// <param name="height"></param>
    /// <returns></returns>
    LRESULT on_resize(_In_ const std::uint16_t width,
        _In_ const std::uint16_t height) noexcept;

    bool _active;
    int _dx;
    int _dy;
    int _height;
    HINSTANCE _instance;
    std::unique_ptr<server> _server;
    settings _settings;
    int _width;
    wil::unique_hwnd _window;
};
