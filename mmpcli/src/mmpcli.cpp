// <copyright file="mmpcli.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "mmpcli.h"

#include "mmp_client.h"
#include "trace.h"


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
