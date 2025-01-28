/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef _MDEBUG_H__
#define _MDEBUG_H__
#include "mlogger.h"

//#define ENABLE_DEBUG_OUT
#define ENABLE_WARNING_OUT

#ifdef ENABLE_DEBUG_OUT
#define mDebug LogDebug
#else
#define mDebug(...)
#endif

#ifdef ENABLE_WARNING_OUT
#define mWarn LogWarn
#else
#define mWarn(...)
#endif

#endif
