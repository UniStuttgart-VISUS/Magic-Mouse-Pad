// <copyright file="window.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"
#include "window.h"

#include "resource.h"


static const _TCHAR *WindowClass = _T("MagicMouse");


/*
 * ::CreateDummyWindow
 */
HWND CreateDummyWindow(HINSTANCE hInstance) {
    using namespace MagicMousePad;
    assert(hInstance != NULL);

    auto title = ResourceHelper<_TCHAR>::LoadString(IDS_APP_TITLE, hInstance);

    auto exStyle = 0;
    auto style = WS_POPUP;

    auto retval = ::CreateWindowEx(exStyle, ::WindowClass, title.c_str(),
        style, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
        nullptr, nullptr, hInstance, nullptr);
    if (retval == NULL) {
        throw std::system_error(::GetLastError(), std::system_category(),
            "Failed to create window");
    }

    return retval;
}


/*
 * ::RegisterWindowClass
 */
void RegisterWindowClass(HINSTANCE hInstance, WNDPROC wndProc) {
    WNDCLASSEX wndClass;

    if (!::GetClassInfoEx(hInstance, ::WindowClass, &wndClass)) {
        ::ZeroMemory(&wndClass, sizeof(wndClass));

        wndClass.cbSize = sizeof(wndClass);
        wndClass.style = CS_CLASSDC;
        wndClass.lpfnWndProc = wndProc;
        wndClass.hInstance = hInstance;
        wndClass.lpszClassName = ::WindowClass;

        if (!::RegisterClassEx(&wndClass)) {
            throw std::system_error(::GetLastError(),
                std::system_category(),
                "Failed to register window class");
        }
    }
}


/*
 * ::UnregisterWindowClass
 */
void UnregisterWindowClass(HINSTANCE hInstance) {
    ::UnregisterClass(::WindowClass, hInstance);
}
