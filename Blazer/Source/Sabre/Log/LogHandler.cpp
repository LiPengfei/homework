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
    BUniversalQueueManagement<BSPLogRecord> *pQueueManager = SINGLETON_GET_PTR(BUniversalQueueManagement<BSPLogRecord>);
    BZ_CHECK_RETURN_BOOL(NULL != pQueueManager);
    BZ_CHECK_RETURN_BOOL(NULL != cpcLogManager);

    DWORD dwQueueID = g_HashString2ID(K_QUEUE_KEY_LOG_RECORD_CONSOLE);
    BSPLogRecordQueue spLogRecordQueue = pQueueManager->GetUniversalQueue(dwQueueID);
    BZ_CHECK_RETURN_BOOL(false == !spLogRecordQueue);

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
    BUniversalQueueManagement<BSPLogRecord> *pQueueManager = SINGLETON_GET_PTR(BUniversalQueueManagement<BSPLogRecord>);
    BZ_CHECK_RETURN_BOOL(NULL != pQueueManager);
    BZ_CHECK_RETURN_BOOL(NULL != cpcLogManager);

    DWORD dwQueueID = g_HashString2ID(K_QUEUE_KEY_LOG_RECORD_FILE);
    BSPLogRecordQueue spLogRecordQueue = pQueueManager->GetUniversalQueue(dwQueueID);
    BZ_CHECK_RETURN_BOOL(spLogRecordQueue);

    BSPLogRecord spLogRecord = cpcLogManager->GetLogRecord();
//     if (spLogRecord && (spLogRecord->GetLogRecordType() & K_LOG_RECORD_TYPE_FILE))
//     {
//         spLogRecordQueue->PushNode(spLogRecord);
//     }

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
    BUniversalQueueManagement<BSPLogRecord> *pQueueManager = SINGLETON_GET_PTR(BUniversalQueueManagement<BSPLogRecord>);
    BZ_CHECK_RETURN_BOOL(NULL != pQueueManager);
    BZ_CHECK_RETURN_BOOL(NULL != cpcLogManager);

    DWORD dwQueueID = g_HashString2ID(K_QUEUE_KEY_LOG_RECORD_NET);
    BSPLogRecordQueue spLogRecordQueue = pQueueManager->GetUniversalQueue(dwQueueID);
    BZ_CHECK_RETURN_BOOL(spLogRecordQueue);

    BSPLogRecord spLogRecord = cpcLogManager->GetLogRecord();
//     if (spLogRecord && (spLogRecord->GetLogRecordType() & K_LOG_RECORD_TYPE_NET))
//     {
//         spLogRecordQueue->PushNode(spLogRecord);
//     }

    return TRUE;
}

BZ_DECLARE_NAMESPACE_END