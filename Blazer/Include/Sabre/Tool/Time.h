#ifndef __BLAZER_SABRE_TIME_H__
#define __BLAZER_SABRE_TIME_H__

#include "Public.h"
#include <string>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BLocalTime : private BUnCopyable
{
public:
    BLocalTime();
    ~BLocalTime();

public:
    DWORD  GetDay()   CONST;
    DWORD  GetMonth() CONST;
    DWORD  GetYear()  CONST;
    std::string GetCurDate(CONST CHAR *cszFormat = "%04d/%02d/%02d") CONST;
    std::string GetCurTime(CONST CHAR *cszFormat = "%02d:%02d:%02d") CONST;
    std::string GetCurDateAndTime(CONST CHAR *cszFormat = "%04d/%02d/%02d %02d:%02d:%02d") CONST;
};

BZ_DECLARE_NAMESPACE_END

#endif

