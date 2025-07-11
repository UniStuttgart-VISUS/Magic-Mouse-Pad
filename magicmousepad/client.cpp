// <copyright file="client.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "client.h"


/*
 * client::client
 */
client::client(_In_ const sockaddr_storage& address) : _address(address) {
    this->update();
}


/*
 * client::address_length
 */
int client::address_length(void) const noexcept {
    switch (this->_address.ss_family) {
        case AF_INET:
            return sizeof(sockaddr_in);

        case AF_INET6:
            return sizeof(sockaddr_in6);

        default:
            return 0;
    }
}


/*
 * client::last_update
 */
client::time_point client::last_update(void) const noexcept {
    auto dt = this->_last_update.load(std::memory_order_acquire);
    return clock::time_point(duration(dt));
}


/*
 * client::update
 */
void client::update(void) noexcept {
    this->_last_update.store(
        clock::now().time_since_epoch().count(),
        std::memory_order_release);
}


/*
 * std::less<client>::operator ()
 */
bool std::less<client>::operator ()(_In_ const client& lhs,
        _In_ const client& rhs) const noexcept {
    static const std::less<sockaddr_storage> less;
    return less(lhs._address, rhs._address);
}
