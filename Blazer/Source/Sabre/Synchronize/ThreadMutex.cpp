#include "Synchronize/ThreadMutex.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#ifdef _WIN32
    
BThreadMutex::BThreadMutex(int nSpin) throw (std::runtime_error)
{
    if (!::InitializeCriticalSectionAndSpinCount(&m_cs, nSpin))
    {
        throw std::runtime_error("InitializeCriticalSectionAndSpinCount fails");
    }
}

BThreadMutex::~BThreadMutex()
{
    ::DeleteCriticalSection(&m_cs);
}

BOOL BThreadMutex::TryLock()
{
    return ::TryEnterCriticalSection(&m_cs);
}

void BThreadMutex::Lock()
{
    ::EnterCriticalSection(&m_cs);
}

void BThreadMutex::Unlock()
{
    ::LeaveCriticalSection(&m_cs);
}

#else

BThreadMutex::BThreadMutex(const pthread_mutexattr_t *attr)
    throw (std::runtime_error)
{
    pthread_mutex_init(&m_cs, attr);
}

BThreadMutex::~BThreadMutex()
{
    pthread_mutex_destroy(&m_cs);
}

void BThreadMutex::Lock()
{
    pthread_mutex_lock(&m_cs);
}

void BThreadMutex::Unlock()
{
    pthread_mutex_unlock(&m_cs);
}

#endif

BZ_DECLARE_NAMESPACE_END
