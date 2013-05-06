#ifndef __BLAZER_SABRE_TIMER_H__
#define __BLAZER_SABRE_TIMER_H__

#include "Public.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BTimer : private BUnCopyable
{
private:
#ifdef WIN32
    LARGE_INTEGER m_liFrequency;
    LARGE_INTEGER m_liStartTime;
    LARGE_INTEGER m_liStopTime;
#else
    timeval m_tvStartTime;
    timeval m_tvStopTime;
#endif

public:
    BTimer();
    ~BTimer();

public:
    void  Start();
    void  Stop();
    DWORD GetElapseInSec() CONST;
    DWORD GetElapseInMilliSec() CONST;
    BOOL  HasTimePassedInSec(DWORD dwSec) CONST;
    BOOL  HasTimePassedInMilliSec(DWORD dwMilliSec) CONST;
};

BZ_DECLARE_NAMESPACE_END

#endif

