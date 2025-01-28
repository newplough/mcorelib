/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "mobjectpool.h"
#include "mappevent.h"
#include "mlogger.h"

MCORELIB_NAMESPACE_BEGING

// 分配唯一对象id
static muint_t allocObjId()
{
    static std::atomic_uint count(0);
    return ++count;
}

MObject::MObject(const mstring_t &name)
    : m_name(name)
{
    m_ownerTid = GetAppCurrentThreadId();
    m_objId = allocObjId();
    MObjectPool::instance()->addObject(this);

    mDebug("$_%s: %s, m_objId=%u, tid=%u",
            __FUNCTION__,objName().c_str(), m_objId, m_ownerTid);
}

MObject::~MObject()
{
    mDebug("#_%s: %s, m_objId=%u", __FUNCTION__,objName().c_str(), m_objId);

    MObjectPool::instance()->removeObject(this);
    mtid_t freeId = GetAppCurrentThreadId();
    if (m_ownerTid != freeId)
    {
        mWarn("Object[%s,%d] owner=%u freeId=%u", objName().c_str(), m_objId,
                 m_ownerTid, freeId);
    }
}

// 注册slot函数：slot名称，slot函数
void MObject::registerSlot(MSLOT slot, MObject::SlotFun slotFunc)
{
    MObjectPool::instance()->registerSlot(slot, slotFunc);
}

// 用信号名关联对象object的槽名称slot
void MObject::connect(MSIG signal, const MObject &object, MSLOT slot)
{
    std::vector<ObjSlot> ssList;
    findObjSlot(signal, ssList);
    for(auto ss : ssList)
    {
        if (ss.objId == object.objId() && ss.slot == slot)
        {
            mWarn("Exist the Signal: %s, %d, %s", signal, object.objId(), slot);
            return;
        }
    }

    m_sigMap.insert(std::make_pair(signal, ObjSlot { object.objId(), slot }));
}

// 发出信号：signal名，参数列表
void MObject::emitSig(MSIG signal, const MVariantList &para)
{
    MObjectPool::instance()->emitSig(objId(), signal, para);
}

// 通过信号名字找出关联的objId和slot函数
void MObject::findObjSlot(MSIG signal, std::vector<ObjSlot> &ssList)
{
    ssList.clear();

    auto range = m_sigMap.equal_range(signal);

    // 遍历这个范围，打印所有的值
    for (auto it = range.first; it != range.second; ++it)
    {
        ssList.push_back(it->second);
    }
}

// 确保MObject是new出来的
// 否则调用deleteLater后会导致崩溃
void MObject::deleteLater()
{
    Event evt = { this->ownerTid(), this->objId(), Event::Deletelater, "" };
    MEventManager::instance()->postEvent(evt);
}

MCORELIB_NAMESPACE_END
