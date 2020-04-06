// copyright file="CommandLine.cpc" company="Visualisierungsinstitut der Universität Stuttgart" >
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "pch.h"
#include "CommandLine.h"


/*
 * CommandLine::CommandLine
 */
CommandLine::CommandLine(const TCHAR *cmdLine) : _addressFamily(AF_INET),
        _port(47596) {
    if (cmdLine == nullptr) {
        throw std::invalid_argument("The command line must not be null "
            "pointer");
    }

    auto args = MagicMousePad::Tokenise(std::basic_string<TCHAR>(cmdLine),
        std::isspace, true);

    {
        auto it = MagicMousePad::FindArgument(_T("/ipversion"), args.begin(),
            args.end());
        if (it != args.end()) {
            switch (::_ttoi(it->c_str())) {
                case 6:
                    this->_addressFamily = AF_INET6;
                    break;

                case 4:
                default:
                    this->_addressFamily = AF_INET;
                    break;
            }
        }
    }

    {
        auto it = MagicMousePad::FindArgument(_T("/port"), args.begin(),
            args.end());
        if (it != args.end()) {
            this->_port = static_cast<std::uint16_t>(::_ttoi(it->c_str()));
        }
    }
}
