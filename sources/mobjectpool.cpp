/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "mappevent.h"
#include "mlogger.h"
#include "mobjectpool.h"

MCORELIB_NAMESPACE_BEGING

DEFINE_SINGLETON(MObjectPool)

MObjectPool::MObjectPool()
{
    mDebug("$_%s", __FUNCTION__);
}

MObjectPool::~MObjectPool()
{
    mDebug("#_%s", __FUNCTION__);
}

void MObjectPool::addObject(MObject *obj)
{
    if (obj != nullptr)
    {
        muint_t objId = obj->objId();
        MMMutexLocker lck(&m_mutex);
        std::map<muint_t, MObject *>::iterator it = m_objMap.find(objId);
        if (it == m_objMap.end())
        {
            m_objMap[objId] = obj;
            refInstance(); // 增加引用计数
            mDebug("addObject: [%d], %d", objId, (int)instRefer);
        }
        else
        {
            mDebug("addObject Failed: [%d]=%p, %d", objId,
                    (int)instRefer);
        }
    }
}

void MObjectPool::removeObject(MObject *obj)
{
    if (obj != nullptr)
    {
        muint_t objId = obj->objId();
        mDebug("removeObject: [%d], %d", objId, (int)instRefer);

        MMMutexLocker lck(&m_mutex);
        std::map<muint_t, MObject *>::iterator it = m_objMap.find(objId);
        if (it != m_objMap.end())
        {
            m_objMap.erase(objId);
            lck.unlock();
            if (derefInstance() <= 0)
            {
                delete this;
            }
        }
        else
        {
            mDebug("removeObject Faild: [%d], %d", objId, (int)instRefer);
        }
    }
}

void MObjectPool::deleteObject(muint_t objId)
{
    MMMutexLocker lck(&m_mutex);
    MObject *obj = nullptr;
    std::map<muint_t, MObject *>::iterator it = m_objMap.find(objId);
    if (it != m_objMap.end())
    {
        obj = it->second;
        if (GetAppCurrentThreadId() == obj->ownerTid())
        {
            m_objMap.erase(it);
            mDebug("deleteObject: [%d], %d", objId, (int)instRefer);
        }
        else
        {
            mWarn("Don't delete from other thread!");
            obj = nullptr;
        }
    }
    lck.unlock();
    if (obj != nullptr)
    {
        delete obj;
        if (derefInstance() <= 0)
        {
            delete this;
        }
    }
}

void MObjectPool::registerSlot(MSLOT slot, MObject::SlotFun act)
{
    MMMutexLocker lck(&m_mutex);
    m_slotMap[slot] = act;
}

// 执行槽函数：执行对象的id， 槽名，参数列表
void MObjectPool::doSlot(muint_t objId, MSLOT slot, const MVariantList &para)
{
    MMMutexLocker lck(&m_mutex);

    std::map<muint_t, MObject *>::iterator it = m_objMap.find(objId);
    if (it != m_objMap.end())
    {
        MObject *obj = it->second;
        if (GetAppCurrentThreadId() == obj->ownerTid()) // 在同一线程
        {
            MObject::SlotFun fn = m_slotMap[slot];
            lck.unlock();
            if (fn)
            {
                (obj->*fn)(para);
            }
        }
        else // 不在同一线程，把执行槽消息投递到obj所在线程中执行
        {
            mtid_t tid = obj->ownerTid();
            lck.unlock();
            MEventManager::instance()->postSlot(tid, objId, slot, para);
        }
    }
}

// 触发对象的信号：发出对象的id， 信号名，参数列表
void MObjectPool::emitSig(muint_t srcObjId, MSIG sig, const MVariantList &para)
{
    MMMutexLocker lck(&m_mutex);
    std::vector<MObject::ObjSlot> ssList;

    std::map<muint_t, MObject *>::iterator src_it = m_objMap.find(srcObjId);
    if (src_it == m_objMap.end())
    {
        mWarn("Not exist src object: %d", srcObjId);
        return;
    }

    MObject *src_obj = src_it->second;
    src_obj->findObjSlot(sig, ssList);
    if (ssList.size() <= 0)
    {
        mWarn("Not find the Sig: %d, %s", srcObjId, sig);
        return;
    }

    lck.unlock();

    for(auto ss : ssList) // 遍历所有的关联槽函数并执行
    {
        lck.lock();
        bool locked = true;
        std::map<muint_t, MObject *>::iterator dst_it
            = m_objMap.find(ss.objId);
        if (dst_it != m_objMap.end())
        {
            MObject *dst_obj = dst_it->second;
            if (GetAppCurrentThreadId() == dst_obj->ownerTid()) // 在相同的线程，直接执行即可
            {
                std::map<mstring_t, MObject::SlotFun>::iterator fun_it
                    = m_slotMap.find(ss.slot);
                if (fun_it != m_slotMap.end())
                {
                    MObject::SlotFun fn = fun_it->second;
                    lck.unlock(); // 执行之前先解锁
                    locked = false;
                    (dst_obj->*fn)(para);
                }
                else
                {
                    mWarn("Not register slot fun: %d, %s", ss.objId,
                             ss.slot);
                }
            }
            else // 不在同一线程，把执行槽消息投递到dst_obj所在线程中执行
            {
                mtid_t tid = dst_obj->ownerTid();
                lck.unlock(); // 执行之前先解锁
                locked = false;
                MEventManager::instance()->postSlot(tid, ss.objId, ss.slot, para);
            }
        }
        else
        {
            mWarn("Not exist dst object: %d", ss.objId);
        }
        if (locked)
        {
            lck.unlock();
        }
    }
}

MCORELIB_NAMESPACE_END
