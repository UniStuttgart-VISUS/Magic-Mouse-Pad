// <copyright file="pch.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2020 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "targetver.h"
#include <WinSock2.h>

#include <cassert>
#include <cinttypes>
#include <stdexcept>
#include <string>
#include <system_error>

#include <d2d1.h>
#include <dwrite.h>
#include <tchar.h>
#include <Windows.h>
#include <windowsx.h>

#include "MagicMousePad/MouseButton.h"
#include "MagicMousePad/onexit.h"
#include "MagicMousePad/ResourceHelper.h"
