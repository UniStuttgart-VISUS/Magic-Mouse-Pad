// <copyright file="trace.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * tracer::operator ()
 */
template<class... TArguments>
void tracer::operator ()(_In_z_ const char *const format,
        TArguments&&... arguments) noexcept {
    constexpr const char *const prefix = "[%s:%d] ";
    std::vector<char> buffer(::_scprintf(prefix, this->_file,
        this->_line) + 1);

    if (::sprintf_s(buffer.data(),
            buffer.size(),
            prefix,
            this->_file,
            this->_line) < 0) {
        return;
    }

    const auto offset = ::strlen(buffer.data());
    const auto size = ::_scprintf(format,
        std::forward<TArguments>(arguments)...) + 1;
    buffer.resize(offset + size + 2);

    if (::sprintf_s(buffer.data() + offset,
            size,
            format,
            std::forward<TArguments>(arguments)...) < 0) {
        return;
    }

    ::strcat_s(buffer.data(), buffer.size(), "\r\n");

    ::OutputDebugStringA(buffer.data());
}


/*
 * tracer::operator ()
 */
template<class... TArguments>
void tracer::operator ()(_In_z_ const wchar_t *const format,
        TArguments&&... arguments) noexcept {
    constexpr const wchar_t *const prefix = L"[%hs:%d] ";
    std::vector<wchar_t> buffer(::_scwprintf(prefix, this->_file,
        this->_line) + 1);

    if (::swprintf_s(buffer.data(),
            buffer.size(),
            prefix,
            this->_file,
            this->_line) < 0) {
        return;
    }

    const auto offset = ::wcslen(buffer.data());
    const auto size = ::_scwprintf(format,
        std::forward<TArguments>(arguments)...) + 1;
    buffer.resize(offset + size + 2);

    if (::swprintf_s(buffer.data() + offset,
            size,
            format,
            std::forward<TArguments>(arguments)...) < 0) {
        return;
    }

    ::wcscat_s(buffer.data(), buffer.size(), L"\r\n");

    ::OutputDebugStringW(buffer.data());
}
