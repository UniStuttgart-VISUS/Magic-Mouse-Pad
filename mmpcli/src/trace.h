// <copyright file="trace.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <cstdio>
#include <vector>

#if defined(_WIN32)
#include <Windows.h>
#endif /* defined(_WIN32) */


/// <summary>
/// Implements a debug tracer for the library.
/// </summary>
class tracer final {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="file">The file where the trace was written.</param>
    /// <param name="line">The line where the trace was written.</param>
    inline tracer(_In_z_ const char *const file, _In_ int line) noexcept
        : _file(file), _line(line) {}

    /// <summary>
    /// Outputs a formatted trace message to the debugger console.
    /// </summary>
    /// <typeparam name="TArguments"></typeparam>
    /// <param name="format"></param>
    /// <param name="arguments"></param>
    template<class... TArguments>
    void operator ()(_In_z_ const char *const format,
        TArguments&&... arguments) noexcept;

    /// <summary>
    /// Outputs a formatted trace message to the debugger console.
    /// </summary>
    /// <typeparam name="TArguments"></typeparam>
    /// <param name="format"></param>
    /// <param name="arguments"></param>
    template<class... TArguments>
    void operator ()(_In_z_ const wchar_t *const format,
        TArguments&&... arguments) noexcept;

private:

    const char *const _file;
    const int _line;
};

#include "trace.inl"


#if (defined(_DEBUG) || defined(DEBUG))
#define MMP_TRACE tracer(__FILE__, __LINE__)
#else /* (defined(_DEBUG) || defined(DEBUG))*/
#define MMP_TRACE __noop
#endif /* (defined(_DEBUG) || defined(DEBUG))*/
