/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef MMUTEX_RTOS_H
#define MMUTEX_RTOS_H
#include "mmutex.h"
#include "cmsis_os.h"

MCORELIB_NAMESPACE_BEGING

class MMutex::MMutex_PlatformData
{
    osMutexId_t handle;

public:
    MMutex_PlatformData()
    {
        handle = osMutexNew(nullptr);
    }
    ~MMutex_PlatformData()
    {
        if (handle)
        {
            osMutexDelete(handle);
            handle = nullptr;
        }
    }

    bool lock()
    {
        if (handle)
        {
            return (osOK == osMutexAcquire(handle, portMAX_DELAY));
        }
        return false;
    }

    void unlock()
    {
        if (handle)
        {
            osMutexRelease(handle);
        }
    }
};

MCORELIB_NAMESPACE_END
#endif

