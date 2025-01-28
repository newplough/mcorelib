/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "mlogger.h"
#include "mtimertask.h"
#include "mobjectpool.h"
#include <queue>

MCORELIB_NAMESPACE_BEGING

DEFINE_SINGLETON(MTimerTask)

struct greater_class {
public:
    bool operator()(MTimerTask::TimerDat *a, MTimerTask::TimerDat *b)
    {
        return a->expiration > b->expiration;
    }
};

// MTimerTask
enum TimerState {
    TimerState_Removed = -2,
    TimerState_Stopped = -1,
    TimerState_PrepareRun = 0,
    TimerState_Running = 1
};

MTimerTask::MTimerTask()
    : MThread("TimerTask")
{
    mDebug("$_%s: %s", __FUNCTION__,objName().c_str());
}

MTimerTask::~MTimerTask()
{
     mDebug("#_%s: %s", __FUNCTION__,objName().c_str());

    m_sem.notify();
    stop();
    size_t count = timerHeap.size();
    for (size_t i = 0; i < count; i++)
    {
        TimerDat *td = timerHeap[i];
        if (td)
            delete td;
    }
    timerHeap.clear();
    timerMap.clear();
}

void MTimerTask::addTimer(MTimer *timer)
{
    muint_t objId = timer->objId();
    MMMutexLocker lck(&m_mutex);

    std::map<muint_t, TimerDat *>::iterator it = timerMap.find(objId);
    if (it == timerMap.end())
    {
        TimerDat *td = new TimerDat;
        if (td == NULL)
        {
            mDebug("Timer[%d] alloc failed!", objId);
            return;
        }
        timerMap[objId] = td;

        td->objId = objId;
        td->type = timer->m_type;
        td->interval = timer->m_interval;
        td->state = TimerState_PrepareRun;
        td->expiration = td->interval + GetOsTick();
        td->session = timer->m_session;
        mDebug("Timer[%d] Start expiration: %ld", objId, td->expiration);
        timerHeap.push_back(td); //插到数组最后一个位置上
        refInstance();
    }
    else
    {
        TimerDat *td = it->second;
        td->type = timer->m_type;
        td->interval = timer->m_interval;
        td->state = TimerState_PrepareRun;
        td->expiration = td->interval + GetOsTick();
        td->session = timer->m_session;
    }
    std::push_heap(timerHeap.begin(), timerHeap.end(), greater_class());
    lck.unlock();
    m_sem.notify();
}

void MTimerTask::stopTimer(muint_t objId)
{
    MMMutexLocker lck(&m_mutex);
    std::map<muint_t, TimerDat *>::iterator it = timerMap.find(objId);
    if (it != timerMap.end())
    {
        TimerDat *td = it->second;
        td->state = TimerState_Stopped;
    }
}

void MTimerTask::removeTimer(muint_t objId)
{
    MMMutexLocker lck(&m_mutex);
    std::map<muint_t, TimerDat *>::iterator it = timerMap.find(objId);
    if (it != timerMap.end())
    {
        TimerDat *td = it->second;
        td->state = TimerState_Removed;
        if (derefInstance() <= 0)
        {
            lck.unlock();
            delete this;
        }
    }
}

#define MAX_TIMEOUT 1000

enum TimerActiveStatus {
    TimerActiveStatus_NotExist = -1,
    TimerActiveStatus_Outofdate = 0,
    TimerActiveStatus_Wait = 1,
    TimerActiveStatus_Removed = 2
};

// 激活定时器
int MTimerTask::activeTimer(muint_t &objId, mllong_t &timeout, mint_t &seesion)
{
    int ret = TimerActiveStatus_NotExist;
    mllong_t now = GetOsTick();

    MMMutexLocker lck(&m_mutex);
    if (timerHeap.size() > 0)
    {
        std::pop_heap(timerHeap.begin(), timerHeap.end(), greater_class());
        TimerDat *td = timerHeap.back();
        objId = td->objId;
        // mDebug("TimerObj[%d]: state=%d type=%d reAdd=%d",
        // objId, td->state, td->type, reAdd);
        if (td->state >= 0)
        {
            mllong_t delta = 0; //偏差
            if (td->expiration <= now)
            {
                if (td->state == TimerState_PrepareRun)
                {
                    // delta = now - td->expiration;
                }
                else
                {
                    td->state = TimerState_Running;
                }
                timeout = 0;
            }
            else //还没有过期
            {
                timeout = td->expiration - now;
                if (timeout > MAX_TIMEOUT)
                {
                    timeout = MAX_TIMEOUT;
                }
            }
            // mDebug("Timer[%d] will run, %ld, %ld, %d, %d",
            //        objId, td->expiration, now, delta, timeout);

            if (timeout == 0)
            {
                timerHeap.pop_back(); // 先移除
                seesion = td->session;
                if (td->type == MTimer::CIRCLE)
                {
                    // mDebug("Timer run next: %ld", td->expiration);
                    td->expiration = td->interval + now - delta;
                    timerHeap.push_back(td); //更新后再加入
                }
                ret = TimerActiveStatus_Outofdate;
            }
            else
            {
                // mDebug("Timer[%d] Wait, %d", objId, timeout);
                ret = TimerActiveStatus_Wait; // do nothing for wait
            }
        }
        else
        {
            timerHeap.pop_back(); //直接移除
            if (td->state == TimerState_Removed)
            {
                timerMap.erase(objId);
                delete td;
                mDebug("Timer[%d] deleted", objId);
                ret = TimerActiveStatus_Removed;
            }
        }
        push_heap(timerHeap.begin(), timerHeap.end(), greater_class());
    }
    else
    {
        // ret = TIMER_NOT_EXIST;
    }

    return ret;
}

void MTimerTask::taskLoop(void)
{
    while (!isStopRequested())
    {
        muint_t objId;
        mllong_t timeout;
        mint_t seesion;
        switch (activeTimer(objId, timeout, seesion))
        {
        case TimerActiveStatus_Outofdate:
            MObjectPool::instance()->doSlot(objId, "MTimer::onTimeout1",
                                            MVariantList { MVariant(seesion) });
            break;
        case TimerActiveStatus_Wait:
            // mDebug("sleep: %d", timeout);
            m_sem.wait(timeout); // better
            // mDebug("sleepOK: %d", timeout);
            break;
        case TimerActiveStatus_Removed:
            break;
        case TimerActiveStatus_NotExist:
        default:
            m_sem.wait(MAX_TIMEOUT);
            break;
        }
    }
}

MCORELIB_NAMESPACE_END

