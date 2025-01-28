/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "mthread.h"
#include "mappevent.h"
#include "mlogger.h"

#if (MOS_TYPE == MOS_LINUX)
#include "platform/linux_mthread.h"
#elif (MOS_TYPE == MOS_WIN)
#include "platform/win_mthread.h"
#elif (MOS_TYPE == MOS_RTOS)
#include "platform/rtos_mthread.h"
#else
#error " ^_^ "
#endif

MCORELIB_NAMESPACE_BEGING

class MThread::MThread_Private
{
public:
    MThread_Private()
            : stopped(false)
    {
    }
    std::atomic<bool> stopped;
    MSemaphore startSem;
    MSemaphore stopSem;
};

MThread::MThread(const mstring_t &name, int stackSize)
        : MObject(name)
        , platData(new MThread_PlatformData(this, name.c_str(), stackSize))
        , privData(new MThread_Private)
{
    mDebug("$_%s: %s", __FUNCTION__, objName().c_str());
}

MThread::~MThread()
{
    mDebug("#_%s: %s", __FUNCTION__, objName().c_str());
    stop();

    delete platData;
    platData = nullptr;

    delete privData;
    privData = nullptr;
}

// 获取线程id
mtid_t MThread::threadId() const
{
    return platData->threadId();
}

// 获取操作系统分配的当前线程ID
mtid_t MThread::getOsCurrentThreadId(void)
{
    return MThread_PlatformData::getOsCurrentThreadId();
}

// 获取当前运行线程的线程ID
mtid_t MThread::getCurrentThreadId(void)
{
    return MThread_PlatformData::getCurrentThreadId();
}

// 启动线程，sync表示是否等到等待线程运行之后才返回
bool MThread::start(bool sync)
{
    if (platData->threadId() < 0)
    {
        // 在start中获取到m_threadId
        platData->start();
        if (sync)
        {
            privData->startSem.wait();
        }
        return true;
    }
    else // 已经创建了
    {
        return false;
    }
}

// 请求终止线程，线程结束后才返回，
// 被请求终止的现程需要响应终止请求，否则此处就不能返回了
void MThread::stop()
{
    mDebug("Task[%s] Stop %ld", objName().c_str(), platData->threadId());
    if (platData->threadId() > 0)
    {
        if (MEventManager::instance()->stopEventLoop(platData->threadId()))
        {
            privData->stopped.store(true);
            platData->stop();
            privData->stopSem.wait();
        }
        else
        {
            mDebug("Task[%s] StopFAIL? %ld", objName().c_str(), platData->threadId());
        }
    }
    else
    {
        mDebug("Task[%s] StopFAIL? %ld", objName().c_str(), platData->threadId());
    }
}

// 是否被请求终止
bool MThread::isStopRequested() const
{
    return privData->stopped;
}

// 当前线程睡眠毫秒ms时间
void MThread::sleep(unsigned ms)
{
    MThread_PlatformData::sleep(ms);
}

// 线程初始化
int MThread::init()
{
    return 0;
}

// 线程无限循环执行，直到被中止
void MThread::taskLoop(void)
{
    if (platData)
    {
        platData->runLoop(-1);
    }
}

void MThread::run()
{
    mDebug("MThread[%s] Run[%zu]", objName().c_str(), GetAppCurrentThreadId());
    int ret = init();
    privData->startSem.notify();
    if (0 == ret)
    {
        taskLoop();
    }
    mDebug("MThread[%s] exit", objName().c_str());
    platData->prepareExit();
    privData->stopSem.notify();
    // 在此之后不能再访问MThread对象中的数据了，因为有可能被释放了
}

// 设置线程优先级
void MThread::setPriority(muchar_t prio)
{
    if (platData->threadId() > 0)
    {
        platData->setPriority(prio);
    }
}

// 设置当前线程的优先级
void MThread::setCurrentThreadPriority(muchar_t prio)
{
    MThread_PlatformData::setThreadPriority(getCurrentThreadId(), prio);
}

MCORELIB_NAMESPACE_END
