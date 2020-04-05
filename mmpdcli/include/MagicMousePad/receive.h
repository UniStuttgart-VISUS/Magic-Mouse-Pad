// <copyright file="receive.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <functional>


/* Forward declarations. */
namespace MagicMousePad { struct Header; }
struct sockaddr;


namespace MagicMousePad {

    /// <summary>
    /// The type of the callback passed to <see cref="Receive" />.
    /// </summary>
    /// <remarks>
    /// <para>The actual message body follows directly on the header passed to
    /// the callback. The pointer can therefore be reinterpret casted to the
    /// actual message type.</para>
    /// <para>The header has been converted to host byte order, whereas the body
    /// has not. The callback must perform this conversion.</para>
    /// </remarks>
    typedef std::function<void(Header *, const sockaddr&, const int)>
        ReceiveCallback;

    /// <summary>
    /// Receives messages and invokes <paramref name="callback" /> for each of
    /// them.
    /// </summary>
    /// <remarks>
    /// <para>It is assumed that this function is running in a separte thread,
    /// so the function will initialise Winsock on enter. Close
    /// <paramref name="socket" /> for the function to exit.</para>
    /// </remarks>
    /// <param name="senderAddr">The socket to receive from. If a socket error
    /// occurs on this socket, the method will exit.</param>
    /// <param name="callback">The callback to be invoked for each message. The
    /// body directly follows the header in memory.</param>.
    void Receive(const int socket, const ReceiveCallback& callback);
}
