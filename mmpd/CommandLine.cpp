// copyright file="CommandLine.cpc" company="Visualisierungsinstitut der Universit�t Stuttgart" >
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#include "pch.h"
#include "CommandLine.h"


/*
 * CommandLine::CommandLine
 */
CommandLine::CommandLine(const TCHAR *cmdLine) : _addressFamily(AF_INET),
    _port(47596) { }
