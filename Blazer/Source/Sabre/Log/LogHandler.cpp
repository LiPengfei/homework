#include "Log/LogHandler.h"
#include "Log/LogManager.h"
#include "Structure/UniversalQueue.h"
#include "DesignPattern/Singleton.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BConsoleLogHandler                                                            */
/*-------------------------------------------------------------------------------------------*/
BConsoleLogHandler::BConsoleLogHandler()
{
}

BConsoleLogHandler::~BConsoleLogHandler()
{
}

BOOL BConsoleLogHandler::Init()
{
    BOOL bRetCode = FALSE;

    bRetCode = m_workerThread.Init();
    BZ_CHECK_RETURN_BOOL(bRetCode);

    bRetCode = m_workerThread.Start();
    BZ_CHECK_RETURN_BOOL(bRetCode);

    return TRUE;
}

BOOL BConsoleLogHandler::UnInit()
{
    BOOL bRetCode = FALSE;

    bRetCode = m_workerThread.SuspendThread();
    BZ_CHECK_RETURN_BOOL(bRetCode);

    bRetCode = m_workerThread.Stop();
    BZ_CHECK_RETURN_BOOL(bRetCode);

    return TRUE;
}

BOOL BConsoleLogHandler::Dispatch(IN CPCLogManager cpcLogManager)
{
    BUniversalQueueManagement<BSPLogRecord> *pQueueManager = BZ_SINGLETON_GET_PTR(BUniversalQueueManagement<BSPLogRecord>);
    BZ_CHECK_RETURN_BOOL(NULL != pQueueManager);
    BZ_CHECK_RETURN_BOOL(NULL != cpcLogManager);

    DWORD dwQueueID = BZ_HashString2ID(K_STRING_ID_OF_CONSOLE_LOG_HANDLER);
    BSPLogRecordQueue spLogRecordQueue ;
    BOOL  bRetCode  = pQueueManager->GetUniversalQueue(dwQueueID, spLogRecordQueue);
    BZ_CHECK_RETURN_BOOL(bRetCode);

    BSPLogRecord spLogRecord = cpcLogManager->GetLogRecord();
    BPackageHead head;
    if (spLogRecord)
    {

        BPackageHandler::GetHead(spLogRecord->m_cpContent, head);
        if (0 != head.m_cConsoleFlag)
        {
            spLogRecordQueue->PushNode(spLogRecord);
        }
    }

    return TRUE;
}

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BFileLogHandler                                                               */
/*-------------------------------------------------------------------------------------------*/
BFileLogHandler::BFileLogHandler()
{
}

BFileLogHandler::~BFileLogHandler()
{
}

BOOL BFileLogHandler::Init()
{
    BOOL bRetCode = FALSE;

    bRetCode = m_workerThread.Init();
    BZ_CHECK_RETURN_BOOL(bRetCode);

    bRetCode = m_workerThread.Start();
    BZ_CHECK_RETURN_BOOL(bRetCode);
    
    return TRUE;
}

BOOL BFileLogHandler::UnInit()
{
    return TRUE;
}

BOOL BFileLogHandler::Dispatch(IN CPCLogManager cpcLogManager)
{
    BUniversalQueueManagement<BSPLogRecord> *pQueueManager = BZ_SINGLETON_GET_PTR(BUniversalQueueManagement<BSPLogRecord>);
    BZ_CHECK_RETURN_BOOL(NULL != pQueueManager);
    BZ_CHECK_RETURN_BOOL(NULL != cpcLogManager);

    DWORD dwQueueID = BZ_HashString2ID(K_STRING_ID_OF_FILE_LOG_HANDLER);
    BSPLogRecordQueue spLogRecordQueue;
    BOOL  bRetCode  = pQueueManager->GetUniversalQueue(dwQueueID, spLogRecordQueue);
    BZ_CHECK_RETURN_BOOL(bRetCode);

    // modified by lipengfei 13/05/03
    BSPLogRecord spLogRecord = cpcLogManager->GetLogRecord();
     if (spLogRecord)
     {
         BPackageHead head;
         BPackageHandler::GetHead(spLogRecord->m_cpContent, head);
         if (0 != head.m_cFileFlag)
         {
             spLogRecordQueue->PushNode(spLogRecord);
         }
     }

    return TRUE;
}

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BNetLogHandler                                                                */
/*-------------------------------------------------------------------------------------------*/
BNetLogHandler::BNetLogHandler()
{
}

BNetLogHandler::~BNetLogHandler()
{
}

BOOL BNetLogHandler::Init()
{
    return TRUE;
}

BOOL BNetLogHandler::UnInit()
{
    return TRUE;
}

BOOL BNetLogHandler::Dispatch(IN CPCLogManager cpcLogManager)
{
    BUniversalQueueManagement<BSPLogRecord> *pQueueManager = BZ_SINGLETON_GET_PTR(BUniversalQueueManagement<BSPLogRecord>);
    BZ_CHECK_RETURN_BOOL(NULL != pQueueManager);
    BZ_CHECK_RETURN_BOOL(NULL != cpcLogManager);

    DWORD dwQueueID = BZ_HashString2ID(K_STRING_ID_OF_NET_LOG_HANDLER);
    BSPLogRecordQueue spLogRecordQueue;
    BOOL  bRetCode  = pQueueManager->GetUniversalQueue(dwQueueID, spLogRecordQueue);
    BZ_CHECK_RETURN_BOOL(bRetCode);

    BSPLogRecord spLogRecord = cpcLogManager->GetLogRecord();
//     if (spLogRecord && (spLogRecord->GetLogRecordType() & K_LOG_RECORD_TYPE_NET))
//     {
//         spLogRecordQueue->PushNode(spLogRecord);
//     }

    return TRUE;
}

BZ_DECLARE_NAMESPACE_END