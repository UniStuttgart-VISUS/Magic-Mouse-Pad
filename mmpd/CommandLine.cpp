// copyright file="CommandLine.cpp" company="Visualisierungsinstitut der Universität Stuttgart" >
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
    static const TCHAR Delims[] = _T(",;");
    if (cmdLine == nullptr) {
        throw std::invalid_argument("The command line must not be null "
            "pointer");
    }

    this->_bounds.left = std::numeric_limits<LONG>::lowest();
    this->_bounds.top = std::numeric_limits<LONG>::lowest();
    this->_bounds.right = (std::numeric_limits<LONG>::max)();
    this->_bounds.bottom = (std::numeric_limits<LONG>::max)();

    this->_startPoint.x = this->_startPoint.y = 0;

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

    {
        auto it = MagicMousePad::FindArgument(_T("/bounds"), args.begin(),
            args.end());
        if (it != args.end()) {
            auto bounds = MagicMousePad::Tokenise(*it, Delims, Delims
                + std::size(Delims), true);
            if (bounds.size() == 2) {
                this->_bounds.left = this->_bounds.top = 0;
                this->_bounds.right = ::_ttoi(bounds[0].c_str());
                this->_bounds.bottom = ::_ttoi(bounds[1].c_str());

            } else if (bounds.size() == 4) {
                this->_bounds.left = ::_ttoi(bounds[0].c_str());
                this->_bounds.top = ::_ttoi(bounds[1].c_str());
                this->_bounds.right = ::_ttoi(bounds[2].c_str());
                this->_bounds.bottom = ::_ttoi(bounds[3].c_str());

            } else {
                throw std::invalid_argument("The /bounds parameter must "
                    "either specify width and height or the left, top, "
                    "right and bottom bounds.");
            }

            auto width = this->_bounds.right - this->_bounds.left;
            auto height = this->_bounds.bottom - this->_bounds.top;
            this->_startPoint.x = this->_bounds.left + width / 2;
            this->_startPoint.y = this->_bounds.top + height / 2;
        }
    }

    {
        auto it = MagicMousePad::FindArgument(_T("/startpoint"), args.begin(),
            args.end());
        if (it != args.end()) {
            auto coords = MagicMousePad::Tokenise(*it, Delims, Delims
                + std::size(Delims), true);
            this->_startPoint.x = ::_ttoi(coords[0].c_str());
            this->_startPoint.y = (coords.size() > 1)
                ? ::_ttoi(coords[1].c_str())
                : this->_startPoint.x;
        }
    }
}
