/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "mlogger.h"
#include "mplatform.h"
#include "mtimertask.h"

MCORELIB_NAMESPACE_BEGING

// Timer
MTimer::MTimer(Type type)
    : MObject()
    , m_type(type)
    , m_interval(0)
    , m_session(-1)
{
    mDebug("$_%d", __FUNCTION__,objId());
}

MTimer::~MTimer()
{
    mDebug("#_%d", __FUNCTION__,objId());
    MTimerTask::instance()->removeTimer(objId());
}

void MTimer::start(muint_t msecs)
{
    if (msecs > 0)
    {
        m_interval = msecs;
        m_session = (m_session < 0) ? (-m_session) : (m_session + 1);
        MTimerTask::instance()->addTimer(this);
    }
    else
    {
        mWarn("can't start a negtive timer!");
    }
}

void MTimer::stop()
{
    m_session = (m_session > 0) ? (-m_session - 1) : (m_session - 1);
    MTimerTask::instance()->stopTimer(objId());
}

MAKE_SLOT_1(MTimer, onTimeout, mint_t session)
{
    if (session == m_session)
    {
        EMIT_SIG_0(this, MTimer::timeout);
    }
    else
    {
        mWarn("session: %d, %d", session, m_session);
    }
}

MCORELIB_NAMESPACE_END
