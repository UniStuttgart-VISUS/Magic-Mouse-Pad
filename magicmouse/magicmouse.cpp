// <copyright file="magicmouse.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <mmpcli.h>
#include <tchar.h>
#include <Windows.h>

#include <wil/result.h>


/// <summary>
/// Entry point of the magic mouse application that receives position updates
/// from a magic mouse pad and moves the mouse cursor accordingly.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int _tmain(_In_ const int argc, _In_reads_(argc) TCHAR *argv) {
    try {
        // Configure the mouse pad.
        mmp_configuration config;
        ::mmp_configure_discovery(&config, 14863, 0, 500);

        // Connect to the mouse pad.
        auto handle = visus::mmp::connect(config);

        //::mmp_connect(han)

        return 0;
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
}
