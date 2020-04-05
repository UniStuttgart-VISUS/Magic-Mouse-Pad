// <copyright file="Server.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"
#include "Server.h"


/*
 * Server::Server
 */
Server::Server(void) : _sequenceNumber(0), _socket(SOCKET_ERROR) {
    ::InitializeCriticalSection(&this->_lock);
}


/*
 * Server::~Server
 */
Server::~Server(void) {
    this->Stop();
    ::DeleteCriticalSection(&this->_lock);
}


/*
 * Server::Start
 */
void Server::Start(const std::uint16_t port, const int addressFamily) {
    if (this->_socket != SOCKET_ERROR) {
        throw std::logic_error("The server is already running.");
    }

    sockaddr_storage address;
    auto endPoint = MagicMousePad::InitialiseServerAddress(address,
        addressFamily, port);

    this->_socket = ::socket(addressFamily, SOCK_DGRAM, IPPROTO_UDP);
    if (this->_socket == SOCKET_ERROR) {
        throw std::system_error(::WSAGetLastError(), std::system_category(),
            "Failed to create server socket");
    }

    if (::bind(this->_socket, endPoint.first, endPoint.second)
            == SOCKET_ERROR) {
        throw std::system_error(::WSAGetLastError(), std::system_category(),
            "Failed to bind server socket");
    }

    assert(!this->_receiver.joinable());
    this->_receiver = std::thread(std::bind(&Server::Receive, this));
}


/*
 * Server::Stop
 */
void Server::Stop(void) noexcept {
    ::closesocket(this->_socket);

    if (this->_receiver.joinable()) {
        this->_receiver.join();
    }
}


/*
 * Server::Receive
 */
void Server::Receive(void) {
    {
        WSADATA wsaData;
        auto status = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (status != NO_ERROR) {
            throw std::system_error(status, std::system_category(),
                "Failed to start Winsock 2.2 in receiver thread");
        }
    }

    while (true) {
        /// TODO
        break;
    }

    ::WSACleanup();
}
