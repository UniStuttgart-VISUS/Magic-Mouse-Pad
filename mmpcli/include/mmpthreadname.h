// <copyright file="mmpthreadname.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include <inttypes.h>

#include "mmpapi.h"


/// <summary>
/// Sets the debugger name of the thread with the specified ID.
/// </summary>
/// <param name="thread_id">The ID of the thread to set the ID of or -1 for
/// the calling thread.</param>
/// <param name="thread_name">The name of the thread. If this is
/// <see langword="nullptr" />, the function does nothing.</param>
void MMPCLI_API mmp_set_thread_name(_In_ const uint32_t thread_id,
    _In_opt_z_ const char *thread_name);
