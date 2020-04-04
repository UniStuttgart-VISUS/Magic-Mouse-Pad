// <copyright file="mmpd.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"

#include "window.h"
#include "State.h"


/*
 * ::WndProc
 */
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam,
        LPARAM lParam) {

    switch (msg) {
        case WM_CLOSE:
            ::PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            State::Retrieve(hWnd)->OnKeyDown(wParam);
            return 0;

        case WM_KEYUP:
            State::Retrieve(hWnd)->OnKeyUp(wParam);
            return 0;

        case WM_LBUTTONDOWN:
            State::Retrieve(hWnd)->OnMouseDown(
                MagicMousePad::MouseButton::Left);
            return 0;

        case WM_LBUTTONUP:
            State::Retrieve(hWnd)->OnMouseUp(
                MagicMousePad::MouseButton::Left);
            return 0;

        case WM_MBUTTONDOWN:
            State::Retrieve(hWnd)->OnMouseDown(
                MagicMousePad::MouseButton::Middle);
            return 0;

        case WM_MBUTTONUP:
            State::Retrieve(hWnd)->OnMouseUp(
                MagicMousePad::MouseButton::Middle);
            return 0;

        case WM_MOUSEMOVE:
            State::Retrieve(hWnd)->OnMouseMove(GET_X_LPARAM(lParam),
                GET_Y_LPARAM(lParam));
            return 0;

        case WM_RBUTTONDOWN:
            State::Retrieve(hWnd)->OnMouseDown(
                MagicMousePad::MouseButton::Right);
            return 0;

        case WM_RBUTTONUP:
            State::Retrieve(hWnd)->OnMouseUp(
                MagicMousePad::MouseButton::Right);
            return 0;

        case WM_PAINT:
            State::Retrieve(hWnd)->OnDraw();
            return 0;

        case WM_SIZE:
            State::Retrieve(hWnd)->OnSize();
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

        auto hWnd = ::CreateMousePadWindow(hInstance);
        auto guardWindow = ::MagicMousePad::OnExit([hWnd]() {
            ::DestroyWindow(hWnd);
            });

        State state(hWnd);

        ::ShowWindow(hWnd, cmdShow);

        MSG msg;
        while (::GetMessage(&msg, nullptr, 0, 0)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        return static_cast<int>(msg.wParam);
    } catch (std::exception & ex) {
        ::MessageBoxA(NULL, ex.what(), nullptr, MB_OK | MB_ICONERROR);
        return -1;
    }

#if 0
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MMPD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MMPD));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
#endif
    return 9;
}

#if 0

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MMPD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MMPD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
#endif
