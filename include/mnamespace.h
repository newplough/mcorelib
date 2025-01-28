/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef M_NAMESPACE_H
#define M_NAMESPACE_H

//[*] 命名空间辅助

#define MCORELIB_NAME mcorelib
//-1 命名空间开头
#define MCORELIB_NAMESPACE_BEGING  \
namespace MCORELIB_NAME {
//-2 命名空间结尾
#define MCORELIB_NAMESPACE_END \
}
//-2 命名空间使用
#define USE_MCORELIB_NAMESPACE \
using namespace MCORELIB_NAME;

#endif
