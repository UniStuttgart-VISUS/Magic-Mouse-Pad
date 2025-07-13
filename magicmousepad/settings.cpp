// <copyright file="settings.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "settings.h"

#include <wil/registry.h>
#include <wil/result.h>

/*
 * settings::settings
 */
settings::settings(void) noexcept : _height(0), _width(0) {
    ::memset(&this->_address, 0, sizeof(this->_address));
    this->_address.ss_family = AF_INET;
}


/*
 * settings::address_length
 */
int settings::address_length(void) const noexcept {
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
 * settings::load
 */
void settings::load(_In_ HKEY key) {
    THROW_WIN32_IF(ERROR_INVALID_HANDLE, !key);

    const auto get_uint = [key](LPCWSTR name, uint32_t& dst) {
        DWORD v;
        auto r = SUCCEEDED(wil::reg::get_value_dword_nothrow(key, name, &v));

        if (r) {
            dst = v;
        }

        return r;
    };

    {
        std::uint32_t af;
        if (get_uint(L"AddressFamily", af)) {
            ::memset(&this->_address, 0, sizeof(this->_address));
            this->_address.ss_family = af;
        }
    }

    {
        std::uint32_t port;
        if (get_uint(L"Port", port)) {
            switch (this->_address.ss_family) {
                case AF_INET: {
                    auto& a = reinterpret_cast<sockaddr_in&>(this->_address);
                    a.sin_port = ::htons(port);
                    } break;

                case AF_INET6: {
                    auto& a = reinterpret_cast<sockaddr_in6&>(this->_address);
                    a.sin6_port = ::htons(port);
                    } break;
            }
        }
    }

    get_uint(L"Height", this->_height);
    get_uint(L"Width", this->_width);
}


/*
 * nlohmann::adl_serializer<settings>::to_json
 */
nlohmann::json nlohmann::adl_serializer<settings>::to_json(
        _In_ const settings& value) noexcept {
    auto retval = nlohmann::json::object();

    switch (value._address.ss_family) {
        case AF_INET: {
            auto a = reinterpret_cast<const sockaddr_in&>(value._address);
            retval["Address"] = nlohmann::json::object();
            retval["Address"]["Family"] = 4;
            retval["Address"]["Port"] = ::ntohs(a.sin_port);
            } break;

        case AF_INET6: {
            auto a = reinterpret_cast<const sockaddr_in6&>(value._address);
            retval["Address"] = nlohmann::json::object();
            retval["Address"]["Family"] = 6;
            retval["Address"]["Port"] = ::ntohs(a.sin6_port);
            } break;
    }

    retval["Height"] = value._height;
    retval["Width"] = value._width;

    return retval;
}


/*
 * nlohmann::adl_serializer<settings>::from_json
 */
settings nlohmann::adl_serializer<settings>::from_json(
        _In_ const nlohmann::json& json) noexcept {
    settings retval;

    {
        auto it = json.find("Address");
        if (it != json.end()) {
            ::memset(&retval._address, 0, sizeof(retval._address));

            const auto f = it->find("Family");
            const auto ipv6 = ((f != it->end()) && (f->get<int>() == 6));

            auto p = it->find("Port");
            if (p != it->end()) {
                auto port = p->get<std::uint16_t>();

                if (ipv6) {
                    auto& a = reinterpret_cast<sockaddr_in6&>(retval._address);
                    a.sin6_family = AF_INET6;
                    a.sin6_addr = in6addr_any;
                    a.sin6_port = ::htons(port);

                } else {
                    auto& a = reinterpret_cast<sockaddr_in&>(retval._address);
                    a.sin_family = AF_INET;
                    a.sin_addr.s_addr = INADDR_ANY;
                    a.sin_port = ::htons(port);
                }
            }
        } /* if (it != json.end()) */
    }

    {
        auto it = json.find("Height");
        retval._height = (it != json.end()) ? it->get<std::uint32_t>() : 0;
    }

    {
        auto it = json.find("Width");
        retval._width = (it != json.end()) ? it->get<std::uint32_t>() : 0;
    }

    return retval;
}


/*
 * std::less<sockaddr_storage>::operator ()
 */
bool std::less<sockaddr_storage>::operator ()(_In_ const sockaddr_storage& lhs,
        _In_ const sockaddr_storage& rhs) const noexcept {
    const auto dp = lhs.ss_family - rhs.ss_family;

    if (dp < 0) {
        return true;

    } else if (dp > 0) {
        return false;

    } else {
        return (::memcmp(&lhs, &rhs, sizeof(sockaddr_storage)) < 0);
    }
}
