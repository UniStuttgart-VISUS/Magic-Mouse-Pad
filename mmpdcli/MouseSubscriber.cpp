// <copyright file="MouseSubscriber.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "MagicMousePad/MouseSubscriber.h"

#include <cassert>
#include <functional>
#include <stdexcept>
#include <vector>

#include <ws2ipdef.h>

#include "MagicMousePad/receive.h"
#include "MagicMousePad/serveraddress.h"


#if !defined(_WIN32)
#define SOCKET_ERROR (-1)
#define INVALID_SOCKET (-1)
#endif /* !defined(_WIN32) */


/*
 * MagicMousePad::MouseSubscriber::~MouseSubscriber
 */
MagicMousePad::MouseSubscriber::~MouseSubscriber(void) {
    this->Unsubscribe();
}


/*
 * MagicMousePad::MouseSubscriber::MouseSubscriber
 */
MagicMousePad::MouseSubscriber::MouseSubscriber(void)
        : _sequenceNumber(0), _socket(INVALID_SOCKET) {
    ::ZeroMemory(&this->_server, sizeof(this->_server));
}


/*
 * MagicMousePad::MouseSubscriber::OnMouseDown
 */
void MagicMousePad::MouseSubscriber::OnMouseDown(const MouseButton button) { }


/*
 * MagicMousePad::MouseSubscriber::OnMouseMove
 */
void MagicMousePad::MouseSubscriber::OnMouseMove(const std::int32_t x,
    const std::int32_t y) { }


/*
 * MagicMousePad::MouseSubscriber::OnMouseUp
 */
void MagicMousePad::MouseSubscriber::OnMouseUp(
    const MagicMousePad::MouseButton button) { }


/*
 * MagicMousePad::MouseSubscriber::OnMouseVisibilityChanged
 */
void MagicMousePad::MouseSubscriber::OnMouseVisibilityChanged(
    const bool isVisible) { }


/*
 * MagicMousePad::MouseSubscriber::Subscribe
 */
void MagicMousePad::MouseSubscriber::Subscribe(const PortType port,
        const AddressFamily protocol) {
    assert((protocol == AddressFamily::InterNetwork)
        || (protocol == AddressFamily::InterNetwork6));
    if (this->_socket != -1) {
        throw std::logic_error("A subscription cannot be started unless any "
            "ongoing subscription has been cancelled");
    }

#if defined(_WIN32)
    {
        WSADATA wsaData;
        auto status = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (status != NO_ERROR) {
            throw std::system_error(status, std::system_category(),
                "Failed to start Winsock 2.2");
        }
    }
#endif /* defined(_WIN32) */

    this->_socket = ::socket(static_cast<int>(protocol), SOCK_DGRAM,
        IPPROTO_UDP);
    if (this->_socket == INVALID_SOCKET) {
        throw this->GetSocketError("Failed to create UDP socket");
    }

    {
        sockaddr_storage address;
        auto endPoint = InitialiseServerAddress(address,
            static_cast<std::uint16_t>(protocol), port);
        if (::bind(this->_socket, endPoint.first, endPoint.second)
                == SOCKET_ERROR) {
            throw this->GetSocketError("Failed to bind socket");
        }
    }

    assert(!this->_receiver.joinable());
    this->_receiver = std::thread(MagicMousePad::Receive, this->_socket,
        std::bind(&MouseSubscriber::OnMessage, this, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3));
}


/*
 * MagicMousePad::MouseSubscriber::Subscribe
 */
void MagicMousePad::MouseSubscriber::Subscribe(const EndPointType& server,
        const PortType port, const SubscriptionMessage& subscription) {
    assert(subscription.Header.ID == SubscriptionMessageID);
    assert(subscription.Header.Length == sizeof(subscription));
    auto address = CheckAddress(server);
    this->Subscribe(port, address.first);
    this->_server = server;
    this->UpdateSubscription(subscription);
}


/*
 * MagicMousePad::MouseSubscriber::Unsubscribe
 */
void MagicMousePad::MouseSubscriber::Unsubscribe(void) {
    ::closesocket(this->_socket);
    if (this->_receiver.joinable()) {
        this->_receiver.join();
    }

#if defined(_WIN32)
    ::WSACleanup();
#endif /* defined(_WIN32) */
}


/*
 * MagicMousePad::MouseSubscriber::UpdateSubscription
 */
void MagicMousePad::MouseSubscriber::UpdateSubscription(
        SubscriptionMessage subscription) {
    auto address = CheckAddress(this->_server);
    auto a = reinterpret_cast<sockaddr *>(&this->_server);

    ToNetworkOrder(subscription.Header);
    ToNetworkOrder(subscription);

    if (::sendto(this->_socket, reinterpret_cast<const char *>(&subscription),
            sizeof(subscription), 0, a, address.second) == SOCKET_ERROR) {
        throw this->GetSocketError("Failed to send subscription message");
    }
}


/*
 * MagicMousePad::MouseSubscriber::CheckAddress
 */
std::pair< MagicMousePad::MouseSubscriber::AddressFamily, int>
MagicMousePad::MouseSubscriber::CheckAddress(const EndPointType& address) {
    switch (address.ss_family) {
        case AF_INET:
            return std::make_pair(AddressFamily::InterNetwork,
                static_cast<int>(sizeof(sockaddr_in)));

        case AF_INET6:
            return std::make_pair(AddressFamily::InterNetwork6,
                static_cast<int>(sizeof(sockaddr_in6)));

        default:
            throw std::invalid_argument("The server end point address must "
                "either be an IPv4 or IPv6 address");
    }
}


/*
 * MagicMousePad::MouseSubscriber::CheckSequenceNumber
 */
bool MagicMousePad::MouseSubscriber::CheckSequenceNumber(const Header& header) {
    auto retval = (header.SequenceNumber > this->_sequenceNumber);
    this->_sequenceNumber = header.SequenceNumber;
    return retval;
}


/*
 * MagicMousePad::MouseSubscriber::GetSocketError
 */
std::system_error MagicMousePad::MouseSubscriber::GetSocketError(
        const std::string &msg) {
#if defined(_WIN32)
    throw std::system_error(::WSAGetLastError(), std::system_category(), msg);
#else /* defined(_WIN32) */
    throw std::system_error(errno, std::system_category(), msg);
#endif /* defined(_WIN32) */
}


/*
 * MagicMousePad::MouseSubscriber::OnMessage
 */
void MagicMousePad::MouseSubscriber::OnMessage(Header *header,
        const sockaddr &, int) {
    switch (header->ID) {
        case MousePositionMessageID: {
            auto m = reinterpret_cast<MousePositionMessage *>(header);
            ToHostOrder(*m);
            if (this->CheckSequenceNumber(m->Header)) {
                this->OnMouseMove(m->X, m->Y);
            }
        } break;

        case MouseDownMessageID: {
            auto m = reinterpret_cast<MouseDownMessage *>(header);
            ToHostOrder(*m);
            if (this->CheckSequenceNumber(m->Header)) {
                this->OnMouseDown(static_cast<MouseButton>(m->Button));
            }
        } break;

        case MouseUpMessageID: {
            auto m = reinterpret_cast<MouseUpMessage *>(header);
            ToHostOrder(*m);
            if (this->CheckSequenceNumber(m->Header)) {
                this->OnMouseUp(static_cast<MouseButton>(m->Button));
            }
        } break;

        case MouseVisibilityMessageID: {
            auto m = reinterpret_cast<MouseVisibilityMessage *>(header);
            ToHostOrder(*m);
            if (this->CheckSequenceNumber(m->Header)) {
                this->OnMouseVisibilityChanged(m->IsVisible != 0);
            }
        } break;

        default:
            // Ignore any message we do not know.
            break;
    }
}
