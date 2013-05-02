#ifndef __BZ_SABRE_THREADSYNCHRONIZE_H__
#define __BZ_SABRE_THREADSYNCHRONIZE_H__

#include "Public.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

inline VOID g_MilliSleep(DWORD dwMilliseconds)
{
#ifdef WIN32
    ::Sleep(dwMilliseconds);
#else
    ::usleep(dwMilliseconds * 1000);
#endif
}

inline VOID g_Sleep(DWORD dwSeconds)
{
#ifdef WIN32
    ::Sleep(dwSeconds * 1000);
#else
    ::sleep(dwSeconds);
#endif
}

template <class MutexType>
class BGuard : private BUnCopyable
{
private:
    MutexType &m_mutex;

public:
    explicit BGuard(MutexType &mutex) : m_mutex(mutex) { m_mutex.Lock(); }
    ~BGuard() { m_mutex.Unlock(); }
};

BZ_DECLARE_NAMESPACE_END

#endif

