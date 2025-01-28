/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef MTHREAD_RTOS_H
#define MTHREAD_RTOS_H
#include "mthread.h"
#include "cmsis_os.h"

// 使用cmsis接口封装线程类

#define DEFAULT_STACK_SIZE (2048) // 栈空间默认2k

MCORELIB_NAMESPACE_BEGING

class MThread::MThread_PlatformData
{
    friend void *taskShell(void *arg);
    
    MThread *m_task;    
    MEventLoop *m_looper;    
    
    osThreadId_t m_taskHandle;
    osThreadAttr_t m_attributes;
    
public:
    inline MThread_PlatformData(MThread *task, const char* name, int stackSize)
        : m_task(task)        
        , m_looper(nullptr)
        , m_taskHandle(nullptr)
    {
        m_attributes.name = name;
        if (stackSize <= 0)
        {
            stackSize = DEFAULT_STACK_SIZE;
        }
        else
        {
            stackSize = (stackSize>>2) << 2;
        }
        m_attributes.stack_size = stackSize;
        m_attributes.priority = (osPriority_t) osPriorityNormal;
    }
    ~MThread_PlatformData()
    {
    }

    static mtid_t getOsCurrentThreadId(void)
    {
        return (mtid_t) osThreadGetId();
    }

    static mtid_t getCurrentThreadId(void)
    {
        return (mtid_t) osThreadGetId();
    }

    static void sleep(unsigned ms)
    {
        osDelay(pdMS_TO_TICKS(ms));
    }

    static void taskShell(void *arg)
    {
        MThread_PlatformData *p = (MThread_PlatformData *)arg;
        mDebug("taskShell: %ld", GetAppCurrentThreadId());
        if (p != nullptr)
        {
            p->runTask();
        }
    }

    mtid_t threadId() const
    {
        return m_taskHandle ? (mtid_t)m_taskHandle : -1;
    }

    void runTask()
    {
        m_looper = new MEventLoop(m_task->objName());
        m_task->run();

        delete m_looper;
        m_looper = nullptr;
        osThreadExit();
    }

    inline void start()
    {
        m_taskHandle = osThreadNew(taskShell, this, &m_attributes);
        mDebug("Task[%s] start, %ld", m_task->objName().c_str(), threadId());
    }

    inline void prepareExit()
    {
        m_taskHandle = nullptr;
    }

    inline void stop()
    {
        if (m_looper)
        {
            m_looper->quit();
        }
        mDebug("Task[%s] StopOK", m_task->objName().c_str());
    }

    inline void runLoop(int ms)
    {
        if (m_looper)
        {
            m_looper->run(ms);
        }
    }

    void setPriority(muchar_t prio)
    {
        setThreadPriority(threadId(), prio);
    }

    static void setThreadPriority(mtid_t tid, muchar_t prio)
    {
        if (tid > 0)
        {
            prio += osPriorityLow;
            if (prio >= osPriorityISR)
            {
                prio -= 1;
            }
            osThreadSetPriority((osThreadId_t)tid, (osPriority_t)prio);
        }
    }
};

MCORELIB_NAMESPACE_END
#endif

