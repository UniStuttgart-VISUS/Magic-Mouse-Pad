// <copyright file="tokenise.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2018 - 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <string>
#include <vector>


namespace MagicMousePad {

    /// <summary>
    /// Splits a string at the characters that satisfy the delimiter predicate
    /// <paramref name="isDelim" /> and returns a vector of the tokens.
    /// </summary>
    /// <typeparam name="C">The character type.</typeparam>
    /// <typeparam name="P">A predicate that checks for a single character
    /// whether it is a delimiter or not.</typeparam>
    /// <param name="str">The string to be split.</param>
    /// <param name="isDelim">The predicate checking a character whether it is
    /// a delimiter.</param>
    /// <param name="omitEmpty">If <c>true</c>, removes empty tokens. This
    /// defaults to <c>false</c>.</param>
    /// <returns>A vector holding the tokens (or the whole string if no
    /// occurrence of the delimiter characters was found).</returns>
    template<class C, class P>
    std::vector<std::basic_string<C>> Tokenise(const std::basic_string<C> &str,
        const P& isDelim, const bool omitEmpty = false);

    /// <summary>
    /// Splits a string at the given delimiters <paramref name="beginDelim" />
    /// to <paramref name="endDelim" /> and returns a vector of the tokens.
    /// </summary>
    /// <typeparam name="C">The character type.</typeparam>
    /// <typeparam name="I">An iterator over <typeparamref name="C" />.
    /// </typeparam>
    /// <param name="str">The string to be split.</param>
    /// <param name="beginDelim">The begin of the range of delimiter
    /// characters.</param>
    /// <param name="endDelim">The end of the range of delimiter characters.
    /// </param>
    /// <param name="omitEmpty">If <c>true</c>, removes empty tokens. This
    /// defaults to <c>false</c>.</param>
    /// <returns>A vector holding the tokens (or the whole string if no
    /// occurrence of the delimiter characters was found).</returns>
    template<class C, class I>
    std::vector<std::basic_string<C>> Tokenise(const std::basic_string<C>& str,
        I beginDelim, I endDelim, const bool omitEmpty = false);

    /// <summary>
    /// Splits a string at the given separator 'sep' and returns a vector of the
    /// tokens.
    /// </summary>
    /// <typeparam name="C">The character type.</typeparam>
    /// <param name="str">The string to be split.</param>
    /// <param name="delim">The delimiter.</param>
    /// <param name="omitEmpty">If <c>true</c>, removes empty tokens. This
    /// defaults to <c>false</c>.</param>
    /// <returns>A vector holding the tokens (or the whole string if no
    /// occurrence of the delimiter characters was found).</returns>
    template<class C>
    std::vector<std::basic_string<C>> Tokenise(const std::basic_string<C>& str,
        const C delim, const bool omitEmpty = false);

    /// <summary>
    /// Splits a string at the given separator 'sep' and returns a vector of the
    /// tokens.
    /// </summary>
    /// <typeparam name="C">The character type.</typeparam>
    /// <param name="str">The string to be split.</param>
    /// <param name="delim">The delimiter.</param>
    /// <param name="omitEmpty">If <c>true</c>, removes empty tokens. This
    /// defaults to <c>false</c>.</param>
    /// <returns>A vector holding the tokens (or the whole string if no
    /// occurrence of the delimiter characters was found).</returns>
    template<class C>
    std::vector<std::basic_string<C>> Tokenise(const std::basic_string<C>& str,
        const std::basic_string<C>& delim, const bool omitEmpty = false);

}

#include "MagicMousePad/tokenise.inl"

