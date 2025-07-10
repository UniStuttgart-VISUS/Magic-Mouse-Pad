// <copyright file="magicmousepad.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <wil/resource.h>

#include "mouse_pad.h"


/// <summary>
/// Entry point of the magic mouse pad application.
/// </summary>
/// <param name="instance"></param>
/// <param name="prev_instance"></param>
/// <param name="command_line"></param>
/// <param name="show"></param>
/// <returns></returns>
int APIENTRY _tWinMain(_In_ const HINSTANCE instance,
        _In_opt_ const HINSTANCE prev_instance,
        _In_ const LPTSTR command_line,
        _In_ const int show) {
    UNREFERENCED_PARAMETER(prev_instance);
    UNREFERENCED_PARAMETER(command_line);
    UNREFERENCED_PARAMETER(show);

    try {
        MSG msg;
        WSADATA wsa_data;

        {
            DWORD status = ::WSAStartup(MAKEWORD(2, 2), &wsa_data);
            THROW_WIN32_IF(status, status != NO_ERROR);
        }

        auto wsa_cleanup = wil::scope_exit([](void) { ::WSACleanup(); });
        auto mouse_pad = mouse_pad::create(instance);

        while (::GetMessage(&msg, NULL, 0, 0)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        return static_cast<int>(msg.wParam);
    } catch (std::exception& ex) {
        ::OutputDebugStringA(ex.what());
        ::MessageBoxA(NULL, ex.what(), nullptr, MB_OK | MB_ICONERROR);
        return -1;
    }
}
