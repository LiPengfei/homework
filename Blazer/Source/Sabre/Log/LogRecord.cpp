#include "Log/LogRecord.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

typedef std::string STRING;

// BLogRecord::BLogRecord()
// {
// }
// 
// BLogRecord::~BLogRecord()
// {
// }
// 
// BOOL BLogRecord::SetLogRecordDetail(IN CONST BLogRecordDetail &detail)
// {
//     m_logRecordDetail = detail;
//     return TRUE;
// }
// 
// BOOL BLogRecord::SetLogRecordAuxiliary(IN CONST BLogRecordAuxiliary &auxiliary)
// {
//     m_logRecordDetail.m_auxiliary = auxiliary;
//     return TRUE;
// }
// 
// BOOL BLogRecord::SetLogRecordType(IN CONST BYTE byLogRecordType)
// {
//     m_logRecordDetail.m_byLogRecordType = byLogRecordType;
//     return TRUE;
// }
// 
// BOOL BLogRecord::SetLogRecordContent(IN const char *const cpcLogContent)
// {
//     BZ_CHECK_C_STRING_RETURN_BOOL(cpcLogContent);
//     m_strLogContent = cpcLogContent;
//     return TRUE;
// }
// 
// BOOL BLogRecord::SetLogRecordContent(IN CONST STRING &strLogContent)
// {
//     m_strLogContent = strLogContent;
//     return TRUE;
// }
// 
// BLogRecordDetail BLogRecord::GetLogRecordDetail() CONST
// {
//     return m_logRecordDetail;
// }
// 
// BLogRecordAuxiliary BLogRecord::GetLogRecordAuxiliary() CONST
// {
//     return m_logRecordDetail.m_auxiliary;
// }
// 
// BYTE BLogRecord::GetLogRecordType() CONST
// {
//     return m_logRecordDetail.m_byLogRecordType;
// }
// 
// STRING BLogRecord::GetLogRecordContent() CONST
// {
//     return m_strLogContent;
// }

BZ_DECLARE_NAMESPACE_END