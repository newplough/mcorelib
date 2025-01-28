/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef MSEMAPHORE_LINUX_H
#define MSEMAPHORE_LINUX_H
#include <condition_variable>
#include "mmutex.h"
MCORELIB_NAMESPACE_BEGING

class MSemaphore::MSemaphore_PlatformData
{
    std::condition_variable cv;
    std::mutex mtx;
    muint_t count;

public:
    MSemaphore_PlatformData(muint_t n)
        : count(n)
    {
    }

    inline void notify()
    {
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        cv.notify_one();
    }

    inline void wait()
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (count == 0)
        {
            // wait on the mutex until notify is called
            cv.wait(lock);
        }
        count--;
    }

    inline bool wait(mllong_t mesc)
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (cv.wait_for(lock, std::chrono::milliseconds(mesc))
            == std::cv_status::timeout)
        {
            return false;
        }
        count--;
        return true;
    }
};

MCORELIB_NAMESPACE_END
#endif
