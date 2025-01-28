/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include <sys/syscall.h>
#include <unistd.h>
#include <pthread.h>
#include <thread>
#include <chrono>
#include "mthread.h"

MCORELIB_NAMESPACE_BEGING

class MThread::MThread_PlatformData
{
    friend void *taskShell(void *arg);
    MThread *m_task;
    mtid_t m_threadId;
    MEventLoop *m_looper;

public:
    inline MThread_PlatformData(MThread *task, const char* name, int stackSize)
        : m_task(task)
        , m_threadId(-1)
        , m_looper(nullptr)
    {
    }
    ~MThread_PlatformData()
    {
    }

    static mtid_t getOsCurrentThreadId(void)
    {
        return syscall(__NR_gettid);
    }

    static mtid_t getCurrentThreadId(void)
    {
        return pthread_self();
    }

    static void sleep(unsigned msec)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(msec));
    }

    static void *taskShell(void *arg)
    {
        MThread_PlatformData *p = (MThread_PlatformData *)arg;
        mDebug("taskShell: %ld", GetAppCurrentThreadId());
        if (p != nullptr)
        {
            p->runTask();
        }
        return nullptr;
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

    inline void start()
    {
        pthread_t tid = -1;
        pthread_create(&tid, nullptr, taskShell, this);
        m_threadId = (mtid_t)tid;
        mDebug("Task[%s] start, %ld", m_task->objName().c_str(),
                m_threadId);
    }

    inline void prepareExit()
    {
	    pthread_detach(m_threadId);
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
	    setThreadPriority(getCurrentThreadId(), prio);
	}
	
    static void setThreadPriority(mtid_t tid, muchar_t prio)
    {
        struct sched_param sp;
        memset((void *)&sp, 0, sizeof(sp));
        sp.sched_priority = prio;
        int poli = 0;
        if (prio <= 0)
        {
            poli = SCHED_OTHER;
        }
        else
        {
            poli = SCHED_RR;
        }
        // set the sched params for the current thread.
        if (tid <= 0)
        {
            tid = pthread_self();
        }
        if (tid > 0)
        {            
            int ret = pthread_setschedparam(tid, poli, &sp);
            if (ret != 0)
            {
                mWarn("setThreadPriority(%ld, %d) failed: %d", tid, prio, ret);
            }
        }
        else
        {
            mWarn("setThreadPriority(%ld, %d) failed", tid, prio);
        }
    }
};

MCORELIB_NAMESPACE_END
