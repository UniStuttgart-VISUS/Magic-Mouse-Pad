// <copyright file="Server.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"
#include "Server.h"


/*
 * Server::Server
 */
Server::Server(void) : _seqNumber(0), _socket(SOCKET_ERROR) {
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
 * Server::AddClient
 */
void Server::AddClient(const sockaddr_storage& address) {
    switch (address.ss_family) {
        case AF_INET:
            this->AddClient(*reinterpret_cast<const sockaddr *>(&address),
                sizeof(sockaddr_in));
            break;

        case AF_INET6:
            this->AddClient(*reinterpret_cast<const sockaddr *>(&address),
                sizeof(sockaddr_in));
            break;

        default:
            throw std::invalid_argument("The client address must either be an "
                "IPv4 or IPv6 address.");
    }
}


/*
 * Server::AddClient
 */
void Server::AddClient(const sockaddr& address, const int addressLen) {
    ::EnterCriticalSection(&this->_lock);

    auto client = std::find_if(this->_clients.begin(), this->_clients.end(),
        [&address, addressLen](const Client &c) {
            return c.IsAddress(address, addressLen);
        });

    if (client == this->_clients.end()) {
        this->_clients.emplace_back(&address, addressLen);
    }

    ::LeaveCriticalSection(&this->_lock);
}


/*
 * Server::Send
 */
void Server::Send(MagicMousePad::MousePositionMessage msg) {
    using namespace MagicMousePad;

    msg.Header.SequenceNumber = ++this->_seqNumber;
    ToNetworkOrder(msg.Header);

    const auto x = msg.X;
    const auto y = msg.Y;
    this->Send(msg, [&msg, x, y](const Client& c) {
        msg.X = x;
        msg.Y = y;
        c.ClipPosition(msg.X, msg.Y);
        ToNetworkOrder(msg);
        });
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
    this->_receiver = std::thread(MagicMousePad::Receive, this->_socket,
        std::bind(&Server::OnMessage, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3));
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
 * Server::OnMessage
 */
void Server::OnMessage(MagicMousePad::Header *header,
        const sockaddr& clientAddr, const int clientAddrLen) {
    using namespace MagicMousePad;

    if (header->ID == SubscriptionMessageID) {
        auto m = reinterpret_cast<SubscriptionMessage *>(header);
        ToHostOrder(*m);

        ::EnterCriticalSection(&this->_lock);

        auto client = std::find_if(this->_clients.begin(), this->_clients.end(),
            [&clientAddr, clientAddrLen](const Client &c) {
                return c.IsAddress(clientAddr, clientAddrLen);
            });

        if (client != this->_clients.end()) {
            // We know this client already, just update its clipping area.
            ::OutputDebugString(_T("Known client sent update.\r\n"));
            client->SetClippingArea(*m);

        } else {
            // Add a new client to the list of subscriptions.
            ::OutputDebugString(_T("New client connected.\r\n"));
            this->_clients.emplace_back(&clientAddr, clientAddrLen);
            this->_clients.back().SetClippingArea(*m);
        }

        ::LeaveCriticalSection(&this->_lock);
    }
}
