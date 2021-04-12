#pragma once

#include "OS.h"
#include "macros.h"

#define SME_LOG(msg, ...) OSReport("[SME] %s", msg, ##__VA_ARGS__)

#ifdef SME_DEBUG
#define SME_DEBUG_LOG(msg, ...) SME_LOG(msg, ##__VA_ARGS__)
#else
#define SME_DEBUG_LOG(msg, ...)
#endif
