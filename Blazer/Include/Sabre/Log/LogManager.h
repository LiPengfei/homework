#ifndef __BLAZER_SABRE_LOGMANAGER_H__
#define __BLAZER_SABRE_LOGMANAGER_H__

#include "Public.h"
#include "Log/LogRecord.h"
#include "Log/LogHandler.h"
#include "SmartPointer/SharedPtr.h"
#include <vector>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BLogManager                                                                   */
/*-------------------------------------------------------------------------------------------*/
class BLogManager : private BUnCopyable
{
private:
    typedef std::vector<BSPLogHandler> BLogHandlerVector;

private:
    BLogHandlerVector  m_logHandlerVector;                  // all log handler vector
    BSPLogRecord       m_spLogRecord;                       // the current log record

public:
    BLogManager();
    ~BLogManager();

public:
    BOOL Init();
    BOOL NotifyHandler();

    BOOL AttachHandler(IN CONST BSPLogHandler &spLogHandler);
    BOOL DetachHandler(IN CONST BSPLogHandler &spLogHandler);

    BOOL AttachHandler(IN CPLogHandler cpLogHandler = NULL);
    BOOL DetachHandler(IN CPLogHandler cpLogHandler = NULL);

    BOOL DetachHandler(IN CONST DWORD dwLogHandlerID = 0);

    BOOL SetLogRecord(IN CONST BSPLogRecord &spLogRecord);
    BSPLogRecord GetLogRecord() CONST;
};

typedef       BLogManager *       PLogManager;
typedef       BLogManager * CONST CPLogManager;     // equal to "CONST PLogManager" type
typedef CONST BLogManager *       PCLogManager;
typedef CONST BLogManager * CONST CPCLogManager;

typedef BSharedPtr<BLogManager>   BSPLogManager;

inline BOOL g_AttachLogHandler(
    IN       BLogManager   &manager     ,
    IN CONST BSPLogHandler &spLogHandler)
{
    return manager.AttachHandler(spLogHandler);
}

inline BOOL g_DetachHandler(
    IN       BLogManager   &manager     ,
    IN CONST BSPLogHandler &spLogHandler)
{
    return manager.DetachHandler(spLogHandler);
}

inline void BZ_WriteLog(
    IN       BLogManager  &manager    ,
    IN CONST BSPLogRecord &spLogRecord)
{
    BOOL bRetCode = manager.SetLogRecord(spLogRecord);
    BZ_CHECK_RETURN_VOID(bRetCode);

    bRetCode = manager.NotifyHandler();
    BZ_CHECK_RETURN_VOID(bRetCode);
}

BZ_DECLARE_NAMESPACE_END

#endif