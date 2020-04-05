// copyright file="Client.cpp" company="Visualisierungsinstitut der Universität Stuttgart" >
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"
#include "Client.h"


/*
 * Client::Client
 */
Client::Client(const sockaddr *address, const int addressLength)
        : _addressLength(addressLength) {
    if (address == nullptr) {
        throw std::invalid_argument("The client address must not be nullptr");
    }
    if (addressLength > sizeof(this->_address)) {
        throw std::invalid_argument("The specified address is too long");
    }

    ::memcpy(&this->_address, address, this->_addressLength);
    ::ZeroMemory(&this->_clippingArea, sizeof(this->_clippingArea));
}


/*
 * Client::SetClippingArea
 */
void Client::SetClippingArea(const MagicMousePad::SubscriptionMessage& msg) {
    this->_clippingArea.left = msg.Left;
    this->_clippingArea.top = msg.Top;
    this->_clippingArea.right = msg.Left + msg.Width;
    this->_clippingArea.bottom = msg.Top + msg.Height;
}
