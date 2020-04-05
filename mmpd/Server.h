// <copyright file="Server.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once

#include "Client.h"


/// <summary>
/// The server that manages the client connections.
/// </summary>
class Server {

public:

    Server(void);

    ~Server(void);

    void Start(const std::uint16_t port, const int addressFamily = AF_INET);

    void Stop(void) noexcept;

private:

    void Receive(void);

    std::vector<Client> _clients;
    CRITICAL_SECTION _lock;
    std::thread _receiver;
    decltype(MagicMousePad::Header::SequenceNumber) _sequenceNumber;
    SOCKET _socket;

};
