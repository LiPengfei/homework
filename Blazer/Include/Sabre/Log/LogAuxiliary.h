#ifndef __BLAZER_SABRE_LOGAUXILIARY_H__
#define __BLAZER_SABRE_LOGAUXILIARY_H__

#include "Public.h"
#include "LogPublic.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

/*-------------------------------------------------------------------------------------------*/
/* UNION     : BLogRecordAuxiliary                                                           */
/*-------------------------------------------------------------------------------------------*/
union BLogRecordAuxiliary
{
    struct _BNetLogRecordAuxiliary
    {
        DWORD m_dwNetID;
    } m_netLogAuxiliary;

    struct _BFileLogRecordAuxiliary
    {
        DWORD m_dwLogFileID;
    } m_fileLogAuxiliary;

    struct _BConsoleLogRecordAuxiliary
    {
        DWORD m_dwSimulationWindowID;
        WORD  m_wConsoleLogColor;
    } m_consoleLogAuxiliary;
};

/*-------------------------------------------------------------------------------------------*/
/* STRUCT    : BLogRecordDetail                                                              */
/*-------------------------------------------------------------------------------------------*/
typedef struct BLogRecordDetail
{
    BYTE                m_byLogRecordType;  // log record type
    BOOL                m_bIsAddTimestamp;  // add time stamp or not
    BLogRecordAuxiliary m_auxiliary;        // log record auxiliary info

    BLogRecordDetail() : m_byLogRecordType(K_LOG_RECORD_TYPE_DEFAULT), m_bIsAddTimestamp(FALSE)
    {
    }

} *PLogRecordDetail;

BZ_DECLARE_NAMESPACE_END

#endif