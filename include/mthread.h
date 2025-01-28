/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef _MTHREAD_H__
#define _MTHREAD_H__
#include "mobject.h"


MCORELIB_NAMESPACE_BEGING


class MThread : public MObject
{
public:
    MThread(const mstring_t &name = "",
            int stackSize = -1);

    virtual ~MThread();

    // 获取线程id
    mtid_t threadId() const;

    // 启动线程，sync表示是否等到等待线程运行之后才返回
    bool start(bool sync = true);

    // 请求终止线程，线程结束后才返回，
    // 被请求终止的现程需要响应终止请求，否则此处就不能返回了
    void stop();

    // 是否被请求终止
    bool isStopRequested() const;

    // 设置线程优先级, 为了充分利用系统提供的优先级，不同系统优先级范围不一样
    // 在windows中范围是[0,5],分别对应[THREAD_PRIORITY_LOWEST ... THREAD_PRIORITY_TIME_CRITICAL]
    // 在linux中范围是[0,99], 其中:0代表SCHED_OTHER线程, 1~99代表SCHED_RR线程的优先级1-99
    // 在freertos中范围是[0,47], 分别对应[osPriorityLow ... osPriorityRealtime7]
    void setPriority(muchar_t prio);

public:
    // 获取操作系统分配的当前线程ID
    static mtid_t getOsCurrentThreadId(void);

    // 获取程序分配的当前线程ID
    static mtid_t getCurrentThreadId(void);

    // 设置当前线程的优先级
    static void setCurrentThreadPriority(muchar_t prio);

    // 当前线程睡眠毫秒ms时间
    static void sleep(unsigned ms);

protected:
    // 线程初始化
    virtual int init();

    // 线程无限循环执行，直到被中止
    virtual void taskLoop(void);

private:
    // 启动线程后
    // 执行init和taskLoop
    void run();

private:    
    DECLARE_PLATFORM_DATA(MThread)
    DECLARE_PRIVATE_DATA(MThread)
};

MCORELIB_NAMESPACE_END
#endif
