// <copyright file="onexit.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once


namespace MagicMousePad {

namespace Detail {

    template<class T> class OnExitGuard {

    public:

        typedef T exit_handler;

        inline explicit OnExitGuard(exit_handler &&exitHandler)
            : _exitHandler(std::forward<exit_handler>(exitHandler)),
            _isInvoked(false) { }

        OnExitGuard(const OnExitGuard &) = delete;

        inline OnExitGuard(OnExitGuard &&rhs)
            : _exitHandler(std::move(rhs.exitHandler)),
            _isInvoked(rhs._isInvoked) {
            rhs._isInvoked = true;
        }

        inline ~OnExitGuard(void) {
            (*this)();
        }

        inline void cancel(void) noexcept {
            this->_isInvoked = true;
        }

        OnExitGuard &operator =(const OnExitGuard &) = delete;

        OnExitGuard &operator =(OnExitGuard &&rhs);

        inline operator bool(void) const {
            return !this->_isInvoked;
        }

        void operator ()(void) noexcept;

    private:

        exit_handler _exitHandler;
        bool _isInvoked;

    };
}

    template<class T> inline Detail::OnExitGuard<T> OnExit(T &&exitHandler) {
        return Detail::OnExitGuard<T>(std::forward<T>(exitHandler));
    }

}

#include "onexit.inl"
