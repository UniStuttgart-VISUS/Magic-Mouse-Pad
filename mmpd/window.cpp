// <copyright file="window.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"
#include "window.h"

#include "Resource.h"


static const _TCHAR *WindowClass = _T("MagicMousePad");


/*
 * ::CreateMousePadWindow
 */
HWND CreateMousePadWindow(HINSTANCE hInstance) {
    using namespace MagicMousePad;
    assert(hInstance != NULL);

    auto title = ResourceHelper<_TCHAR>::LoadString(IDS_APP_TITLE, hInstance);

    auto exStyle = WS_EX_APPWINDOW;
    auto style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

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
        wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
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
