/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "mapplication.h"
#include "mappevent.h"
#include "mtimertask.h"
#include "mobjectpool.h"
#include "mlogger.h"

MCORELIB_NAMESPACE_BEGING

static std::atomic<void*> privPtr;
class MApplication::MApplication_Private : public MObject
{
public:
    MApplication_Private()
        : MObject("APPPD")
        , loop("APPPD")
    {
		mDebug("MApplication_Private: %p\n", this);
        privPtr.store(this);
    }

    ~MApplication_Private()
    {
        mDebug("~MApplication_Private\n");
        loop.quit();
    }

    inline int exec()
    {
        loop.run(-1);
        return 0;
    }

private:
    MEventLoop loop;
};


MApplication *MApplication::instance = nullptr;

MApplication::MApplication()
    : privData(nullptr)
{
    init();
}

MApplication::~MApplication()
{
    if (instance == this)
    {
        instance = nullptr;
		void *p = privPtr.load();
        mDebug("~MApplication: %p", p);
        if (p != nullptr)
        {
            privPtr.store(nullptr);
            if (privData != nullptr)
            {
                delete privData;
				privData = nullptr;
            }
        }

        MTimerTask::destroy();
        MObjectPool::destroy();
        MEventManager::destroy();
    }
}

void MApplication::init()
{
    if (instance == nullptr)
    {
        instance = this;
        MObjectPool::instance();
        MEventManager::instance();

        privData = new MApplication_Private();

        MTimerTask *tim = MTimerTask::instance();
        // 启动时间任务
        tim->start();
    }
}

// 设置应用程序优先级
// 在一些系统中需要高级权限才能调用该接口
void MApplication::setPriority(int prio)
{
    MThread::setCurrentThreadPriority(prio);
	MTimerTask::instance()->setPriority(prio);
}

// 应用程序执行，启动事件循环
// 如果没有主动退出，一直在这儿循环
int MApplication::exec()
{
    if ((instance == this) && (privData != nullptr))
    {
        return privData->exec();
    }
    return -1;
}

// 退出应用程序
void MApplication::quit()
{
    if (instance == this)
    {
        // 退出请求可能在多个线程中同时发出
        // 这里需要判断有效性，避免失败错误
	    std::atomic<void*> null(nullptr);
        void * p = privPtr.exchange(null);
        if (p != nullptr)
        {
            mDebug("deleteLater: %p", p);
            privData->deleteLater();
            privData = nullptr;
        }
    }
}

MCORELIB_NAMESPACE_END
