/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef __MPLATFORM_H__
#define __MPLATFORM_H__
#include "mmacroutils.h"

// 定义操作系统类型
#define MOS_WIN      1
#define MOS_LINUX    2
#define MOS_RTOS     3

#ifdef _WIN32
#define MOS_TYPE MOS_WIN
#elif __linux__
#define MOS_TYPE MOS_LINUX
#else
// 没法检测到系统则指定
#define MOS_TYPE MOS_RTOS
#endif


MCORELIB_NAMESPACE_BEGING


// 获取系统TICK
mllong_t GetOsTick(void);

// 获取系统当前日期
void GetOsCurrentDate(mdate_t& date);

// 获取系统当前时间
void GetOsCurrentTime(mtime_t& date);

// 睡眠毫秒时间
void SleepMsec(unsigned int msec);

// 操作系统分配的当前线程ID
mtid_t GetOsCurrentThreadId(void);

// 程序分配的当前线程ID
mtid_t GetAppCurrentThreadId(void);


MCORELIB_NAMESPACE_END
#endif
