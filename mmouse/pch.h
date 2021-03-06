// <copyright file="pch.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once

#include <WinSock2.h>

#include <atomic>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>

#include <shellapi.h>
#include <tchar.h>
#include <Windows.h>
#include <ws2tcpip.h>

#include "MagicMousePad/cmdline.h"
#include "MagicMousePad/endpoint.h"
#include "MagicMousePad/MouseSubscriber.h"
#include "MagicMousePad/onexit.h"
#include "MagicMousePad/ResourceHelper.h"
#include "MagicMousePad/tokenise.h"
