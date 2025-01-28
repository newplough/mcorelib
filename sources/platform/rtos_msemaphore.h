/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef MSEMAPHORE_RTOS_H
#define MSEMAPHORE_RTOS_H
#include "msemaphore.h"
#include "cmsis_os.h"

// 使用cmsis接口封装信号量类

MCORELIB_NAMESPACE_BEGING

class MSemaphore::MSemaphore_PlatformData
{
    osSemaphoreId_t handle;
    
public:
	MSemaphore_PlatformData(muint_t n)
	{
	    handle = osSemaphoreNew(n+5, n, nullptr);
    }

	~MSemaphore_PlatformData()
    {
        if (handle)
        {
            osSemaphoreDelete(handle);
            handle = nullptr;
        }
    }
    
	void notify()
    {
	    if (handle)
	    {
	        osSemaphoreRelease(handle);
	    }
    }

	bool wait()
    {
	    if (handle)
	    {
	        return osOK == osSemaphoreAcquire(handle, portMAX_DELAY);
	    }
	    return false;
    }

	bool wait(mllong_t mesc)
	{
		return (osOK == osSemaphoreAcquire(handle, pdMS_TO_TICKS(mesc)));
	}
};

MCORELIB_NAMESPACE_END
#endif

