/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef __MTYPES_H__
#define __MTYPES_H__
#include <string>
#include "mnamespace.h"

MCORELIB_NAMESPACE_BEGING

// 定义基本数据类型

typedef signed char        mchar_t;
typedef unsigned char      muchar_t;
typedef signed int         mint_t;
typedef unsigned int       muint_t;
typedef signed long        mlong_t;
typedef unsigned long      mulong_t;
typedef signed long long   mllong_t;
typedef unsigned long long mullong_t;
typedef double             mdouble_t;
typedef float              mfloat_t;
typedef bool               mbool_t;
typedef mllong_t           mtid_t;
typedef std::string        mstring_t;

typedef struct
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
} mdate_t;

typedef struct
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t msec;
} mtime_t;

MCORELIB_NAMESPACE_END
#endif
