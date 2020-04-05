// <copyright file="Server.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>


/*
 * Server::Send
 */
template<class T> void Server::Send(T& msg, const OnSendCallback<T>& onSend) {
    ::EnterCriticalSection(&this->_lock);

    for (auto c = this->_clients.begin(); c != this->_clients.end();) {
        if (onSend) {
            onSend(*c);
        }

        //::OutputDebugString(_T("Sending message ...\r\n"));
        auto cnt = ::sendto(this->_socket, reinterpret_cast<const char *>(&msg),
            sizeof(T), 0, c->GetAddress(), c->GetAddressLength());
        if (cnt <= 0) {
            c = this->_clients.erase(c);
        } else {
            ++c;
        }
    }

    ::LeaveCriticalSection(&this->_lock);
}
