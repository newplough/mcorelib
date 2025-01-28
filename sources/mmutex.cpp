/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "mplatform.h"
#include "mmutex.h"


#if (MOS_TYPE == MOS_RTOS)
#include "platform/rtos_mmutex.h"
#elif (MOS_TYPE == MOS_WIN)
#include "platform/win_mmutex.h"
#elif (MOS_TYPE == MOS_LINUX)
#include "platform/linux_mmutex.h"
#else
#error " ^_^ "
#endif

MCORELIB_NAMESPACE_BEGING

MMutex::MMutex()
	: platData(new MMutex_PlatformData())
{
}

MMutex::~MMutex()
{
	delete platData;
}

bool MMutex::lock()
{
	return platData->lock();
}
	
void MMutex::unlock()
{
	platData->unlock();
}

MCORELIB_NAMESPACE_END
