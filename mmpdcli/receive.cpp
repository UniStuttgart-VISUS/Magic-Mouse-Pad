// <copyright file="receive.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "MagicMousePad/receive.h"

#include <stdexcept>
#include <system_error>
#include <vector>

#include "MagicMousePad/messages.h"


/*
 * MagicMousePad::Receive
 */
void MagicMousePad::Receive(const int socket, const ReceiveCallback& callback) {
    if (!callback) {
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

    std::vector<char> buffer(sizeof(Header), 0);
    sockaddr_storage peerStorage;
    auto peer = reinterpret_cast<sockaddr *>(&peerStorage);
    int peerLen = 0;

    while (true) {
        {
            auto cnt = ::recvfrom(socket, buffer.data(), sizeof(Header),
                0, peer, &peerLen);
            if (cnt <= 0) {
                break;
            }
        }

        auto header = reinterpret_cast<Header *>(buffer.data());
        if (buffer.size() < header->Length) {
            buffer.resize(header->Length);
        }
        header = reinterpret_cast<Header *>(buffer.data());
        ToHostOrder(*header);

        {
            auto cnt = ::recvfrom(socket, buffer.data() + sizeof(Header),
                header->Length - sizeof(Header), 0, peer, &peerLen);
            if (cnt <= 0) {
                break;
            }

            callback(header, *peer, peerLen);
        }

    } /* end while (true) */

#if defined(_WIN32)
    ::WSACleanup();
#endif /* defined(_WIN32) */
}
