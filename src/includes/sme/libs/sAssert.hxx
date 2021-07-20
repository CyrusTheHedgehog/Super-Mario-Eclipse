#pragma once

#include "OS.h"
#include "PPCArch.h"
#include "printf.h"

#include "macros.h"

#define SME_ASSERT(expr, msg, ...)                                             \
  if (!(expr)) {                                                               \
    char errmsg[256];                                                          \
    sprintf(errmsg, "[SME] %s: %s", (SME_FUNC_SIG), (msg));                        \
    OSPanic(__FILE__, __LINE__, errmsg, ##__VA_ARGS__);                        \
    __OSUnhandledException(6, OSGetCurrentContext(), 0);                                           \
  }

#ifdef SME_DEBUG
#define SME_DEBUG_ASSERT(expr, msg, ...) SME_ASSERT(expr, msg, ##__VA_ARGS__)
#else
#define SME_DEBUG_ASSERT(expr, msg, ...)
#endif

#define SME_ERROR(msg, ...)                                                    \
  char errmsg[256];                                                            \
  sprintf(errmsg, "[SME] %s: %s", (SME_FUNC_SIG), (msg));                          \
  OSPanic(__FILE__, __LINE__, errmsg, ##__VA_ARGS__);
