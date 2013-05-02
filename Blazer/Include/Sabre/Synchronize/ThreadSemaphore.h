#ifndef __BLAZER_SABRE_THREADSEMAPHORE_H__
#define __BLAZER_SABRE_THREADSEMAPHORE_H__

#include "Public.h"
#include <stdexcept>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

    static const long SEMAPHORE_MAXIMUM_DEF = 32;

class BThreadSemaphore : public BUnCopyable
{
#ifdef _WIN32
private:
    HANDLE          m_hSemaphore;

public:
    BThreadSemaphore(long lInitialCount = 0,
        long lMaximumCount = SEMAPHORE_MAXIMUM_DEF)
        throw (std::runtime_error);

    virtual ~BThreadSemaphore();

#else // for linux
private:
    pthread_mutex_t m_mutex;
    pthread_cond_t  m_cond;

public:
    BThreadSemaphore(const pthread_mutexattr_t *mutexAtrr, 
        const pthread_condattr_t *condAttr);
#endif

public:
    int ReleaseSemaphore(long n = 1);
    int WaitSemaphore(unsigned long dsMillisecondes = INFINITE);
};

BZ_DECLARE_NAMESPACE_END
#endif
