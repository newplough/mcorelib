/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef _MTIMER_H__
#define _MTIMER_H__
#include "mobject.h"
MCORELIB_NAMESPACE_BEGING

// 定时器，提供两种类型：
// 周期性触发和单次触发
class MTimer : public MObject
{
    friend class MTimerTask;

public:
    enum Type { ONCE = 0, CIRCLE = 1 };

    MTimer(Type type = CIRCLE);

    ~MTimer();

    void start(muint_t msecs);

    void stop();

    inline bool isActive() const
    {
        return (m_session >= 0);
    }

    DECLARE_SIG_0(timeout)

private:
    DECLARE_SLOT_1(MTimer, onTimeout, mint_t)

private:
    Type m_type;
    muint_t m_interval;
    mint_t m_session;
};

MCORELIB_NAMESPACE_END
#endif
