// copyright file="CommandLine.cpp" company="Visualisierungsinstitut der Universität Stuttgart" >
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"
#include "CommandLine.h"


/*
 * CommandLine::CommandLine
 */
CommandLine::CommandLine(const TCHAR *cmdLine) : _port(0) {
    ::ZeroMemory(&this->_server, sizeof(this->_server));
}
