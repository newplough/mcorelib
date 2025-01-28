/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef MMUTEX_WIN_H
#define MMUTEX_WIN_H
#include <pthread.h>
#include "mmutex.h"
#include "mdebug.h"

MCORELIB_NAMESPACE_BEGING

class MMutex::MMutex_PlatformData
{
    pthread_mutex_t m_mutex;

public:
    MMutex_PlatformData()
    {
        pthread_mutexattr_t attr;
        int ret = pthread_mutexattr_init(&attr);
        if (ret != 0)
        {
            mWarn("pthread_mutexattr_init failed: %d\n", ret);
        }
        // if (recursive)
        //{
        //    ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        //}

        ret = pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
        if (ret != 0)
        {
            const int max_prio = sched_get_priority_max(SCHED_FIFO);
            ret = pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_PROTECT);
            ret = ret || pthread_mutexattr_setprioceiling(&attr, max_prio);
            if (ret != 0)
            {
                mWarn("pthread_mutexattr_setprioceiling[%d] failed: %d\n",
                       max_prio, ret);
            }
        }
        ret = pthread_mutex_init(&m_mutex, &attr);
        if (ret != 0)
            mWarn("pthread_mutex_init failed: %d\n", ret);

        ret = pthread_mutexattr_destroy(&attr);
        if (ret != 0)
            mWarn("pthread_mutexattr_destroy failed: %d\n", ret);
    }

    ~MMutex_PlatformData()
    {
    }

    bool lock()
    {
        return 0 == pthread_mutex_lock(&m_mutex);
    }

    void unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }
};


MCORELIB_NAMESPACE_END
#endif
