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
State::State(HWND hWnd, const CommandLine &cmdLine)
        : _bounds(cmdLine.GetBounds()), _escapeKey(VK_ESCAPE),
        _hWnd(hWnd), _isActive(false), _position(cmdLine.GetStartPoint()),
        _renderer(hWnd) {
    this->_renderer.SetBackgroundColour(D2D1::ColorF::DarkGray);
    this->_server.Start(cmdLine.GetPort(), cmdLine.GetAddressFamily());
    State::Attach(*this, hWnd);
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

        // Capture mouse to receive events when it leaves the window.
        ::SetCapture(this->_hWnd);

        // Initialise previous position at the current cursor position.
        ::GetCursorPos(&this->_prevPosition);
        ::ScreenToClient(this->_hWnd, &this->_prevPosition);
    }

    if (this->_isActive) {
        MagicMousePad::MouseDownMessage msg;
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

        // Check whether the mouse left he mouse pad and wrap the position. Note
        // that we need to wrap the previous position as well to get valid
        // deltas.
        if (pos.x < 0) {
            ::OutputDebugString(_T("Wrap left.\r\n"));
            pos.x += static_cast<LONG>(size.width);
            this->_prevPosition.x += static_cast<LONG>(size.width);
            isWrap = true;
        }

        if (pos.x > static_cast<LONG>(size.width)) {
            ::OutputDebugString(_T("Wrap right.\r\n"));
            pos.x -= static_cast<LONG>(size.width);
            this->_prevPosition.x -= static_cast<LONG>(size.width);
            isWrap = true;
        }

        if (pos.y < 0) {
            ::OutputDebugString(_T("Wrap top.\r\n"));
            pos.y += static_cast<LONG>(size.height);
            this->_prevPosition.y += static_cast<LONG>(size.height);
            isWrap = true;
        }

        if (pos.y > static_cast<LONG>(size.height)) {
            ::OutputDebugString(_T("Wrap bottom.\r\n"));
            pos.y -= static_cast<LONG>(size.height);
            this->_prevPosition.y -= static_cast<LONG>(size.height);
            isWrap = true;
        }

        // Update the global position based on the delta.
        {
            auto dx = pos.x - this->_prevPosition.x;
            auto dy = pos.y - this->_prevPosition.y;
            this->_position.x += dx;
            this->_position.y += dy;

            //std::wstringstream msg;
            //msg << "Global before clamp: (" << this->_position.x << ", "
            //    << this->_position.y << ")" << std::endl;
            //::OutputDebugStringW(msg.str().c_str());
        }

        // If the virtual mouse left the virtual desktop, clamp the position.
        if (this->_position.x < this->_bounds.left) {
            ::OutputDebugString(_T("Clamp left.\r\n"));
            this->_position.x = this->_bounds.left;
        }

        if (this->_position.x > this->_bounds.right) {
            ::OutputDebugString(_T("Clamp right.\r\n"));
            this->_position.x = this->_bounds.right;
        }

        if (this->_position.y < this->_bounds.top) {
            ::OutputDebugString(_T("Clamp top.\r\n"));
            this->_position.y = this->_bounds.top;
        }

        if (this->_position.y > this->_bounds.bottom) {
            ::OutputDebugString(_T("Clamp bottom.\r\n"));
            this->_position.y = this->_bounds.bottom;
        }

        // Remember the previous window-local position before we convert it to
        // desktop coordinates to move the cursor.
        this->_prevPosition = pos;

        // Move the local cursor to reflect the wrap.
        if (isWrap) {
            ::ClientToScreen(this->_hWnd, &pos);
            ::SetCursorPos(pos.x, pos.y);
        }

        // Send out the new position.
        MagicMousePad::MousePositionMessage msg;
        msg.X = this->_position.x;
        msg.Y = this->_position.y;
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
