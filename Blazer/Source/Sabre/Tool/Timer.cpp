#include "Tool/Timer.h"

#ifdef WIN32
#else
#include <sys/time.h>
#endif

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

BTimer::BTimer()
{
#ifdef WIN32
    ::QueryPerformanceFrequency(&m_liFrequency);
#else
#endif
}

BTimer::~BTimer()
{
}

VOID BTimer::Start()
{
#ifdef WIN32
    ::QueryPerformanceCounter(&m_liStartTime);
#else
    ::gettimeofday(&m_tvStartTime, NULL);
#endif
}

VOID BTimer::Stop()
{
#ifdef WIN32
    ::QueryPerformanceCounter(&m_liStopTime);
#else
    ::gettimeofday(&m_tvStopTime, NULL);
#endif
}

DWORD BTimer::GetElapseInSec() CONST
{
    return GetElapseInMilliSec() / 1000;
}

DWORD BTimer::GetElapseInMilliSec() CONST
{
    DWORD dwElapseInSec = 0;
#ifdef WIN32
    LARGE_INTEGER liNowTime;
    ::QueryPerformanceCounter(&liNowTime);
    dwElapseInSec = (DWORD)((liNowTime.QuadPart - m_liStartTime.QuadPart) * 1000 / m_liFrequency.QuadPart);
#else
    timeval tvNowTime;
    ::gettimeofday(&tvNowTime, NULL);
    dwElapseInSec = (DWORD)((tvNowTime.tv_sec - m_tvStartTime.tv_sec) * 1000 + tv_sec.tv_usec / 1000);
#endif
    return dwElapseInSec;
}

BOOL  BTimer::HasTimePassedInSec(DWORD dwSec) CONST
{
    if (GetElapseInSec() >= dwSec)
    {
        return TRUE;
    }
    return FALSE;
}

BOOL  BTimer::HasTimePassedInMilliSec(DWORD dwMilliSec) CONST
{
    if (GetElapseInMilliSec() >= dwMilliSec)
    {
        return TRUE;
    }
    return FALSE;
}

BZ_DECLARE_NAMESPACE_END