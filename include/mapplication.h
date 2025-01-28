/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef _MAPPLICATION_H_
#define _MAPPLICATION_H_
#include "mmacroutils.h"

MCORELIB_NAMESPACE_BEGING

#define mApp (MApplication::instance)

// 应用程序
class MApplication
{
    DISABLE_CLASS_COPY(MApplication)

public:
    MApplication();
    ~MApplication();

    static MApplication *instance;

    // 设置应用程序优先级
    // 在一些系统中需要高级权限才能调用该接口
    void setPriority(int prio);

    // 应用程序执行，启动事件循环
    // 如果没有主动退出，一直在这儿循环
    int exec();

    // 退出应用程序
    void quit();

private:
    void init();

private:
    DECLARE_PRIVATE_DATA(MApplication)
};

MCORELIB_NAMESPACE_END

#endif
