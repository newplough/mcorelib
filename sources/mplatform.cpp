/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#include "mplatform.h"

#if (MOS_TYPE == MOS_WIN)
#include <windows.h>
#include <thread>
#include <chrono>
#include <sstream>

#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")
#endif // _MSC_VER

#elif (MOS_TYPE == MOS_LINUX)
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <pthread.h>


#elif (MOS_TYPE == MOS_RTOS)
#include "cmsis_os.h"
#endif
#include "mthread.h"


MCORELIB_NAMESPACE_BEGING

mtid_t GetOsCurrentThreadId(void)
{
    return MThread::getOsCurrentThreadId();
}

mtid_t GetAppCurrentThreadId(void)
{
    return MThread::getCurrentThreadId();
}

void SleepMsec(unsigned int msec)
{
    MThread::sleep(msec);
}

#if (MOS_TYPE == MOS_LINUX)

// 获取系统tick数
mllong_t GetOsTick(void)
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);

    return (tp.tv_sec * 1000 + tp.tv_nsec / 1000000);
}

// 获取系统当前日期
void GetOsCurrentDate(mdate_t& date)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    date.year = 1900 + ltm->tm_year;
    date.month = 1 + ltm->tm_mon;
    date.day =  ltm->tm_mday;
}

// 获取系统当前时间
void GetOsCurrentTime(mtime_t& time)
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    time_t seconds = tv.tv_sec;
    // 转换为tm结构体以获取时分秒信息
    struct tm* tm_info = localtime(&seconds);
    time.hour = tm_info->tm_hour;
    time.minute = tm_info->tm_min;
    time.second = tm_info->tm_sec;
    time.msec = tv.tv_usec / 1000;
}

#endif // __linux__

#if (MOS_TYPE == MOS_WIN)

template <typename C> double getClockPrecision()
{
    using namespace std;
    typedef typename C::period P; // type of time unit
    if (ratio_less_equal<P, milli>::value)
    {
        // convert to and print as milliseconds
        typedef typename ratio_multiply<P, kilo>::type TT;
        return double(TT::num) / TT::den;
    }
    else // seconds
    { 	
        return  double(P::num) * 1000 / P::den ;
    }
}

// 获取系统tick数
mllong_t GetOsTick(void)
{
	static  double precision = getClockPrecision<std::chrono::steady_clock>();
	return (mllong_t)(std::chrono::steady_clock::now().time_since_epoch().count()*precision);
}

// 获取系统当前日期
void GetOsCurrentDate(mdate_t& date)
{
    SYSTEMTIME st;
    GetSystemTime(&st);

    date.year = st.wYear;;
    date.month = st.wMonth;
    date.day = st.wDay;
}

// 获取系统当前时间
void GetOsCurrentTime(mtime_t& time)
{
    SYSTEMTIME st;
    GetSystemTime(&st);

    time.hour = st.wHour;
    time.minute = st.wMinute;
    time.second = st.wSecond;
    time.msec = st.wMilliseconds;
}

#endif // _WIN32

#if (MOS_TYPE == MOS_RTOS)

//-在其他操作系统中需要实现下列函数
//-! : 获取系统tick数
extern "C" uint64_t _GetSysTick(void);

//-! :获取当前日期
extern "C" void _GetOsCurrentDate(uint16_t* year,
        uint8_t*  month,
        uint8_t*  day);

//-! : 获取当前时间
extern "C" void _GetOsCurrentTime(uint8_t* hour,
        uint8_t* minute,
        uint8_t* second,
        uint16_t* msec);


// 获取系统tick数
mllong_t GetOsTick(void)
{
    return _GetSysTick();
}

// 获取系统当前日期
void GetOsCurrentDate(mdate_t& date)
{
    _GetOsCurrentDate(&date.year, &date.month, &date.day);
}

// 获取系统当前时间
void GetOsCurrentTime(mtime_t& time)
{
    _GetOsCurrentTime(&time.hour, &time.minute, &time.second, &time.msec);
}

#endif

MCORELIB_NAMESPACE_END
