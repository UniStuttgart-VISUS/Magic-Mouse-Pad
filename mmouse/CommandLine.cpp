// copyright file="CommandLine.cpp" company="Visualisierungsinstitut der Universit�t Stuttgart" >
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#include "pch.h"
#include "CommandLine.h"


/*
 * CommandLine::CommandLine
 */
CommandLine::CommandLine(const TCHAR *cmdLine) : _port(0) {
    ::ZeroMemory(&this->_server, sizeof(this->_server));
}
