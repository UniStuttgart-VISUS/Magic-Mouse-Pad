// <copyright file="mmouse.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"

#include "CommandLine.h"
#include "MouseSubscriber.h"
#include "resource.h"
#include "window.h"


#define WM_SYSTRAYICON (WM_USER + 1)


/// <summary>
/// Handles events for the mouse pad window.
/// </summary>
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam,
        LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_MENU_SUBSCRIBE:
                    try {
                        auto data = ::GetWindowLongPtr(hWnd, GWLP_USERDATA);
                        auto subscriber = reinterpret_cast<MouseSubscriber *>(data);
                        subscriber->Announce();
                    } catch (std::exception &ex) {
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

                auto hMenu = ::LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU));
                assert(hMenu != NULL);
                hMenu = ::GetSubMenu(hMenu, 0);

                ::SetForegroundWindow(hWnd);
                ::TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0,
                    hWnd, NULL);
                ::PostMessage(hWnd, WM_NULL, 0, 0);
            }
            return 0;

        default:
            return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }
}


/// <summary>
/// Entry point of the application.
/// </summary>
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPTSTR cmdLine, int cmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);

    try {
        // Register the window class.
        ::RegisterWindowClass(hInstance, ::WndProc);
        auto guardWindowClass = MagicMousePad::OnExit([hInstance]() {
            ::UnregisterWindowClass(hInstance);
            });

        // Create the hidden window processing the events.
        auto hWnd = ::CreateDummyWindow(hInstance);
        auto guardWindow = ::MagicMousePad::OnExit([hWnd]() {
            ::DestroyWindow(hWnd);
            });

        // Add the systray icon.
        NOTIFYICONDATA nid;
        ::ZeroMemory(&nid, sizeof(nid));
        nid.cbSize = sizeof(nid);
        nid.hWnd = hWnd;
        nid.uID = IDI_TRAYICON;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRAYICON));
        nid.uCallbackMessage = WM_SYSTRAYICON;
        ::LoadString(hInstance, IDS_TOOLTIP, nid.szTip,
            static_cast<int>(std::size(nid.szTip)));

        if (!::Shell_NotifyIcon(NIM_ADD, &nid)) {
            throw std::system_error(::GetLastError(), std::system_category(),
                "Failed to add systray icon");
        }
        auto guardTrayIcon = ::MagicMousePad::OnExit([&nid]() {
            ::Shell_NotifyIcon(NIM_DELETE, &nid);
            });

        // Create the subscriber.
        CommandLine c(cmdLine);
        MouseSubscriber subscriber(c);
        auto guardSubscriber = ::MagicMousePad::OnExit(
            [&subscriber]() { subscriber.Stop(); });

        // Attach the subscriber to the dummy window.
        {
            auto data = reinterpret_cast<LONG_PTR>(std::addressof(subscriber));

            if (::SetWindowLongPtr(hWnd, GWLP_USERDATA, data) == 0) {
                auto error = ::GetLastError();
                if (error != NO_ERROR) {
                    throw std::system_error(error, std::system_category(),
                        "Failed to attach application object to window handle");
                }
            }
        }

        // Enter the message loop.
        MSG msg;
        while (::GetMessage(&msg, nullptr, 0, 0)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        return static_cast<int>(msg.wParam);
    } catch (std::exception &ex) {
        ::MessageBoxA(NULL, ex.what(), nullptr, MB_OK | MB_ICONERROR);
        return -1;
    }
}
