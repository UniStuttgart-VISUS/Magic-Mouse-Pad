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
}


/*
 * MouseSubscriber::OnMouseVisibilityChanged
 */
void MouseSubscriber::OnMouseVisibilityChanged(const bool isVisible) {
    ::OutputDebugString(_T("Received visibility update.\r\n"));
    ::ShowCursor(isVisible ? TRUE : FALSE);
}
