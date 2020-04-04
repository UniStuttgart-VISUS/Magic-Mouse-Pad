// <copyright file="ResourceHelper.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include <string>
#include <system_error>

#include <Windows.h>

#include "ResourceHelper.h"


#if defined(_WIN32)
/*
 * MagicMousePad::ResourceHelper<char>::LoadString
 */
std::string MagicMousePad::ResourceHelper<char>::LoadString(const UINT id,
        HINSTANCE hInstance) {
    char *ptr;
    auto cnt = ::LoadStringA(hInstance, id, reinterpret_cast<LPSTR>(&ptr), 0);

    auto error = ::GetLastError();
    if ((cnt == 0) && (error != NO_ERROR)) {
        throw std::system_error(error, std::system_category(),
            "Failed to retrieve string resource");
    }

    return std::string(ptr, ptr + cnt);
}
#endif /* defined(_WIN32) */


#if defined(_WIN32)
/*
 * MagicMousePad::ResourceHelper<wchar_t>::LoadString
 */
std::wstring MagicMousePad::ResourceHelper<wchar_t>::LoadString(const UINT id,
        HINSTANCE hInstance) {
    wchar_t *ptr;
    auto cnt = ::LoadStringW(hInstance, id, reinterpret_cast<LPWSTR>(&ptr), 0);

    auto error = ::GetLastError();
    if ((cnt == 0) && (error != NO_ERROR)) {
        throw std::system_error(error, std::system_category(),
            "Failed to retrieve string resource");
    }

    return std::wstring(ptr, ptr + cnt);
}
#endif /* defined(_WIN32) */
