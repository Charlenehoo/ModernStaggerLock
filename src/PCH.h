#pragma once

// clib
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REL/Relocation.h>

// winnt
#include <ShlObj_core.h>

// workaround: NG dropped the legacy SKSEAPI calling-convention macro
//             that DKUtil's Hook headers rely on
#ifndef SKSEAPI
#   define SKSEAPI
#endif

// workaround: DKUtil's Hook/Shared.hpp uses GetModuleBaseNameA
//             without including <Psapi.h>
#include <Psapi.h>

using namespace std::literals;
using namespace REL::literals;

// Version
#include "Version.h"

// DKUtil
#include "DKUtil/Logger.hpp"


#define DLLEXPORT extern "C" [[maybe_unused]] __declspec(dllexport)