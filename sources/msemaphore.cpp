/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "mplatform.h"
#include "msemaphore.h"


#if (MOS_TYPE == MOS_RTOS)
#include "platform/rtos_msemaphore.h"
#elif (MOS_TYPE == MOS_WIN)
#include "platform/win_msemaphore.h"
#elif (MOS_TYPE == MOS_LINUX)
#include "platform/linux_msemaphore.h"
#else
#error " ^_^ "
#endif

MCORELIB_NAMESPACE_BEGING

MSemaphore::MSemaphore(muint_t n)
	: platData(new MSemaphore_PlatformData(n))
{
}

MSemaphore::~MSemaphore()
{
	delete platData;
}

void MSemaphore::notify()
{
	platData->notify();
}
	

void MSemaphore::wait()
{
	platData->wait();
}

bool MSemaphore::wait(mllong_t mesc)
{
	return platData->wait(mesc);
}
	
MCORELIB_NAMESPACE_END
