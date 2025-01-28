/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "mlogger.h"


void* operator new(std::size_t size) 
{
    void* ptr = pvPortMalloc(size);
    if (!ptr) 
    {
        LogError("alloc failed: %d\n", size);
    }
    return ptr;
}
 
void operator delete(void* ptr) noexcept 
{
    vPortFree(ptr);
}
