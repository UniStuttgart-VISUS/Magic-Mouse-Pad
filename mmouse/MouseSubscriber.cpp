// <copyright file="MouseSubscriber.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"
#include "MouseSubscriber.h"


/*
 * MouseSubscriber::MouseSubscriber
 */
MouseSubscriber::MouseSubscriber(const CommandLine& cmdLine) {
    sockaddr_storage server;
    auto hack = reinterpret_cast<sockaddr_in *>(&server);
    hack->sin_family = AF_INET;
    hack->sin_addr.S_un.S_addr = (INADDR_LOOPBACK);
    hack->sin_addr.S_un.S_un_b.s_b1 = 192;
    hack->sin_addr.S_un.S_un_b.s_b2 = 168;
    hack->sin_addr.S_un.S_un_b.s_b3 = 1;
    hack->sin_addr.S_un.S_un_b.s_b4 = 184;
    hack->sin_port = htons(47596);
    this->Subscribe(server, /*cmdLine.GetPort()*/ 14263);
}


/*
 * MouseSubscriber::OnMouseMove
 */
void MouseSubscriber::OnMouseMove(const std::int32_t x,
        const std::int32_t y) {
    ::SetCursorPos(x, y);
}


/*
 * MouseSubscriber::OnMouseVisibilityChanged
 */
void MouseSubscriber::OnMouseVisibilityChanged(const bool isVisible) {
    ::ShowCursor(isVisible ? TRUE : FALSE);
}
