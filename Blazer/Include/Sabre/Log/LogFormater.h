#ifndef __BLAZER_SABRE_LOGFORMATER_H__
#define __BLAZER_SABRE_LOGFORMATER_H__

#include "Tool/Time.h"
#include "Log/LogRecord.h"
#include "SmartPointer/SharedPtr.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BLogFormater                                                                  */
/*-------------------------------------------------------------------------------------------*/
class BLogFormater : private BUnCopyable
{
protected:
    BLocalTime m_localTime;

public:
    BLogFormater();
    virtual ~BLogFormater();

public:
    virtual BOOL FormatRecord(INOUT BLogRecord &logRecord);
    virtual BOOL FormatRecord(INOUT BSPLogRecord &spLogRecord);
};

typedef       BLogFormater *       PLogFormater;
typedef       BLogFormater * CONST CPLogFormater;       // equal to "CONST PLogFormater" type
typedef CONST BLogFormater *       PCLogFormater;
typedef CONST BLogFormater * CONST CPCLogFormater;

typedef BSharedPtr<BLogFormater>   BSPLogFormater;

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BNetLogFormater                                                               */
/*-------------------------------------------------------------------------------------------*/
class BNetLogFormater : public BLogFormater
{
public:
    BNetLogFormater();
    ~BNetLogFormater();
};

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BFileLogFormater                                                              */
/*-------------------------------------------------------------------------------------------*/
class BFileLogFormater : public BLogFormater
{
public:
    BFileLogFormater();
    ~BFileLogFormater();
};

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BConsoleLogFormater                                                           */
/*-------------------------------------------------------------------------------------------*/
class BConsoleLogFormater : public BLogFormater
{
public:
    BConsoleLogFormater();
    ~BConsoleLogFormater();
};

BZ_DECLARE_NAMESPACE_END

#endif