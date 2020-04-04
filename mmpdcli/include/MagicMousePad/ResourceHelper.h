// <copyright file="ResourceHelper.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once


namespace MagicMousePad {

    /// <summary>
    /// Provides convenience functions for dealing with Windows resources.
    /// </summary>
    template<class T> class ResourceHelper { };


    /// <summary>
    /// Specialisation for ANSI strings.
    /// </summary>
    template<> class ResourceHelper<char> {

    public:

#if defined(_WIN32)
        /// <summary>
        /// Loads the specified string resource.
        /// </summary>
        static std::string LoadString(const UINT id, HINSTANCE hInstance = NULL);
#endif /* defined(_WIN32) */
    };


    /// <summary>
    /// Specialisation for Unicode strings.
    /// </summary>
    template<> class ResourceHelper<wchar_t> {

    public:

#if defined(_WIN32)
        /// <summary>
        /// Loads the specified string resource.
        /// </summary>
        static std::wstring LoadString(const UINT id, HINSTANCE hInstance = NULL);
#endif /* defined(_WIN32) */
    };
}
