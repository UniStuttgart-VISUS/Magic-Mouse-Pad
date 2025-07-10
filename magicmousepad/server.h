// <copyright file="server.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

#include <WinSock2.h>

#include <wil/resource.h>


/// <summary>
/// The server keeps track of all the clients connected to the mouse pad that
/// receive updates from it.
/// </summary>
class server final {

public:

private:

    std::vector<wil::unique_socket> _clients;
    std::mutex _lock;
    wil::unique_socket _socket;

};
