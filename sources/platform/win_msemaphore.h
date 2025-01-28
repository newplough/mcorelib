/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef M_SEMAPHORE_WIIN_H
#define M_SEMAPHORE_WIIN_H
#include <condition_variable>
#include <mutex>
#include "msemaphore.h"

MCORELIB_NAMESPACE_BEGING


class MSemaphore::MSemaphore_PlatformData
{
    std::mutex mtx;
    std::condition_variable cv;
    int count;
    
public:
    MSemaphore_PlatformData(muint_t n)
        : count(n)
	{
    }

	~MSemaphore_PlatformData()
    {
    }
    
	void notify()
    {
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        cv.notify_one();
    }

	bool wait()
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (count == 0)
        {
            // wait on the mutex until notify is called
            cv.wait(lock);
        }
        count--;
        return true;
    }

	bool wait(mllong_t mesc)
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
