#include <time.h>
#include "Tool/Time.h"

#pragma warning(disable:4996)

BZ_DECLARE_NAMESPACE_BEGIN(sabre)
  
typedef std::string STRING;

BLocalTime::BLocalTime()
{
}

BLocalTime::~BLocalTime()
{
}

DWORD BLocalTime::GetDay() CONST
{
    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);
    return sysTime.wDay;
}

DWORD BLocalTime::GetMonth() CONST
{
    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);
    return sysTime.wMonth;
}

DWORD BLocalTime::GetYear() CONST
{
    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);
    return sysTime.wYear;
}

STRING BLocalTime::GetCurDate(CONST CHAR *cszFormat) CONST
{
    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);

    CHAR szBuffer[64];
    ::memset(szBuffer, 0, 64);

    ::sprintf(szBuffer, cszFormat, sysTime.wYear, sysTime.wMonth, sysTime.wDay);
    return STRING(szBuffer);
}

STRING BLocalTime::GetCurTime(CONST CHAR *cszFormat) CONST
{
    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);

    CHAR szBuffer[64];
    ::memset(szBuffer, 0, 64);

    ::sprintf(szBuffer, cszFormat, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    return STRING(szBuffer);
}

STRING BLocalTime::GetCurDateAndTime(CONST CHAR *cszFormat) CONST
{
    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);

    CHAR szBuffer[64];
    ::memset(szBuffer, 0, 64);

    ::sprintf(szBuffer, cszFormat, 
        sysTime.wYear, sysTime.wMonth, sysTime.wDay,
        sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    return STRING(szBuffer);
}

BZ_DECLARE_NAMESPACE_END