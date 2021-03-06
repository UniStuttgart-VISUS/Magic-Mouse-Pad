// <copyright file="window.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once


extern HWND CreateMousePadWindow(HINSTANCE hInstance);

extern void RegisterWindowClass(HINSTANCE hInstance, WNDPROC wndProc);

extern void UnregisterWindowClass(HINSTANCE hInstance);
