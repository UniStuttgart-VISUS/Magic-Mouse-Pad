// copyright file="Client.h" company="Visualisierungsinstitut der Universit�t Stuttgart" >
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once


/// <summary>
/// Holds the subscription information of a single client.
/// </summary>
class Client {

public:

    /// <summary>
    /// Initialises a new instance with the specified end point.
    /// </summary>
    Client(const struct sockaddr *address, const int addressLength);

    /// <summary>
    /// Applies any optional clipping to the given coordinates.
    /// </summary>
    void ClipPosition(std::int32_t& x, std::int32_t& y) const;

    /// <summary>
    /// Gets the end point address of the client.
    /// </summary>
    inline const struct sockaddr *GetAddress(void) const {
        return reinterpret_cast<const struct sockaddr *>(&this->_address);
    }

    /// <summary>
    /// Gets the length of the end point address in bytes.
    /// </summary>
    inline int GetAddressLength(void) const {
        return this->_addressLength;
    }

    /// <summary>
    /// Answer whether the client has the specified address.
    /// </summary>
    bool IsAddress(const struct sockaddr& address,
        const int addressLength) const;

    /// <summary>
    /// Sets the clipping area to the value reported by the client.
    /// </summary>
    void SetClippingArea(const MagicMousePad::SubscriptionMessage& msg);

private:

    struct sockaddr_storage _address;
    int _addressLength;
    RECT _clippingArea;
};
