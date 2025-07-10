// <copyright file="mmpcli.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "mmpcli.h"

#include "mmp_client.h"
#include "trace.h"


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
 * ::mmp_configure_server4
 */
_Success_(return == 0) int mmp_configure_server4(
        _In_ mmp_configuration *configuration,
        _In_ const sockaddr_in *server) {
    if (configuration == nullptr) {
        MMP_TRACE("The client configuration is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    if (server == nullptr) {
        MMP_TRACE("The server address is invalid.");
        return ERROR_INVALID_PARAMETER;
    }

    ::memcpy(&configuration->address, server, sizeof(sockaddr_in));

    // Coerce the address family to IPv4.
    configuration->address.ss_family = AF_INET;

    return 0;
}


/*
 * mmp_configure_server6
 */
_Success_(return == 0) int mmp_configure_server6(
        _In_ mmp_configuration *configuration,
        _In_ const sockaddr_in6 *server) {
    if (configuration == nullptr) {
        MMP_TRACE("The client configuration is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    if (server == nullptr) {
        MMP_TRACE("The server address is invalid.");
        return ERROR_INVALID_PARAMETER;
    }

    ::memcpy(&configuration->address, server, sizeof(sockaddr_in6));

    // Coerce the address family to IPv6.
    configuration->address.ss_family = AF_INET6;

    return 0;
}


/*
 * ::mmp_connect
 */
_Success_(return == 0) int mmp_connect(
        _Out_ mmp_handle *handle,
        _In_ const mmp_configuration *configuration) {
    if (handle == nullptr) {
        MMP_TRACE("The output parameter for the handle is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    if (configuration == nullptr) {
        MMP_TRACE("The client configuration is invalid.");
        return ERROR_INVALID_PARAMETER;
    }

    if ((*handle = new (std::nothrow) mmp_client(*configuration)) == nullptr) {
        MMP_TRACE("Insufficient memory to allocate mmp_client.");
        return ERROR_OUTOFMEMORY;
    }

    // Optionally performs discovery of the server address of the mouse pad.
    {
        auto retval = mmp_client::discover(**handle);
        if (retval != 0) {
            return retval;
        }
    }



    throw "TODO";
    return 0;
}


/*
 * ::mmp_disconnect
 */
_Success_(return == 0) MMPCLI_API int mmp_disconnect(_In_ mmp_handle handle) {
    if (handle == nullptr) {
        MMP_TRACE("The handle provided to mmp_disconnect is invalid.");
        return ERROR_INVALID_PARAMETER;
    }

    delete handle;
    return 0;
}


#if defined(__cplusplus)
/*
 * visus::mmp::detail::delete_mmp_handle::operator ()
 */
void visus::mmp::detail::delete_mmp_handle::operator ()(
        _In_opt_ mmp_handle handle) const noexcept {
    if (handle != nullptr) {
        ::mmp_disconnect(handle);
    }
}
#endif /* defined(__cplusplus) */
