#ifndef __BLAZER_SABRE_LOGRECORD_H__
#define __BLAZER_SABRE_LOGRECORD_H__

#include "Public.h"
#include "SmartPointer/SharedPtr.h"
#include "Log/LogAuxiliary.h"
#include "Structure/UniversalQueue.h"
#include "Log/LogPublic.h"
#include <string>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BLogRecord                                                                    */
/*-------------------------------------------------------------------------------------------*/
// class BLogRecord
// {
// private:
//     std::string      m_strLogContent;
//     BLogRecordDetail m_logRecordDetail;       
// 
// public:
//     BLogRecord();
//     ~BLogRecord();
// 
// public:
//     BOOL SetLogRecordDetail(
//         IN CONST BLogRecordDetail &detail
//         );
// 
//     BOOL SetLogRecordAuxiliary(
//         IN CONST BLogRecordAuxiliary &auxiliary
//         );
// 
//     BOOL SetLogRecordType(
//         IN CONST BYTE byLogRecordType = K_LOG_RECORD_TYPE_DEFAULT
//         );
// 
//     BOOL SetLogRecordContent(
//         IN const char *const cpcLogContent = NULL
//         );
// 
//     BOOL SetLogRecordContent(
//         IN CONST std::string &strLogContent
//         );
// 
//     BLogRecordDetail    GetLogRecordDetail()    CONST;
//     BLogRecordAuxiliary GetLogRecordAuxiliary() CONST;
//     BYTE                GetLogRecordType()      CONST;
//     std::string         GetLogRecordContent()   CONST;
// };

// class LogRecord modified by lipengfei 13/05/02
struct BLogRecord
{
    char m_cpContent[BZ_MAX_PACKAGE_DATA];

    BLogRecord()
    {
        BZ_ZeroMemory(m_cpContent, BZ_MAX_PACKAGE_DATA);
    }

    BLogRecord(const char *cpContent, int nLen)
    {
        memmove(m_cpContent, cpContent, nLen);
    }

};
// modified end

typedef       BLogRecord *            PLogRecord;
typedef CONST BLogRecord *            CPLogRecord;
typedef       BLogRecord * CONST      PCLogRecord;
typedef CONST BLogRecord * CONST      CPCLogRecord;

typedef BSharedPtr<BLogRecord>                     BSPLogRecord;
typedef BUniversalQueue<BSPLogRecord>              BLogRecordQueue;
typedef BSharedPtr<BLogRecordQueue>                BSPLogRecordQueue;
typedef BUniversalQueueManagement<BSPLogRecord>    BLogRecordQueueManagement;
typedef BLogRecordQueueManagement *                PLogRecordQueueManagement;

BZ_DECLARE_NAMESPACE_END

#endif