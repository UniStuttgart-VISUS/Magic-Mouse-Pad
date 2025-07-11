// <copyright file="mmpcli.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "mmpcli.h"

#include "mmp_client.h"
#include "mmpmsg.h"
#include "trace.h"


/*
 * ::mmp_connect
 */
_Success_(return == 0) int mmp_connect(
        _Out_ mmp_handle *handle,
        _In_ mmp_configuration *configuration) {
    if (handle == nullptr) {
        MMP_TRACE("The output parameter for the handle is invalid.");
        return ERROR_INVALID_PARAMETER;
    }
    if (configuration == nullptr) {
        MMP_TRACE("The client configuration is invalid.");
        return ERROR_INVALID_PARAMETER;
    }

    // Optionally performs discovery of the server address of the mouse pad.
    RETURN_IF_WIN32_ERROR(mmp_client::discover(*configuration));

    // The client will connect in its constructor. Note that we must catch here,
    // because the constructor could fail with a socket error besides the
    // potential out-of-memory scenario.
    try {
        MMP_TRACE(L"Allocating the magic mouse pad client context.");
        *handle = new mmp_client(*configuration);
        return 0;

    } catch (std::bad_alloc&) {
        MMP_TRACE(L"Insufficient memory to allocate client context.");
        return ERROR_OUTOFMEMORY;

    } catch (const std::exception& ex) {
        MMP_TRACE("Failed to create client context: %hs", ex.what());
        return ERROR_INVALID_PARAMETER;
    }
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
