// <copyright file="window.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once


extern HWND CreateDummyWindow(HINSTANCE hInstance);

extern void RegisterWindowClass(HINSTANCE hInstance, WNDPROC wndProc);

extern void UnregisterWindowClass(HINSTANCE hInstance);
