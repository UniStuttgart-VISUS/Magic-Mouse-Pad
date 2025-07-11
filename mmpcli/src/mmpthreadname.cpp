// <copyright file="mmpthreadname.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "mmpthreadname.h"


/*
 * ::mmp_set_thread_name
 */
void mmp_set_thread_name(_In_ const uint32_t thread_id,
        _In_opt_z_ const char *thread_name) {
    // See https://msdn.microsoft.com/de-de/library/xcb2z8hs.aspx?f=255&MSPPError=-2147217396
    constexpr DWORD MS_VC_EXCEPTION = 0x406D1388;

    if (thread_name != nullptr) {
        struct {
            DWORD dwType;       // Must be 0x1000.
            LPCSTR szName;      // Pointer to name (in user addr space).
            DWORD dwThreadID;   // Thread ID (-1=caller thread).
            DWORD dwFlags;      // Reserved for future use, must be zero.
        } info;
        info.dwType = 0x1000;
        info.szName = thread_name;
        info.dwThreadID = thread_id;
        info.dwFlags = 0;

#pragma warning(push)
#pragma warning(disable: 6320 6322)
        __try {
            ::RaiseException(MS_VC_EXCEPTION, 0,
                sizeof(info) / sizeof(ULONG_PTR),
                reinterpret_cast<ULONG_PTR *>(&info));
        } __except (EXCEPTION_EXECUTE_HANDLER) { }
#pragma warning(pop)
    }
}
