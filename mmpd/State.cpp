// <copyright file="State.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>


#include "pch.h"
#include "State.h"


/*
 * State::Attach
 */
void State::Attach(State& state, HWND hWnd) {
    assert(hWnd != NULL);
    auto data = reinterpret_cast<LONG_PTR>(std::addressof(state));

    if (::SetWindowLongPtr(hWnd, GWLP_USERDATA, data) == 0) {
        auto error = ::GetLastError();
        if (error != NO_ERROR) {
            throw std::system_error(error, std::system_category(),
                "Failed to attach application state to window handle");
        }
    }
}


/*
 * State::Retrieve
 */
State *State::Retrieve(HWND hWnd) {
    assert(hWnd != NULL);
    auto data = ::GetWindowLongPtr(hWnd, GWLP_USERDATA);
    return reinterpret_cast<State *>(data);
}


/*
 * State::State
 */
State::State(HWND hWnd, const CommandLine &cmdLine) : _escapeKey(VK_ESCAPE),
        _hWnd(hWnd), _isActive(false), _renderer(hWnd) {
    this->_renderer.SetBackgroundColour(D2D1::ColorF::DarkGray);
    this->_server.Start(cmdLine.GetPort(), cmdLine.GetAddressFamily());
    State::Attach(*this, hWnd);

    {
        sockaddr_storage server;
        auto hack = reinterpret_cast<sockaddr_in *>(&server);
        hack->sin_family = AF_INET;
        hack->sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
        hack->sin_addr.S_un.S_un_b.s_b1 = 192;
        hack->sin_addr.S_un.S_un_b.s_b2 = 168;
        hack->sin_addr.S_un.S_un_b.s_b3 = 1;
        hack->sin_addr.S_un.S_un_b.s_b4 = 184;
        hack->sin_port = htons(14263);
        this->_server.AddClient(server);
    }
}


/*
 * State::~State
 */
State::~State(void) { }


/*
 * State::OnKeyDown
 */
void State::OnKeyDown(WPARAM key) {
    if (this->_isActive && (key == this->_escapeKey)) {
        ::OutputDebugString(_T("Mouse released by mouse pad.\r\n"));
        ::ReleaseCapture();
        this->_isActive = false;
        this->_renderer.SetBackgroundColour(D2D1::ColorF::DarkGray);
    }
}


/*
 * State::OnKeyUp
 */
void State::OnKeyUp(WPARAM key) { }


/*
 * State::OnMouseDown
 */
void State::OnMouseDown(const MagicMousePad::MouseButton button) {
    if (!this->_isActive) {
        ::OutputDebugString(_T("Mouse captured by mouse pad.\r\n"));
        this->_isActive = true;
        this->_renderer.SetBackgroundColour(D2D1::ColorF::LightGray);
        ::SetCapture(this->_hWnd);
    }

    if (this->_isActive) {
        MagicMousePad::MouseUpMessage msg;
        msg.Button = static_cast<decltype(msg.Button)>(button);
        this->_server.Send(msg);
    }
}


/*
 * State::OnMouseMove
 */
void State::OnMouseMove(const INT x, const INT y) {
    if (this->_isActive) {
        auto isWrap = false;
        POINT pos { x, y };
        auto size = Renderer::GetSize(this->_hWnd);

        if (pos.x < 0) {
            ::OutputDebugString(_T("Wrap left.\r\n"));
            pos.x += static_cast<LONG>(size.width);
            isWrap = true;
        }

        if (pos.x > static_cast<LONG>(size.width)) {
            ::OutputDebugString(_T("Wrap right.\r\n"));
            pos.x -= static_cast<LONG>(size.width);
            isWrap = true;
        }

        if (pos.y < 0) {
            ::OutputDebugString(_T("Wrap top.\r\n"));
            pos.y += static_cast<LONG>(size.height);
            isWrap = true;
        }

        if (pos.y > static_cast<LONG>(size.height)) {
            ::OutputDebugString(_T("Wrap bottom.\r\n"));
            pos.y -= static_cast<LONG>(size.height);
            isWrap = true;
        }

        if (isWrap) {
            ::ClientToScreen(this->_hWnd, &pos);
            ::SetCursorPos(pos.x, pos.y);
        }

        MagicMousePad::MousePositionMessage msg;
        msg.X = pos.x;  // TODO
        msg.Y = pos.y;  // TODO
        this->_server.Send(msg);
    }
}


/*
 * State::OnMouseUp
 */
void State::OnMouseUp(const MagicMousePad::MouseButton button) {
    if (this->_isActive) {
        MagicMousePad::MouseUpMessage msg;
        msg.Button = static_cast<decltype(msg.Button)>(button);
        this->_server.Send(msg);
    }
}
