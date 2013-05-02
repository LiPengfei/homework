#include "Log/LogFormater.h"
#include "Log/LogAuxiliary.h"
#include <string>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

typedef std::string STRING;

BLogFormater::BLogFormater()
{
}

BLogFormater::~BLogFormater()
{
}

BOOL BLogFormater::FormatRecord(INOUT BLogRecord &logRecord)
{
//     BLogRecordDetail    detail    = logRecord.GetLogRecordDetail();
// 
//     // time stamp
//     if (detail.m_bIsAddTimestamp)
//     {
//         STRING content = logRecord.GetLogRecordContent();
//         content = m_localTime.GetCurTime() + " : " + content;
//         logRecord.SetLogRecordContent(content);
//     }
// 
     return TRUE;
}

BOOL BLogFormater::FormatRecord(INOUT BSPLogRecord &spLogRecord)
{
//     BZ_CHECK_RETURN_BOOL(false == !spLogRecord);
//     BLogRecordDetail detail    = spLogRecord->GetLogRecordDetail();
// 
//     // time stamp
//     if (detail.m_bIsAddTimestamp)
//     {
//         STRING content = spLogRecord->GetLogRecordContent();
//         content = m_localTime.GetCurTime() + " : " + content;
//         spLogRecord->SetLogRecordContent(content);
//     }

    return TRUE;
}

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BNetLogFormater                                                               */
/*-------------------------------------------------------------------------------------------*/
BNetLogFormater::BNetLogFormater()
{
}

BNetLogFormater::~BNetLogFormater()
{
}

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : KFileLogFormater                                                              */
/*-------------------------------------------------------------------------------------------*/
BFileLogFormater::BFileLogFormater()
{
}

BFileLogFormater::~BFileLogFormater()
{
}

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : KConsoleLogFormater                                                           */
/*-------------------------------------------------------------------------------------------*/
BConsoleLogFormater::BConsoleLogFormater()
{
}

BConsoleLogFormater::~BConsoleLogFormater()
{
}

BZ_DECLARE_NAMESPACE_END