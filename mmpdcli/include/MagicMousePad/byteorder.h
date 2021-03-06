// <copyright file="byteorder.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once

#if defined(_WIN32)
#include <WinSock2.h>
#endif /* defined(_WIN32) */


namespace MagicMousePad {

    /// <summary>
    /// Convert <paramref name="n" /> from network byte order to host byte order.
    /// </summary>
    inline std::uint8_t ToHostOrder(const std::uint8_t n) {
        return n;
    }

    /// <summary>
    /// Convert <paramref name="n" /> from network byte order to host byte order.
    /// </summary>
    inline std::int8_t ToHostOrder(const std::int8_t n) {
        return n;
    }

    /// <summary>
    /// Convert <paramref name="n" /> from network byte order to host byte order.
    /// </summary>
    inline std::uint16_t ToHostOrder(const std::uint16_t n) {
        return ::ntohs(n);
    }

    /// <summary>
    /// Convert <paramref name="n" /> from network byte order to host byte order.
    /// </summary>
    inline std::int16_t ToHostOrder(const std::int16_t n) {
        auto m = *reinterpret_cast<const std::uint16_t *>(&n);
        m = ToHostOrder(m);
        return *reinterpret_cast<std::int16_t *>(&m);
    }

    /// <summary>
    /// Convert <paramref name="n" /> from network byte order to host byte order.
    /// </summary>
    inline std::uint32_t ToHostOrder(const std::uint32_t n) {
        return ::ntohl(n);
    }

    /// <summary>
    /// Convert <paramref name="n" /> from network byte order to host byte order.
    /// </summary>
    inline std::int32_t ToHostOrder(const std::int32_t n) {
        auto m = *reinterpret_cast<const std::uint32_t *>(&n);
        m = ToHostOrder(m);
        return *reinterpret_cast<std::int32_t *>(&m);
    }

    /// <summary>
    /// Convert <paramref name="n" /> from network byte order to host byte order.
    /// </summary>
    inline std::uint64_t ToHostOrder(const std::uint64_t n) {
#if defined(_WIN32)
        return ::ntohll(n);
#else /* defined(_WIN32) */
        return ::htobe64(n);
#endif /* defined(_WIN32) */
    }

    /// <summary>
    /// Convert <paramref name="n" /> from network byte order to host byte order.
    /// </summary>
    inline std::int64_t ToHostOrder(const std::int64_t n) {
        auto m = *reinterpret_cast<const std::uint64_t *>(&n);
        m = ToHostOrder(m);
        return *reinterpret_cast<std::int64_t *>(&m);
    }

    /// <summary>
    /// Convert <paramref name="n" /> from host byte order to network byte order.
    /// </summary>
    inline std::uint8_t ToNetworkOrder(const std::uint8_t n) {
        return n;
    }

    /// <summary>
    /// Convert <paramref name="n" /> from host byte order to network byte order.
    /// </summary>
    inline std::int8_t ToNetworkOrder(const std::int8_t n) {
        return n;
    }

    /// <summary>
    /// Convert <paramref name="n" /> from host byte order to network byte order.
    /// </summary>
    inline std::uint16_t ToNetworkOrder(const std::uint16_t n) {
        return ::htons(n);
    }

    /// <summary>
    /// Convert <paramref name="n" /> from host byte order to network byte order.
    /// </summary>
    inline std::int16_t ToNetworkOrder(const std::int16_t n) {
        auto m = *reinterpret_cast<const std::uint16_t *>(&n);
        m = ToHostOrder(m);
        return *reinterpret_cast<std::int16_t *>(&m);
    }

    /// <summary>
    /// Convert <paramref name="n" /> from host byte order to network byte order.
    /// </summary>
    inline std::uint32_t ToNetworkOrder(const std::uint32_t n) {
        return ::htonl(n);
    }

    /// <summary>
    /// Convert <paramref name="n" /> from host byte order to network byte order.
    /// </summary>
    inline std::int32_t ToNetworkOrder(const std::int32_t n) {
        auto m = *reinterpret_cast<const std::uint32_t *>(&n);
        m = ToHostOrder(m);
        return *reinterpret_cast<std::int32_t *>(&m);
    }

    /// <summary>
    /// Convert <paramref name="n" /> from host byte order to network byte order.
    /// </summary>
    inline std::uint64_t ToNetworkOrder(const std::uint64_t n) {
#if defined(_WIN32)
        return ::htonll(n);
#else /* defined(_WIN32) */
        return ::be64toh(n);
#endif /* defined(_WIN32) */
    }

    /// <summary>
    /// Convert <paramref name="n" /> from host byte order to network byte order.
    /// </summary>
    inline std::int64_t ToNetworkOrder(const std::int64_t n) {
        auto m = *reinterpret_cast<const std::uint64_t *>(&n);
        m = ToHostOrder(m);
        return *reinterpret_cast<std::int64_t *>(&m);
    }
}
