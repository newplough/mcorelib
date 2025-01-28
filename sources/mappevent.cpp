/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "mappevent.h"
#include "mobjectpool.h"
#include "mlogger.h"


MCORELIB_NAMESPACE_BEGING

DEFINE_SINGLETON(MEventManager)

// 事件管理
MEventManager::MEventManager()
{
    mDebug("$_%s", __FUNCTION__);
}

MEventManager::~MEventManager()
{
    mDebug("#_%s", __FUNCTION__);
}

void MEventManager::registerEventLoop(mtid_t tid, MEventLoop *evtloop)
{
    mDebug("registerEventLoop: %d, %u", (int)instRefer, (int)tid);
    MMMutexLocker lck(&m_mutex);
    std::map<mtid_t, MEventLoop *>::iterator it = m_mapLoop.find(tid);
    if (it == m_mapLoop.end())
    {
        m_mapLoop[tid] = evtloop;
        refInstance();
    }
    else
    {
        mWarn("Exsist the event loop");
    }
}

void MEventManager::unregisterEventLoop(mtid_t tid)
{
    mDebug("unregisterEventLoop: %d, %u", (int)instRefer, (int)tid);
    MMMutexLocker lck(&m_mutex);
    std::map<mtid_t, MEventLoop *>::iterator it = m_mapLoop.find(tid);
    if (it != m_mapLoop.end())
    {
        m_mapLoop.erase(tid);
        lck.unlock();
        if (derefInstance() <= 0)
            delete this;
    }
}

bool MEventManager::stopEventLoop(mtid_t tid)
{
    MMMutexLocker lck(&m_mutex);
    std::map<mtid_t, MEventLoop *>::iterator it = m_mapLoop.find(tid);
    if (it != m_mapLoop.end())
    {
        MEventLoop *loop = it->second;
        loop->quit();
        return true;
    }
    else
    {
        mWarn("Not find the event loop: %ld", tid);
        return false;
    }
}

// 投递槽执行事件, 实际调用postEvent，方便外部使用
void MEventManager::postSlot(mtid_t tid,
                         muint_t objId,
                         MSLOT slot,
                         const MVariantList &para)
{
    // mDebug("doAction: %d, %d, %s", tid, objId, slot);
    MMMutexLocker lck(&m_mutex);
    std::map<mtid_t, MEventLoop *>::iterator it = m_mapLoop.find(tid);
    if (it != m_mapLoop.end())
    {
        MEventLoop *loop = it->second;
        Event event = { tid, objId, Event::Slot, slot, para };
        loop->addEvent(event);
    }
    else
    {
        mWarn("Not contains the task: %u %s", tid, slot);
    }
}

// 投递执行事件
void MEventManager::postEvent(const Event &event)
{
    MMMutexLocker lck(&m_mutex);
    std::map<mtid_t, MEventLoop *>::iterator it = m_mapLoop.find(event.tid);
    if (it != m_mapLoop.end())
    {
        MEventLoop *loop = it->second;
        loop->addEvent(event);
    }
    else
    {
        mWarn("Not contains the task: %u", event.tid);
    }
}

// 事件循环
MEventLoop::MEventLoop(const mstring_t& name)
    : m_name(name)
{
    m_threadId = GetAppCurrentThreadId();
    MEventManager::instance()->registerEventLoop(m_threadId, this);
}

MEventLoop::~MEventLoop()
{
    mDebug("unregisterEventLoop: %s, %ld", m_name.c_str(), m_threadId);
    MEventManager::instance()->unregisterEventLoop(m_threadId);
}

// 从事件队列中获取事件并执行相应动作
void MEventLoop::run(int timeMs)
{
    Event evt;
    mllong_t baseTick = GetOsTick();
    while (m_queue.isActive())
    {
        if (m_queue.pop(evt, timeMs <= 0))
        {
            // mDebug("Event Pop: %d, %s", evt.tid, evt.slot);
            if (evt.tid == m_threadId) //确保处于同一线程
            {
                switch (evt.type)
                {
                case Event::Slot:
                    MObjectPool::instance()->doSlot(evt.objId, evt.slot,
                                                    evt.para);
                    break;
                case Event::Signal:
                    MObjectPool::instance()->emitSig(evt.objId, evt.slot,
                                                     evt.para);
                    break;
                case Event::Deletelater:
                    MObjectPool::instance()->deleteObject(evt.objId);
                    break;
                default:
                    break;
                }
            }
            else
            {
                mWarn("Not in the same task: %u, %u, %u, %s", evt.objId,
                         evt.tid, GetAppCurrentThreadId(), evt.slot);
            }
        }
        else
        {
        }

        if (timeMs > 0)
        {
            if (GetOsTick() < (baseTick + timeMs))
            {
                SleepMsec(1);
            }
            else
            {
                break;
            }
        }
    } // while
}

 // 向事件队列中增加事件
void MEventLoop::addEvent(const Event &event)
{
    if (!m_queue.push(event))
    {
        mWarn("Why is the queue full?");
    }
}

// 事件循环退出
void MEventLoop::quit()
{
    m_queue.quit();
}

MCORELIB_NAMESPACE_END
