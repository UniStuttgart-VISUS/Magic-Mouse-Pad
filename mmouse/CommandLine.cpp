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

    auto args = MagicMousePad::Tokenise(std::basic_string<TCHAR>(cmdLine),
        std::isspace, true);

    {
        auto it = MagicMousePad::FindArgument(_T("/port"), args.begin(),
            args.end());
        if (it != args.end()) {
            this->_port = static_cast<std::uint16_t>(::_ttoi(it->c_str()));
        }
    }

    {
        auto it = MagicMousePad::FindArgument(_T("/server"), args.begin(),
            args.end());
        if (it != args.end()) {
            this->_server = MagicMousePad::ParseEndPoint(it->c_str());
        }
    }
}
