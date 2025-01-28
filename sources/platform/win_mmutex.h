/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef MMUTEX_WIN_H
#define MMUTEX_WIN_H
#include "mmutex.h"
#include <mutex>

MCORELIB_NAMESPACE_BEGING

class MMutex::MMutex_PlatformData
{
    std::mutex m_mutex;

public:
    MMutex_PlatformData()
    {
    }
    ~MMutex_PlatformData()
    {
    }

    bool lock()
    {
        m_mutex.lock();
        return true;
    }

    void unlock()
    {
        m_mutex.unlock();
    }
};


MCORELIB_NAMESPACE_END
#endif
