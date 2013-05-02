#ifndef __BLAZER_SABRE_THREADMUTEX_H__
#define __BLAZER_SABRE_THREADMUTEX_H__

#include <stdexcept>
#include "Public.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#ifdef _WIN32
    static const int SPIN_TIME_DEF = 4000;
#else
#endif

class BThreadMutex : private BUnCopyable
{
#ifdef _WIN32 //windows
private:
    CRITICAL_SECTION m_cs;

public:
    BThreadMutex(int nSpin = SPIN_TIME_DEF) throw (std::runtime_error);

public:
    BOOL TryLock(); //if already locked, do nothing return FALSE, else lock, return TRUE

#else //linux
private:
    pthread_mutex_t m_cs;

public:
    BThreadMutex(const pthread_mutexattr_t *attr = NULL)
        throw (std::runtime_error);

#endif

// common
public:
    virtual ~BThreadMutex();

public:
    void Lock();
    void Unlock();
};

BZ_DECLARE_NAMESPACE_END

#endif
