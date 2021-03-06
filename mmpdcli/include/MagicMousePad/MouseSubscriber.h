// <copyright file="MouseSubscriber.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once

#include <WinSock2.h>

#include <string>
#include <system_error>
#include <thread>

#include "MagicMousePad/messages.h"
#include "MagicMousePad/MouseButton.h"
#include "MagicMousePad/receive.h"


namespace MagicMousePad {

    /// <summary>
    /// The parent class clients that want to receive data from the magic mouse
    /// pad must inherit from.
    /// </summary>
    class MouseSubscriber {

    public:

        /// <summary>
        /// Finalises the instance.
        /// </summary>
        virtual ~MouseSubscriber(void);

    protected:

        /// <summary>
        /// All supported address families.
        /// </summary>
        enum class AddressFamily : std::int32_t {

            /// <summary>
            /// Use IPv4 to subscribe messages from magic mouse pad.
            /// </summary>
            InterNetwork = AF_INET,

            /// <summary>
            /// Use IPv6 to subscribe messages from magic mouse pad.
            /// </summary>
            InterNetwork6 = AF_INET6
        };

        /// <summary>
        /// The type used to specify an IP end point.
        /// </summary>
        typedef struct sockaddr_storage EndPointType;

        /// <summary>
        /// The type used to represent a network port.
        /// </summary>
        typedef std::uint16_t PortType;

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        MouseSubscriber(void);

        /// <summary>
        /// This method is invoked if a network error occurred.
        /// </summary>
        /// <remarks>
        /// <para>This method can be called from an arbitrary thread context.
        /// </para>
        /// <para>The default implementation returns <c>false</c> to signal the
        /// receiver to exit.</para>
        /// </remarks>
        /// <param name="button"></param>
        virtual bool OnError(const std::system_error& error);

        /// <summary>
        /// This method is invoked if the subscriber receives a message that a
        /// mouse button was pressed in the magic mouse pad.
        /// </summary>
        /// <remarks>
        /// This method can be called from an arbitrary thread context.
        /// </remarks>
        /// <param name="button"></param>
        virtual void OnMouseDown(const MouseButton button);

        /// <summary>
        /// This method is invoked if the subscriber recieves a message that the
        /// mouse was moved in the magic mouse pad.
        /// </summary>
        /// <remarks>
        /// This method can be called from an arbitrary thread context.
        /// </remarks>
        /// <param name="x"></param>
        /// <param name="y"></param>
        virtual void OnMouseMove(const std::int32_t x, const std::int32_t y);

        /// <summary>
        /// This method is invoked if the subscriber receives a message that a
        /// mouse button was released in the magic mouse pad.
        /// </summary>
        /// <remarks>
        /// This method can be called from an arbitrary thread context.
        /// </remarks>
        /// <param name="button"></param>
        virtual void OnMouseUp(const MagicMousePad::MouseButton button);

        /// <summary>
        /// This method is invoked if the subscriber receives a message that
        /// indicates that the visibility of the mouse cursor should be changed.
        /// </summary>
        /// <remarks>
        /// This method can be called from an arbitrary thread context.
        /// </remarks>
        /// <param name="button"></param>
        virtual void OnMouseVisibilityChanged(const bool isVisible);

        /// <summary>
        /// Starts an unconstrained subscription for mouse messages on the
        /// specified port.
        /// </summary>
        /// <remarks>
        /// <para>This method simply puts the subscriber into the listening
        /// state on the specified port. Note that the method does not request
        /// a subscription, but the subscriber must be added on the server side
        /// manually.</para>
        /// </remarks>
        /// <param name="port">The port to listen on.</param>
        /// <param name="protocol">The protocol family to use, which
        /// defaults to <see cref="Protocol::InterNetwork" />.</para>
        /// <exception cref="std::logic_error">If the the subscriber is already
        /// running.</exception>
        void Subscribe(const PortType port,
            const AddressFamily protocol = AddressFamily::InterNetwork);

        /// <summary>
        /// Starts a subscription for mouse messages on the specified port and
        /// request to transform mouse coordinates into the coordinate system
        /// specified in <paramref name="subscription" />.
        /// </summary>
        /// <param name="server">The address of the magic mouse pad to connect
        /// to. The address family of this end point address must be either
        /// IPv4 and IPv6. It also determines the address family used for the
        /// local socket opened on port <paramref name="port" />.</param>
        /// <param name="port">The port to listen on.</param>
        /// <param name="subscription">A description of the subscription, which
        /// allows the subscriber to request mouse coordinates in its local
        /// coordinate system.</param>
        /// <exception cref="std::logic_error">If the the subscriber is already
        /// running.</exception>
        void Subscribe(const EndPointType& server, const PortType port,
            const SubscriptionMessage& subscription);

        /// <summary>
        /// Starts a subscription using an ephemeral port.
        /// </summary>
        /// <param name="server">The address of the magic mouse pad to connect
        /// to. The address family of this end point address must be either
        /// IPv4 and IPv6.</param>
        /// <param name="subscription">A description of the subscription, which
        /// allows the subscriber to request mouse coordinates in its local
        /// coordinate system.</param>
        /// <exception cref="std::logic_error">If the the subscriber is already
        /// running.</exception>
        inline void Subscribe(const EndPointType& server,
                const SubscriptionMessage& subscription) {
            this->Subscribe(server, 0, subscription);
        }

        /// <summary>
        /// Starts an unconstrained subscription on an ephemeral port.
        /// </summary>
        /// <param name="server">The address of the magic mouse pad to connect
        /// to. The address family of this end point address must be either
        /// IPv4 and IPv6.</param>
        /// <exception cref="std::logic_error">If the the subscriber is already
        /// running.</exception>
        inline void Subscribe(const EndPointType& server) {
            this->Subscribe(server, 0, SubscriptionMessage());
        }

        /// <summary>
        /// Starts an unconstrained subscription on the specified port.
        /// </summary>
        /// <param name="server">The address of the magic mouse pad to connect
        /// to. The address family of this end point address must be either
        /// IPv4 and IPv6.</param>
        /// <param name="port">The port to listen on.</param>
        /// <exception cref="std::logic_error">If the the subscriber is already
        /// running.</exception>
        inline void Subscribe(const EndPointType &server, const PortType port) {
            this->Subscribe(server, port, SubscriptionMessage());
        }

        /// <summary>
        /// Cancels any ongoing subscription of mouse messages.
        /// </summary>
        void Unsubscribe(void);

        /// <summary>
        /// Resends the subscription message to the given server.
        /// </summary>
        /// <param name="subscription">A description of the subscription, which
        /// allows the subscriber to request mouse coordinates in its local
        /// coordinate system.</param>
        /// <exception cref="std::system_error">If the communication failed
        /// because no subscription was made before.</exception>
        void UpdateSubscription(SubscriptionMessage subscription);

    private:

#if defined(_WIN32)
        typedef SOCKET SocketType;
#else /* defined(_WIN32) */
        typedef int SocketType;
#endif /* defined(_WIN32) */

        static std::pair<AddressFamily, int> CheckAddress(
            const EndPointType& address);

        bool CheckSequenceNumber(const Header &header);

        std::system_error GetSocketError(const std::string& msg);

        void OnMessage(Header *header, const sockaddr&, int);

        std::thread _receiver;
        decltype(Header::SequenceNumber) _sequenceNumber;
        EndPointType _server;
        SocketType _socket;

    };
}
