// <copyright file="pch.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "targetver.h"
#include <WinSock2.h>

#include <cassert>
#include <cinttypes>
#include <functional>
#include <stdexcept>
#include <string>
#include <system_error>
#include <thread>
#include <vector>

#include <d2d1.h>
#include <dwrite.h>
#include <tchar.h>
#include <Windows.h>
#include <windowsx.h>
#include <WS2tcpip.h>

#include "MagicMousePad/byteorder.h"
#include "MagicMousePad/messages.h"
#include "MagicMousePad/MouseButton.h"
#include "MagicMousePad/onexit.h"
#include "MagicMousePad/ResourceHelper.h"
#include "MagicMousePad/serveraddress.h"
