// <copyright file="MouseSubscriber.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"
#include "MouseSubscriber.h"


/*
 * MouseSubscriber::MouseSubscriber
 */
MouseSubscriber::MouseSubscriber(const CommandLine& cmdLine)
        : _cmdLine(cmdLine), _isDisabled(cmdLine.IsDisabled()) {
    ::OutputDebugString(_T("Subscribing to server ...\r\n"));
    this->Subscribe(this->_cmdLine.GetServer(), this->_cmdLine.GetPort());
}


/*
 * MouseSubscriber::Announce
 */
void MouseSubscriber::Announce(void) {
    ::OutputDebugString(_T("Sending subscription update ...\r\n"));
    this->UpdateSubscription(MagicMousePad::SubscriptionMessage());
}


/*
 * MouseSubscriber::OnMouseDown
 */
void MouseSubscriber::OnMouseDown(const MagicMousePad::MouseButton button) {
    ::OutputDebugString(_T("Received mouse-down event.\r\n"));
    if (!this->_isDisabled) {
        INPUT input;
        ::ZeroMemory(&input, sizeof(input));
        input.type = INPUT_MOUSE;

        switch (button) {
            case MagicMousePad::MouseButton::Left:
                input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                break;

            case MagicMousePad::MouseButton::Middle:
                input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
                break;

            case MagicMousePad::MouseButton::Right:
                input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                break;

            default:
                ::OutputDebugString(_T("Unexpected mouse button down.\r\n"));
                return;
        }

        ::SendInput(1, &input, sizeof(input));
    }
}


/*
 * MouseSubscriber::OnMouseMove
 */
void MouseSubscriber::OnMouseMove(const std::int32_t x,
        const std::int32_t y) {
    ::OutputDebugString(_T("Received mouse position.\r\n"));
    //std::stringstream msg;
    //msg << "received: " << x << "," << y << std::endl;
    //::OutputDebugStringA(msg.str().c_str());

    if (!this->_isDisabled) {
        ::SetCursorPos(x, y);
    }
}


/*
 * MouseSubscriber::OnMouseUp
 */
void MouseSubscriber::OnMouseUp(const MagicMousePad::MouseButton button) {
    ::OutputDebugString(_T("Received mouse-up event.\r\n"));
    if (!this->_isDisabled) {
        INPUT input;
        ::ZeroMemory(&input, sizeof(input));
        input.type = INPUT_MOUSE;

        switch (button) {
        case MagicMousePad::MouseButton::Left:
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            break;

        case MagicMousePad::MouseButton::Middle:
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            break;

        case MagicMousePad::MouseButton::Right:
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            break;

        default:
            ::OutputDebugString(_T("Unexpected mouse button up.\r\n"));
            return;
        }

        ::SendInput(1, &input, sizeof(input));
    }

}


/*
 * MouseSubscriber::OnMouseVisibilityChanged
 */
void MouseSubscriber::OnMouseVisibilityChanged(const bool isVisible) {
    ::OutputDebugString(_T("Received visibility update.\r\n"));
    ::ShowCursor(isVisible ? TRUE : FALSE);
}
