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
        ::RegisterWindowClass(hInstance, ::WndProc);
        auto guardWindowClass = MagicMousePad::OnExit([hInstance]() {
            ::UnregisterWindowClass(hInstance);
            });

        auto hWnd = ::CreateDummyWindow(hInstance);
        auto guardWindow = ::MagicMousePad::OnExit([hWnd]() {
            ::DestroyWindow(hWnd);
            });

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

        CommandLine c(cmdLine);
        MouseSubscriber subscriber(c);

        MSG msg;
        while (::GetMessage(&msg, nullptr, 0, 0)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        subscriber.Stop();

        return static_cast<int>(msg.wParam);
    } catch (std::exception &ex) {
        ::MessageBoxA(NULL, ex.what(), nullptr, MB_OK | MB_ICONERROR);
        return -1;
    }
}
