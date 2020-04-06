// <copyright file="tokenise.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2018 - 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>



/*
 * MagicMousePad::Tokenise
 */
template<class C, class P>
std::vector<std::basic_string<C>> MagicMousePad::Tokenise(
        const std::basic_string<C> &str, const P &isDelim,
        const bool omitEmpty) {
    typedef typename std::decay<decltype(str)>::type StringType;

    auto s = str.data();
    std::vector<StringType> retval;

    do {
        auto begin = s;

        while ((*s != 0) && !isDelim(*s)) {
            ++s;
        }

        if (!omitEmpty || ((s - begin) > 0)) {
            retval.emplace_back(begin, s);
        }
    } while (*s++ != 0);

    return retval;
}


/*
 * MagicMousePad::Tokenise
 */
template<class C, class I>
std::vector<std::basic_string<C>> MagicMousePad::Tokenise(
        const std::basic_string<C>& str, I beginDelim, I endDelim,
        const bool omitEmpty) {
    return Tokenise(str, [beginDelim, endDelim](const C c) {
        for (auto it = beginDelim; (it != endDelim); ++it) {
            if (*it == c) {
                return true;
            }
        }

        return false;
        });
}


/*
 * MagicMousePad::Tokenise
 */
template<class C>
std::vector<std::basic_string<C>> MagicMousePad::Tokenise(
        const std::basic_string<C>& str, const C delim,
        const bool omitEmpty) {
    return Tokenise(str, [delim](const C c) { return (c == delim); });
}


/*
 * MagicMousePad::Tokenise
 */
template<class C>
std::vector<std::basic_string<C>> MagicMousePad::Tokenise(
        const std::basic_string<C>& str, const std::basic_string<C>& delim,
        const bool omitEmpty) {
    typedef typename std::decay<decltype(str)>::type StringType;

    std::vector<StringType> retval;

    auto cur = static_cast<typename StringType::size_type>(0);
    while (cur != StringType::npos) {
        auto next = str.find(delim, cur);
        if (!omitEmpty || (cur != next)) {
            retval.emplace_back(str.substr(cur, next - cur));
        }
        if (next != StringType::npos) {
            next += delim.size();
        }
        cur = next;
    }

    return retval;
}
