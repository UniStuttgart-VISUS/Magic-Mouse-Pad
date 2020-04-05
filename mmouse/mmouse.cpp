// <copyright file="mmouse.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"

#include "CommandLine.h"
#include "MouseSubscriber.h"


/// <summary>
/// Entry point of the application.
/// </summary>
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPTSTR cmdLine, int cmdShow) {
    CommandLine c(cmdLine);
    MouseSubscriber s(c);


    ::Sleep(10000);
    return 0;
}
