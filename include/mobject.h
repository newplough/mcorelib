/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef _MOBJECT_H__
#define _MOBJECT_H__
#include "msigslot.h"
#include "mvariant.h"
#include "mdebug.h"
#include <map>

MCORELIB_NAMESPACE_BEGING

class MObject
{
public:
    MObject(const mstring_t &name = "");
    virtual ~MObject();

    // 对象ID
    inline muint_t objId() const
    {
        return m_objId;
    }

    // 对象名称
    inline const mstring_t& objName() const
    {
        return m_name;
    }

    // 所属线程ID
    inline mtid_t ownerTid() const
    {
        return m_ownerTid;
    }

    // 确保MObject是new出来的
    // 否则调用deleteLater后会导致崩溃
    void deleteLater();

public:
    // 定义slot函数的样子
    typedef void (MObject::*SlotFun)(const MVariantList &);

    // 注册slot函数：slot名称，slot函数
    static void registerSlot(MSLOT slot, SlotFun slotFunc);

    // 用信号名关联对象object的槽名称slot
    void connect(MSIG signal, const MObject &object, MSLOT slot);

    // 发出信号：signal名，参数列表
    void emitSig(MSIG signal, const MVariantList &);

    // 通过sig名字找出关联的objId和slot函数
    struct ObjSlot {
        muint_t objId;
        MSLOT slot;
    };
    void findObjSlot(MSIG signal, std::vector<ObjSlot> &ssList);

private:
    mstring_t m_name;
    muint_t m_objId;
    mtid_t m_ownerTid;
    std::multimap<mstring_t, ObjSlot> m_sigMap;
};

MCORELIB_NAMESPACE_END
#endif
