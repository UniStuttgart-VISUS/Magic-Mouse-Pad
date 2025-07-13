// <copyright file="mmp_configuration.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "mmp_configuration.h"

#include <wil/registry.h>
#include <wil/result.h>

#include "mmpendpoint.h"
#include "mmptrace.h"


/*
 * ::mmp_configure_client4
 */
_Success_(return == 0) int mmp_configure_client4(
        _In_ mmp_configuration *configuration,
        _In_ const struct sockaddr_in *client) {
    if (configuration == nullptr) {
        MMP_TRACE("The client configuration is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    if (client == nullptr) {
        MMP_TRACE("The client address is invalid.");
        return ERROR_INVALID_PARAMETER;
    }

    ::memcpy(&configuration->client, client, sizeof(*client));

    // Coerce the address family to IPv4.
    configuration->client.ss_family = AF_INET;

    return 0;
}


/*
 * ::mmp_configure_client6
 */
_Success_(return == 0) int mmp_configure_client6(
        _In_ mmp_configuration *configuration,
        _In_ const struct sockaddr_in6 *client) {
    if (configuration == nullptr) {
        MMP_TRACE("The client configuration is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    if (client == nullptr) {
        MMP_TRACE("The client address is invalid.");
        return ERROR_INVALID_PARAMETER;
    }

    ::memcpy(&configuration->client, client, sizeof(*client));

    // Coerce the address family to IPv6.
    configuration->client.ss_family = AF_INET6;

    return 0;
}


/*
 * ::mmp_configure_discovery
 */
_Success_(return == 0) int mmp_configure_discovery(
        _In_ mmp_configuration *configuration,
        _In_ const uint16_t port,
        _In_ const uint32_t timeout,
        _In_ const uint32_t rate_limit) {
    if (configuration == nullptr) {
        MMP_TRACE("The client configuration is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    if (port == 0) {
        MMP_TRACE("The port for the discovery is invalid.");
        return ERROR_INVALID_PARAMETER;
    }

    configuration->timeout = timeout;
    configuration->rate_limit = rate_limit;

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = ::htons(port);

    return ::mmp_configure_server4(configuration, &address);
}


/*
 * ::mmp_configure_from_registry_key
 */
_Success_(return == 0) int mmp_configure_from_registry_key(
        _In_ mmp_configuration * configuration,
        _In_ HKEY key) {
    if (configuration == nullptr) {
        MMP_TRACE("The client configuration is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    if (key == NULL) {
        MMP_TRACE("The registry key provided is invalid.");
        return ERROR_INVALID_PARAMETER;
    }

    const auto get_int = [key](LPCWSTR name, int32_t& dst) {
        DWORD value;
        if (SUCCEEDED(wil::reg::get_value_dword_nothrow(key, name, &value))) {
            dst = value;
        }
    };

    const auto get_uint = [key](LPCWSTR name, uint32_t& dst) {
        DWORD value;
        if (SUCCEEDED(wil::reg::get_value_dword_nothrow(key, name, &value))) {
            dst = value;
        }
    };

    {
        wil::unique_cotaskmem_string value;
        if (SUCCEEDED(wil::reg::get_value_string_nothrow(key,
                L"Server",
                value))) {
            RETURN_IF_WIN32_ERROR(::mmp_configure_serverw(
                configuration, value.get()));
        }
    }

    get_uint(L"Flags", configuration->flags);
    get_uint(L"Height", configuration->height);
    get_int(L"OffsetX", configuration->offset_x);
    get_int(L"OffsetY", configuration->offset_y);
    get_uint(L"RateLimit", configuration->rate_limit);
    get_int(L"StartX", configuration->offset_x);
    get_int(L"StartY", configuration->offset_y);
    get_uint(L"Timeout", configuration->timeout);
    get_uint(L"Width", configuration->width);

    return 0;
}


/*
 * ::mmp_configure_server4
 */
_Success_(return == 0) int mmp_configure_server4(
        _In_ mmp_configuration *configuration,
        _In_ const struct sockaddr_in *server) {
    if (configuration == nullptr) {
        MMP_TRACE("The client configuration is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    if (server == nullptr) {
        MMP_TRACE("The server address is invalid.");
        return ERROR_INVALID_PARAMETER;
    }

    ::memcpy(&configuration->server, server, sizeof(*server));

    // Coerce the address family to IPv4.
    configuration->server.ss_family = AF_INET;

    return 0;
}


/*
 * mmp_configure_server6
 */
_Success_(return == 0) int mmp_configure_server6(
        _In_ mmp_configuration *configuration,
        _In_ const struct sockaddr_in6 *server) {
    if (configuration == nullptr) {
        MMP_TRACE("The client configuration is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    if (server == nullptr) {
        MMP_TRACE("The server address is invalid.");
        return ERROR_INVALID_PARAMETER;
    }

    ::memcpy(&configuration->server, server, sizeof(*server));

    // Coerce the address family to IPv6.
    configuration->server.ss_family = AF_INET6;

    return 0;
}


/*
 * ::mmp_configure_servera
 */
_Success_(return == 0) int mmp_configure_servera(
        _In_ mmp_configuration *configuration,
        _In_z_ const char *server) {
    if (configuration == nullptr) {
        MMP_TRACE("The client configuration is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    return ::mmp_parse_end_pointa(&configuration->server, server);
}


/*
 * ::mmp_configure_serverw
 */
_Success_(return == 0) int mmp_configure_serverw(
        _In_ mmp_configuration *configuration,
        _In_z_ const wchar_t *server) {
    if (configuration == nullptr) {
        MMP_TRACE("The client configuration is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    return ::mmp_parse_end_pointw(&configuration->server, server);
}
