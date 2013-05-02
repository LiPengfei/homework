#include "Public.h"
#include "Synchronize/ThreadSemaphore.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#ifdef _WIN32 // windows

BThreadSemaphore::BThreadSemaphore(long lInitialCount, long lMaximumCount) throw (std::runtime_error)
{
    m_hSemaphore = ::CreateSemaphore(NULL, lInitialCount, lMaximumCount, NULL);
    if (NULL == m_hSemaphore)
    {
        throw std::runtime_error("semaphore init fails");
    }
}

BThreadSemaphore::~BThreadSemaphore()
{
    ::CloseHandle(m_hSemaphore);
}

int BThreadSemaphore::ReleaseSemaphore(long lReleaseNum)
{
    return ::ReleaseSemaphore(m_hSemaphore, lReleaseNum, NULL);
}

int BThreadSemaphore::WaitSemaphore(unsigned long dwMillSeconds)
{
    return WAIT_OBJECT_0 == ::WaitForSingleObject(m_hSemaphore, dwMillSeconds);
}

#else //linux

BThreadSemaphore::BThreadSemaphore(const pthread_mutexattr_t *mutexAtrr, 
    const pthread_condattr_t *condAttr)
{
    pthread_mutex_init(&m_mutex, mutexAtrr);
    pthread_cond_init (&m_cond,  condAttr);
}

BThreadSemaphore::~BThreadSemaphore()
{
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy (&m_cond);
}

int BThreadSemaphore::ReleaseSemaphore(long lReleaseNum)
{
    if (0 == lReleaseNum)
    {
        pthread_cond_broadcast(&m_cond);
    }
    else
    {
        pthread_cond_signal(&m_cond);
    }
}

int BThreadSemaphore::WaitSemaphore(unsigned long dwMillSeconds)
{
    m_mutex.lock(&m_mutex);
    pthread_cond_wait(&m_cond, &m_mutex);
    m_mutex.unlock(&m_mutex);
}

#endif

BZ_DECLARE_NAMESPACE_END
