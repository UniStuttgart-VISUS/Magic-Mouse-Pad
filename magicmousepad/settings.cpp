// <copyright file="settings.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "settings.h"


/*
 * settings::settings
 */
settings::settings(void) noexcept : _height(0), _width(0) {
    ::memset(&this->_address, 0, sizeof(this->_address));
}


/*
 * nlohmann::adl_serializer<settings>::to_json
 */
nlohmann::json nlohmann::adl_serializer<settings>::to_json(
        _In_ const settings& value) noexcept {
    auto retval = nlohmann::json::object();

    switch (value.address().ss_family) {
        case AF_INET: {
            auto a = reinterpret_cast<const sockaddr_in&>(value._address);
            retval["address"] = nlohmann::json::object();
            retval["address"]["family"] = 4;
            retval["address"]["port"] = ::ntohs(a.sin_port);
            } break;

        case AF_INET6: {
            auto a = reinterpret_cast<const sockaddr_in6&>(value._address);
            retval["address"] = nlohmann::json::object();
            retval["address"]["family"] = 6;
            retval["address"]["port"] = ::ntohs(a.sin6_port);
            } break;
    }

    retval["height"] = value._height;
    retval["width"] = value._width;

    return retval;
}


/*
 * nlohmann::adl_serializer<settings>::from_json
 */
settings nlohmann::adl_serializer<settings>::from_json(
        _In_ const nlohmann::json& json) noexcept {
    settings retval;

    {
        auto it = json.find("address");
        if (it != json.end()) {
            ::memset(&retval._address, 0, sizeof(retval._address));

            const auto f = it->find("family");
            const auto ipv6 = ((f != it->end()) && (f->get<int>() == 6));

            auto p = it->find("port");
            if (p != it->end()) {
                if (ipv6) {
                    auto a = reinterpret_cast<const sockaddr_in6 &>(
                        retval._address);
                    a.sin6_family = AF_INET6;
                    a.sin6_addr = in6addr_any;
                    a.sin6_port = ::htons(p->get<std::uint16_t>());

                } else {
                    auto a = reinterpret_cast<const sockaddr_in&>(
                        retval._address);
                    a.sin_family = AF_INET;
                    a.sin_addr.s_addr = INADDR_ANY;
                    a.sin_port = ::htons(p->get<std::uint16_t>());
                }
            }
        } /* if (it != json.end()) */
    }

    {
        auto it = json.find("height");
        retval._height = (it != json.end()) ? it->get<std::size_t>() : 0;
    }

    {
        auto it = json.find("width");
        retval._width = (it != json.end()) ? it->get<std::size_t>() : 0;
    }

    return retval;
}
