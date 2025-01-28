/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef M_MUTEX_H
#define M_MUTEX_H
#include "mmacroutils.h"

MCORELIB_NAMESPACE_BEGING

class MMutex
{
	DECLARE_PLATFORM_DATA(MMutex)
	
public:
	MMutex();
   ~MMutex();

	bool lock();
	void unlock();
};

// MMutex使用简化助手
// 注意最好只在函数內构造局部MMMutexLocker对象
class MMMutexLocker final
{
public:
    explicit MMMutexLocker(MMutex *m) 
        : _mutex(m), _locked(0)
    {
        unlock();
    }

    ~MMMutexLocker() 
    { 
        unlock(); 
    }

    inline void lock()
    {
        if (!_locked)
        {
            _locked = 1;
            _mutex->lock();
        }
    }
    
    inline void unlock()     
    {
        if (_locked)
        {
            _mutex->unlock();
            _locked = false;
        }
    }

private:
    DISABLE_CLASS_COPY(MMMutexLocker)  
    MMutex* _mutex;
    bool _locked;
};


MCORELIB_NAMESPACE_END
#endif
