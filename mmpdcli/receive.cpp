// <copyright file="receive.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "MagicMousePad/receive.h"

#include <stdexcept>
#include <sstream>
#include <vector>

#include <tchar.h>

#include "MagicMousePad/messages.h"


/*
 * MagicMousePad::Receive
 */
void MagicMousePad::Receive(const SOCKET socket, const ReceiveCallback &onReceive,
        const ErrorCallback &onError) {
    if (!onReceive) {
        throw std::invalid_argument("The message callback must be valid");
    }

#if defined(_WIN32)
    {
        WSADATA wsaData;
        auto status = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (status != NO_ERROR) {
            throw std::system_error(status, std::system_category(),
                "Failed to start Winsock 2.2 in receiver thread");
        }
    }
#endif /* defined(_WIN32) */

    // Note: the size of the largest datagram must be used here.
    std::vector<char> buffer(2 * sizeof(MousePositionMessage), 0);
    sockaddr_storage peerStorage;
    auto peer = reinterpret_cast<sockaddr *>(&peerStorage);
    int peerLen = sizeof(peerStorage);

    while (true) {
        auto cnt = static_cast<int>(buffer.size());
        cnt = ::recvfrom(socket, buffer.data(), cnt, 0, peer, &peerLen);
        if (cnt <= 0) {
            auto error = std::system_error(::WSAGetLastError(),
                std::system_category(), "Failed to receive message");
            ::OutputDebugStringA(error.what());
            if (!onError || !onError(error)) {
                // If no error callback is set or it returned false, exit
                // the loop on error.
                break;
            }
        }

        auto header = reinterpret_cast<Header *>(buffer.data());
        ToHostOrder(*header);
        onReceive(header, *peer, peerLen);
    } /* end while (true) */

#if defined(_WIN32)
    ::WSACleanup();
#endif /* defined(_WIN32) */
}
