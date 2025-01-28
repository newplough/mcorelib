/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef _MSEMAPHORE_H__
#define _MSEMAPHORE_H__
#include "mmacroutils.h"

MCORELIB_NAMESPACE_BEGING

class MSemaphore
{
    DECLARE_PLATFORM_DATA(MSemaphore)

public:
    MSemaphore(muint_t n = 0);

    ~MSemaphore();

    void notify();

    void wait();

    bool wait(mllong_t mesc);
};

MCORELIB_NAMESPACE_END
#endif
