// <copyright file="dumppos.c" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <mmpcli.h>
#include <sal.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <WinSock2.h>
#include <Windows.h>


/// <summary>
/// This function is called when we receive a position update from the magic
/// mouse pad.
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="context"></param>
static void on_mouse_move(_In_ const int32_t x,
        _In_ const int32_t y,
        _In_opt_ void *context) {
    _tprintf(_T("Mouse moved to: %d, %d\n"), x, y);
}


/// <summary>
/// Entry point of the application.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int _tmain(_In_ const int argc, _In_reads_(argc) const TCHAR **argv) {
    mmp_configuration config;
    mmp_handle mouse_pad = NULL;
    int retval = 0;
    WSADATA wsa_data;

    retval = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (retval != 0) {
        fprintf(stderr, "Failed to initialise Winsock: %d\n", retval);
        return retval;
    }

    memset(&config, 0, sizeof(config));
    config.on_mouse_move = &on_mouse_move;

    if (argc > 0) {
        retval = mmp_configure_server(&config, argv[1]);
        if (retval != 0) {
            fprintf(stderr, "Failed to configure server: %d\n", retval);
            return retval;
        }
    }

    retval = mmp_connect(&mouse_pad, &config);
    if (retval != 0) {
        fprintf(stderr, "Failed to connect to mouse pad: %d\n", retval);
        return retval;
    }

    Sleep(10000);

    mmp_disconnect(mouse_pad);

    return retval;
}
