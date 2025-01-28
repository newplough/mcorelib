/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef __MOBJECT_POOL_H__
#define __MOBJECT_POOL_H__
#include "mobject.h"
#include "mmutex.h"

MCORELIB_NAMESPACE_BEGING

// 对象池，维护系统中所有对象
class MObjectPool
{
    DECLARE_SINGLETON(MObjectPool)

    ~MObjectPool();

public:
    // 在对象池中增加对象
    void addObject(MObject *obj);

    // 从对象池中移除对象
    void removeObject(MObject *obj);

    // 注册槽函数：槽名称，槽函数
    void registerSlot(MSLOT slot, MObject::SlotFun);

    // 执行槽函数：执行对象的id， 槽名，参数列表
    void doSlot(muint_t objId, MSLOT slot, const MVariantList &para);

    // 触发对象的信号：发出对象的id， 信号名，参数列表
    void emitSig(muint_t objId, MSIG sig, const MVariantList &para);

    // 删除Object, 怎样判断newed?
    void deleteObject(muint_t objId);

private:
    std::map<muint_t, MObject *> m_objMap;
    std::map<mstring_t, MObject::SlotFun> m_slotMap;
    MMutex m_mutex;
};

MCORELIB_NAMESPACE_END
#endif
