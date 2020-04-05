// <copyright file="Server.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "Client.h"


/// <summary>
/// The server that manages the client connections.
/// </summary>
class Server {

public:

    Server(void);

    ~Server(void);

    inline void Send(MagicMousePad::MouseDownMessage msg) {
        using namespace MagicMousePad;
        ToNetworkOrder(msg.Header);
        ToNetworkOrder(msg);
        this->Send(msg, OnSendCallback<decltype(msg)>());
    }

    void Send(MagicMousePad::MousePositionMessage msg);

    inline void Send(MagicMousePad::MouseUpMessage msg) {
        using namespace MagicMousePad;
        ToNetworkOrder(msg.Header);
        ToNetworkOrder(msg);
        this->Send(msg, OnSendCallback<decltype(msg)>());
    }

    inline void Send(MagicMousePad::MouseVisibilityMessage msg) {
        using namespace MagicMousePad;
        ToNetworkOrder(msg.Header);
        ToNetworkOrder(msg);
        this->Send(msg, OnSendCallback<decltype(msg)>());
    }

    void Start(std::uint16_t port, const int addressFamily = AF_INET);

    void Stop(void) noexcept;

private:

    template<class T>
    using OnSendCallback = std::function<void(const Client&)>;

    void OnMessage(MagicMousePad::Header *header,
        const sockaddr& clientAddr, const int clientAddrLen);

    template<class T> void Send(T& msg, const OnSendCallback<T>& onSend);

    std::vector<Client> _clients;
    CRITICAL_SECTION _lock;
    std::thread _receiver;
    decltype(MagicMousePad::Header::SequenceNumber) _sequenceNumber;
    SOCKET _socket;

};

#include "Server.inl"
