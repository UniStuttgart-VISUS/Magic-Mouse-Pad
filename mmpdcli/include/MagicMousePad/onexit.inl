// <copyright file="onexit.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>


/*
 * MagicMousePad::Detail::OnExitGuard<T>::operator =
 */
template<class T>
MagicMousePad::Detail::OnExitGuard<T>&
MagicMousePad::Detail::OnExitGuard<T>::operator =(OnExitGuard &&rhs) {
    if (this != std::addressof(rhs)) {
        this->_exitHandler = std::move(rhs._exitHandler);
        this->_isInvoke = rhs._isInvoke;
        rhs._isInvoked = true;
    }

    return *this;
}


/*
 * MagicMousePad::Detail::OnExitGuard<T>::operator ()
 */
template<class T>
void MagicMousePad::Detail::OnExitGuard<T>::operator ()(void) noexcept {
    if (*this) {
        this->_exitHandler();
        this->_isInvoked = true;
    }
}
