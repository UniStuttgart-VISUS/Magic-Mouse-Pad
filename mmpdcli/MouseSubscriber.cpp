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

#include "MagicMousePad/serveraddress.h"


#if !defined(_WIN32)
#define SOCKET_ERROR(-1)
#endif /* !defined(_WIN32) */


/*
 * MagicMousePad::MouseSubscriber::~MouseSubscriber
 */
MagicMousePad::MouseSubscriber::~MouseSubscriber(void) {
}


/*
 * MagicMousePad::MouseSubscriber::MouseSubscriber
 */
MagicMousePad::MouseSubscriber::MouseSubscriber(void)
    : _sequenceNumber(0), _socket(SOCKET_ERROR) { }


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
    if (this->_socket == SOCKET_ERROR) {
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
    this->_receiver = std::thread(std::bind(&MouseSubscriber::Receive, this));
}


/*
 * MagicMousePad::MouseSubscriber::Subscribe
 */
void MagicMousePad::MouseSubscriber::Subscribe(const EndPointType& server,
        const PortType port, SubscriptionMessage subscription) {
    assert(subscription.Header.ID == SubscriptionMessageID);
    assert(subscription.Header.Length == sizeof(subscription));
    auto a = reinterpret_cast<const sockaddr *>(&server);
    auto l = 0;

    switch (a->sa_family) {
        case AF_INET:
            this->Subscribe(port, AddressFamily::InterNetwork);
            l = sizeof(sockaddr_in);
            break;

        case AF_INET6:
            this->Subscribe(port, AddressFamily::InterNetwork6);
            l = sizeof(sockaddr_in6);
            break;

        default:
            throw std::invalid_argument("The server end point address must "
                "either be an IPv4 or IPv6 address");
    }

    ToNetworkOrder(subscription);

    if (::sendto(this->_socket, reinterpret_cast<const char *>(&subscription),
            sizeof(subscription), 0, a, l) == SOCKET_ERROR) {
        throw this->GetSocketError("Failed to send subscription message");
    }
}


/*
 * MagicMousePad::MouseSubscriber::Unsubscribe
 */
void MagicMousePad::MouseSubscriber::Unsubscribe(void) {

#if defined(_WIN32)
    ::WSACleanup();
#endif /* defined(_WIN32) */
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
 * MagicMousePad::MouseSubscriber::Receive
 */
void MagicMousePad::MouseSubscriber::Receive(void) {
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
    sockaddr_storage peer;
    int peerLen = 0;

    while (true) {
        {
            auto cnt = ::recvfrom(this->_socket, buffer.data(), sizeof(Header),
                0, reinterpret_cast<sockaddr *>(&peer), &peerLen);
            if (cnt <= 0) {
                break;
            }
        }

        auto header = reinterpret_cast<Header *>(buffer.data());
        if (buffer.size() < header->Length) {
            buffer.resize(header->Length);
        }
        header = reinterpret_cast<Header *>(buffer.data());

        {
            auto size = ToHostOrder(header->Length) - sizeof(Header);
            auto cnt = ::recvfrom(this->_socket, buffer.data() + sizeof(Header),
                size, 0, reinterpret_cast<sockaddr *>(&peer), &peerLen);
            if (cnt <= 0) {
                break;
            }

            switch (header->ID) {
                case MousePositionMessageID: {
                    auto m = reinterpret_cast<MousePositionMessage *>(
                        buffer.data());
                    ToHostOrder(*m);
                    if (this->CheckSequenceNumber(m->Header)) {
                        this->OnMouseMove(m->X, m->Y);
                    }
                } break;

                case MouseDownMessageID: {
                    auto m = reinterpret_cast<MouseDownMessage *>(
                        buffer.data());
                    ToHostOrder(*m);
                    if (this->CheckSequenceNumber(m->Header)) {
                        this->OnMouseDown(static_cast<MouseButton>(m->Button));
                    }
                } break;

                case MouseUpMessageID: {
                    auto m = reinterpret_cast<MouseUpMessage *>(
                        buffer.data());
                    ToHostOrder(*m);
                    if (this->CheckSequenceNumber(m->Header)) {
                        this->OnMouseUp(static_cast<MouseButton>(m->Button));
                    }
                } break;

                case MouseVisibilityMessageID: {
                    auto m = reinterpret_cast<MouseVisibilityMessage *>(
                        buffer.data());
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
    }

#if defined(_WIN32)
    ::WSACleanup();
#endif /* defined(_WIN32) */
}
