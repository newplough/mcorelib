/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "mlogger.h"
#include "mplatform.h"
#include "mmutex.h"
#include <stdarg.h>
#include <string.h>


MCORELIB_NAMESPACE_BEGING

static MMutex *_logMutex = nullptr;

#define LOG_LOCK()                                                             \
    do                                                                         \
    {                                                                          \
        if (_logMutex == nullptr)                                              \
            _logMutex = new MMutex();                                          \
        if (_logMutex)                                                         \
            _logMutex->lock();                                                 \
    } while (0)

#define LOG_UNLOCK()                                                           \
    if (_logMutex)                                                             \
    _logMutex->unlock()

#if (MOS_TYPE == MOS_WIN) || (MOS_TYPE == MOS_LINUX)
static void defaultLogWriter(const char *data, int length)
{
    fwrite(data, length, 1, stdout);
    fflush(stdout);
}

static LOG_WRITER _logWriter = defaultLogWriter;
    
#else
static LOG_WRITER _logWriter = nullptr;
    
#endif
    
void RigsterLogWriter(LOG_WRITER logWriter)
{
    LOG_LOCK();
    _logWriter = logWriter;
    LOG_UNLOCK();
}


void LogOutput(const char *format, ...)
{
    int bytes = 0;

    static char buf[512];
    va_list list;
	
    mtime_t now;
    GetOsCurrentTime(now);
    LOG_LOCK();

#ifdef _MSC_VER
    bytes = sprintf_s(buf, "|%02d:%02d:%02d.%03d|%ld|",
                      now.hour, now.minute, now.second, now.msec,
                      (mulong_t)GetOsCurrentThreadId());
    va_start(list, format);
    bytes += vsnprintf_s(buf + bytes, sizeof(buf) - bytes - 1, _TRUNCATE,
                         format, list);
    bytes += snprintf(buf + bytes, sizeof(buf) - bytes - 1, "\n");
#else
    bytes = sprintf(buf, "|%02d:%02d:%02d.%03d|%ld|",
                    now.hour, now.minute, now.second, now.msec,
                    (mulong_t)GetOsCurrentThreadId());
    va_start(list, format);
    bytes += vsnprintf(buf + bytes, sizeof(buf) - bytes - 1, format, list);
    bytes += snprintf(buf + bytes, sizeof(buf) - bytes - 1, "\n");
#endif
    va_end(list);
    if (_logWriter != nullptr)
    {
        _logWriter(buf, bytes);
    }
    LOG_UNLOCK();
}

MCORELIB_NAMESPACE_END
