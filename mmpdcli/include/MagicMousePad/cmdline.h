// <copyright file="cmdline.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2018 - 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <string>


namespace MagicMousePad {

    /// <summary>
    /// Find the first occurrence of the specified switch in a range of
    /// command line arguments.
    /// <summary>
    /// <typeparam name="T">The character type.</typeparam>
    /// <typeparam name="I">The type of the string iterator.</typeparam>
    /// <param name="name">The name of the switch to find.</param>
    /// <param name="begin">The begin of the command line.</param>
    /// <param name="end">The end of the command line.</param>
    /// <param name="matchCase">If <c>true</c>, perform a case-sensitive
    /// match. Otherwise (the default), perform a case-insensitive match.
    /// </param>
    /// <returns>An iterator for the switch or <paramref name="end" /> if
    /// the switch was not found.</returns>
    template<class T, class I>
    inline I FindSwitch(const std::basic_string<T>& name, I begin, I end,
            const bool matchCase = false) {
        typedef std::basic_string<T> StringType;

        return std::find_if(begin, end, [&](const StringType& s) {
            if (matchCase) {
                return (name == s);
            } else {
                return ((name.length() == s.length())
                    && std::equal(name.cbegin(), name.cend(), s.cbegin(),
                    [](T l, T r) { return (std::tolower(l) == std::tolower(r)); }));
            }
        });
    }

    /// <summary>
    /// Find the first occurrence of the specified switch in a range of
    /// command line arguments.
    /// <summary>
    /// <typeparam name="T">The character type.</typeparam>
    /// <typeparam name="I">The type of the string iterator.</typeparam>
    /// <param name="name">The name of the switch to find.</param>
    /// <param name="begin">The begin of the command line.</param>
    /// <param name="end">The end of the command line.</param>
    /// <param name="matchCase">If <c>true</c>, perform a case-sensitive
    /// match. Otherwise (the default), perform a case-insensitive match.
    /// </param>
    /// <returns>An iterator for the switch or <paramref name="end" /> if
    /// the switch was not found.</returns>
    template<class T, class I>
    inline I FindSwitch(const T *name, I begin, I end,
            const bool matchCase = false) {
        typedef std::basic_string<T> StringType;
        return FindSwitch(
            (name != nullptr) ? StringType(name) : StringType(),
            begin,
            end,
            matchCase);
    }

    /// <summary>
    /// Answer whether the specified switch is in the range of the given command
    /// line arguments.
    /// <summary>
    /// <typeparam name="T">The character type.</typeparam>
    /// <typeparam name="I">The type of the string iterator.</typeparam>
    /// <param name="name">The name of the switch to find.</param>
    /// <param name="begin">The begin of the command line.</param>
    /// <param name="end">The end of the command line.</param>
    /// <param name="matchCase">If <c>true</c>, perform a case-sensitive
    /// match. Otherwise (the default), perform a case-insensitive match.
    /// </param>
    /// <returns><c>true</c> if the switch was found, <c>false</c> otherwise.
    /// </returns>
    template<class T, class I>
    inline bool ContainsSwitch(const std::basic_string<T>& name, I begin,
            I end, const bool matchCase = false) {
        return (FindSwitch(name, begin, end, matchCase) != end);
    }

    /// <summary>
    /// Answer whether the specified switch is in the range of the given command
    /// line arguments.
    /// <summary>
    /// <typeparam name="T">The character type.</typeparam>
    /// <typeparam name="I">The type of the string iterator.</typeparam>
    /// <param name="name">The name of the switch to find.</param>
    /// <param name="begin">The begin of the command line.</param>
    /// <param name="end">The end of the command line.</param>
    /// <param name="matchCase">If <c>true</c>, perform a case-sensitive
    /// match. Otherwise (the default), perform a case-insensitive match.
    /// </param>
    /// <returns><c>true</c> if the switch was found, <c>false</c> otherwise.
    /// </returns>
    template<class T, class I>
    inline bool ContainsSwitch(const T *name, I begin, I end,
            const bool matchCase = false) {
        return (FindSwitch(name, begin, end, matchCase) != end);
    }

    /// <summary>
    /// Find the first occurrence of an argument with the specified switch name
    /// in a range of command line arguments.
    /// </summary>
    /// <typeparam name="T">The character type.</typeparam>
    /// <typeparam name="I">The type of the string iterator.</typeparam>
    /// <param name="name">The name of the switch to find.</param>
    /// <param name="begin">The begin of the command line.</param>
    /// <param name="end">The end of the command line.</param>
    /// <param name="matchCase">If <c>true</c>, perform a case-sensitive
    /// match. Otherwise (the default), perform a case-insensitive match.
    /// </param>
    /// <returns>An iterator of the first element after the switch ifself, or
    /// <paramref name="end" /> if the respective argument has not been found.
    /// </returns>
    template<class T, class I>
    inline I FindArgument(const std::basic_string<T>& name, I begin, I end,
            const bool matchCase = false) {
        auto retval = FindSwitch(name, begin, end, matchCase);
        return (retval != end) ? ++retval : retval;
    }

    /// <summary>
    /// Find the first occurrence of an argument with the specified switch name
    /// in a range of command line arguments.
    /// </summary>
    /// <typeparam name="T">The character type.</typeparam>
    /// <typeparam name="I">The type of the string iterator.</typeparam>
    /// <param name="name">The name of the switch to find.</param>
    /// <param name="begin">The begin of the command line.</param>
    /// <param name="end">The end of the command line.</param>
    /// <param name="matchCase">If <c>true</c>, perform a case-sensitive
    /// match. Otherwise (the default), perform a case-insensitive match.
    /// </param>
    /// <returns>An iterator of the first element after the switch ifself, or
    /// <paramref name="end" /> if the respective argument has not been found.
    /// </returns>
    template<class T, class I>
    inline I FindArgument(const T *name, I begin, I end,
            const bool matchCase = false) {
        auto retval = FindSwitch(name, begin, end, matchCase);
        return (retval != end) ? ++retval : retval;
    }

}
