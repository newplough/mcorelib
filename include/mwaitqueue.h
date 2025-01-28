/****************************************************************************
** This file is part of the MCoreLib library.
** SPDX-FileCopyrightText: 2025 newplough<newplough@126.com>
** SPDX-License-Identifier: MIT
****************************************************************************/
#ifndef _MWAIT_QUEUE_H__
#define _MWAIT_QUEUE_H__
#include "mmutex.h"
#include "msemaphore.h"
#include <queue>

MCORELIB_NAMESPACE_BEGING

// 等待队列, 队列满时入队可以等待，队列空时出队可以等待
template <class T> class MWaitQueue
{
    typename std::queue<T>::size_type m_max;
    
    std::queue<T> m_queue;
    std::atomic_bool m_quit;
    
    MMutex m_mutex;
    MSemaphore m_semEmpty;
    MSemaphore m_semFull;
    
public:
    MWaitQueue(size_t size_max = 1024)
        : m_max(size_max)
        , m_quit(false)
    {
    }

    ~MWaitQueue()
    {
    }

    inline bool isActive() const
    {
        return !m_quit;
    }

    bool push(const T &data)
    {
        MMMutexLocker lock(&m_mutex);
        if (!m_quit)
        {
            if (m_queue.size() < m_max)
            {
                m_queue.push(std::move(data));
                m_semEmpty.notify();
                return true;
            }
            else
            {
                m_semEmpty.notify();
            }
        }

        return false;
    }

    bool pop(T &data, bool wait = true)
    {
        MMMutexLocker lock(&m_mutex);
        while (!m_quit)
        {
            if (!m_queue.empty())
            {
                // data = std::move(_queue.front());
                data = m_queue.front();
                m_queue.pop();
                return true;
            }
            else if (m_quit)
            {
                return false;
            }
            else if (wait)
            {
                lock.unlock();
                m_semEmpty.wait();
                lock.lock();
            }
            else 
            {
                break;
            }
        }
        return false;
    }

    // The queue has finished accepting input
    inline void quit()
    {
        m_quit.store(true);
        m_semEmpty.notify();
    }

    inline int size()
    {
        MMMutexLocker lock(&m_mutex);
        return static_cast<int>(m_queue.size());
    }
};

MCORELIB_NAMESPACE_END
#endif
