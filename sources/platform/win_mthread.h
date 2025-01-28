/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef MTHREAD_WIN_H
#define MTHREAD_WIN_H
#include <windows.h>
#include <process.h>
#include <thread>
#include "mthread.h"

MCORELIB_NAMESPACE_BEGING

static int winPriorityLevel[] =
{
    THREAD_PRIORITY_LOWEST,
    THREAD_PRIORITY_BELOW_NORMAL,
    THREAD_PRIORITY_NORMAL,
    THREAD_PRIORITY_ABOVE_NORMAL,
    THREAD_PRIORITY_HIGHEST,
    THREAD_PRIORITY_TIME_CRITICAL,
};

class MThread::MThread_PlatformData
{
    friend void *taskShell(void *arg);

    MThread *m_task;
    mtid_t m_threadId;
    HANDLE m_handle;
    MEventLoop *m_looper;

public:
    inline MThread_PlatformData(MThread *task, const char* name, int stackSize)
        : m_task(task)
        , m_threadId(-1)
        , m_handle(nullptr)
        , m_looper(nullptr)
    {
    }
    ~MThread_PlatformData()
    {
    }

    static mtid_t getOsCurrentThreadId(void)
    {
        return getCurrentThreadId();
    }

    static mtid_t getCurrentThreadId(void)
    {
         return GetCurrentThreadId();
    }

    static void sleep(unsigned msec)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(msec));
    }

    static unsigned __stdcall taskShell(void *arg)
    {
        MThread_PlatformData *p = (MThread_PlatformData *)arg;
        mDebug("taskShell: %ld", GetAppCurrentThreadId());
        if (p != nullptr)
        {
            p->runTask();
        }
        return 0;
    }

    mtid_t threadId() const
    {
        return m_threadId;
    }

    void runTask()
    {
        m_looper = new MEventLoop(m_task->objName());
        m_task->run();

        delete m_looper;
        m_looper = nullptr;
    }

    inline void start(void)
    {
        unsigned uiThread2ID = 0;
        m_handle = (HANDLE)_beginthreadex(NULL, 0, &taskShell, this,
                                          CREATE_SUSPENDED, &uiThread2ID);
        m_threadId = (mtid_t)uiThread2ID;

        ResumeThread(m_handle);
        mDebug("Task[%s] start, %ld", m_task->objName().c_str(),
                m_threadId);
    }

    inline void prepareExit()
    {
        m_threadId = -1;
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
        if (prio >= ARRAY_SIZE(winPriorityLevel))
        {
            prio = ARRAY_SIZE(winPriorityLevel) - 1;
        }

        SetThreadPriority(m_handle, winPriorityLevel[prio]);
    }

    static void setThreadPriority(mtid_t tid, muchar_t prio)
    {
    }
};

MCORELIB_NAMESPACE_END
#endif

