/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef __MTIMER_TASK_H__
#define __MTIMER_TASK_H__
#include "mobject.h"
#include "msemaphore.h"
#include "mthread.h"
#include "mtimer.h"
#include "mmutex.h"

MCORELIB_NAMESPACE_BEGING

// 定时器任务
class MTimerTask : public MThread
{
    DECLARE_SINGLETON(MTimerTask)
    virtual ~MTimerTask();

protected:
    virtual void taskLoop(void);

private:
    friend class MTimer;

    //添加定时器
    void addTimer(MTimer *timer);

    //停止定时器
    void stopTimer(muint_t objId);

    //移除定时器
    void removeTimer(muint_t objId);

    // 激活定时器
    int activeTimer(muint_t &objId, mllong_t &timeout, mint_t &session);

public:
    struct TimerDat {
        muint_t objId;
        MTimer::Type type;
        muint_t interval;
        mint_t state;
        mllong_t expiration;
        mint_t session;
    };

private:
    std::vector<TimerDat *> timerHeap;
    std::map<muint_t, TimerDat *> timerMap;
    MMutex m_mutex;
    MSemaphore m_sem;
};

MCORELIB_NAMESPACE_END
#endif
