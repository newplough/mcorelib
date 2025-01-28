/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef __MAPP_EVENT_H__
#define __MAPP_EVENT_H__
#include "mobject.h"
#include "mwaitqueue.h"
#include "mmutex.h"

MCORELIB_NAMESPACE_BEGING

// Event结构体定义
struct Event {
    // event类型定义
    enum TYPE {
        Signal = 0, // 信号
        Slot,       // 槽
        Deletelater // 随后删除
    };

    mtid_t tid;  // 发出event的线程id
    muint_t objId; // 发出event的对象id
    TYPE type;  // event类型
    // event联合数据
    union {
        MSIG sig;
        MSLOT slot;
    };
    // event的参数列表
    MVariantList para;

    //TODO: 可以考虑增加同步功能
};

class MEventLoop;

// 事件管理
class MEventManager final
{
    friend class MEventLoop;
    friend class MThread;

    DECLARE_SINGLETON(MEventManager)
    ~MEventManager();

public:
    // 投递槽执行事件, 实际调用postEvent，方便外部使用
    void postSlot(mtid_t tid, muint_t objId, MSLOT slot, const MVariantList &);

    // 投递执行事件
    void postEvent(const Event &event);

protected:
    void registerEventLoop(mtid_t tid, MEventLoop *evtloop);
    void unregisterEventLoop(mtid_t tid);
    bool stopEventLoop(mtid_t tid);

private:
    std::map<mtid_t, MEventLoop *> m_mapLoop;
    MMutex m_mutex;
};

// 事件循环
class MEventLoop final
{
    DISABLE_CLASS_COPY(MEventLoop)

public:
    MEventLoop(const mstring_t& name);
    ~MEventLoop();

    // 从事件队列中获取事件并执行相应动作
    void run(int timeMs);

    // 向事件队列中增加事件
    void addEvent(const Event &event);

    // 事件循环退出
    void quit();

private:
    mstring_t m_name;
    MWaitQueue<Event> m_queue;
    mtid_t m_threadId;
};

MCORELIB_NAMESPACE_END
#endif
