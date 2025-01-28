/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef M_LOGGER_H
#define M_LOGGER_H
#include "mmacroutils.h"


MCORELIB_NAMESPACE_BEGING

typedef void (*LOG_WRITER)(const char *data, int length);

void RigsterLogWriter(LOG_WRITER logWriter); // 注册新的日志写函数

#define LogDebug(format, ...)  /* 输出调试日志 */                                  \
        MCORELIB_NAME::LogOutput("D|%s:%d| " format, _BASENAME(__FILE__), __LINE__, ##__VA_ARGS__)

#define LogInfo(format, ...)  /* 输出信息日志 */                                   \
        MCORELIB_NAME::LogOutput("I|%s:%d| " format, _BASENAME(__FILE__), __LINE__, ##__VA_ARGS__)

#define LogWarn(format, ...)  /* 输出警告日志 */                                   \
        MCORELIB_NAME::LogOutput("W|%s:%d| " format, _BASENAME(__FILE__), __LINE__, ##__VA_ARGS__)

#define LogError(format, ...)  /* 输出错误日志 */                                  \
        MCORELIB_NAME::LogOutput("E|%s:%d| " format, _BASENAME(__FILE__), __LINE__, ##__VA_ARGS__)

// 格式化输出日志信息
void LogOutput(const char *format, ...);

MCORELIB_NAMESPACE_END
#endif // MLOGGER_H
