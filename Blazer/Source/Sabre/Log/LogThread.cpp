#include "Log/LogHandler.h"
#include "File/File.h"
#include "Structure/UniversalQueue.h"
#include "DesignPattern/Singleton.h"
#include "Structure/SimpleString.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)
   
typedef std::string STRING;

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BConsoleLogThread                                                             */
/*-------------------------------------------------------------------------------------------*/
BConsoleLogThread::BConsoleLogThread()
{
}

BConsoleLogThread::~BConsoleLogThread()
{
}

BOOL BConsoleLogThread::Init()
{
    PLogFormater pLogFormater = ::new BConsoleLogFormater();
    BZ_CHECK_RETURN_BOOL(NULL != pLogFormater);

    m_spLogFormater = BSPLogFormater(pLogFormater);
    return TRUE;
}

UINT BConsoleLogThread::Run()
{
    BOOL bLoopFlag = TRUE;

    PLogRecordQueueManagement pQueueManager = SINGLETON_GET_PTR(BLogRecordQueueManagement);
    BZ_CHECK_RETURN_CODE(NULL != pQueueManager, 0);

    DWORD dwQueueID = g_HashString2ID(K_QUEUE_KEY_LOG_RECORD_CONSOLE);
    BSPLogRecordQueue spLogRecordQueue = pQueueManager->GetUniversalQueue(dwQueueID);
    BZ_CHECK_RETURN_CODE(false == !spLogRecordQueue, 0);

    while(bLoopFlag)
    {
        BSPLogRecord  spLogRecord      = spLogRecordQueue->PopNode();
        BSimpleString ssRes            = BPackageHandler::GetData(spLogRecord->m_cpContent);

        printf("%s\n", ssRes.m_cpStr);
    }

    return 0;
}

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BFileLogThread                                                                */
/*-------------------------------------------------------------------------------------------*/
BFileLogThread::BFileLogThread()
{
}

BFileLogThread::~BFileLogThread()
{
}

BOOL BFileLogThread::Init()
{
    PLogFormater pLogFormater = ::new BFileLogFormater();
    BZ_CHECK_RETURN_BOOL(NULL != pLogFormater);

    m_spLogFormater = BSPLogFormater(pLogFormater);
    return TRUE;
}

UINT BFileLogThread::Run()
{
    BOOL bLoopFlag = TRUE;
    BOOL bRetCode  = FALSE;

    PLogRecordQueueManagement pQueueManager = SINGLETON_GET_PTR(BLogRecordQueueManagement);
    BZ_CHECK_RETURN_BOOL(NULL != pQueueManager);

    DWORD dwQueueID = g_HashString2ID(K_QUEUE_KEY_LOG_RECORD_FILE);
    BSPLogRecordQueue spLogRecordQueue = pQueueManager->GetUniversalQueue(dwQueueID);
    BZ_CHECK_RETURN_BOOL(false == !spLogRecordQueue);

    while(bLoopFlag)
    {
        //need rewrite
/*        KSPSmallFile spSmallFile;*/
        BSPLogRecord spLogRecord = spLogRecordQueue->PopNode();

//         if (!spLogRecord && !(spLogRecord->GetLogRecordType() & K_LOG_RECORD_TYPE_FILE))
//             continue;
// 
//         if(!m_spLogFormater->FormatRecord(spLogRecord))
//             continue;
// 
//         BLogRecordDetail    detail    = spLogRecord->GetLogRecordDetail();
//         BLogRecordAuxiliary &auxiliary = detail.m_auxiliary;
// 
//         CONST DWORD dwFileID     = auxiliary.m_fileLogAuxiliary.m_dwLogFileID;
//         CONST STRING &strContent = spLogRecord->GetLogRecordContent();
// 
//         PSmallFileManager pSmallFileManager = SINGLETON_GET_PTR(KSmallFileManager);
//         bRetCode = pSmallFileManager->Get(dwFileID, spSmallFile);
// 
//         if (!bRetCode || !spSmallFile)
//             continue;
// 
//         spSmallFile->WriteLineImmediate(strContent.c_str(), strContent.size());
    }

    return 0;
}

BNetLogThread::BNetLogThread()
{
}

BNetLogThread::~BNetLogThread()
{
}

UINT BNetLogThread::Run()
{
    BOOL bLoopFlag = TRUE;

    PLogRecordQueueManagement pQueueManager = SINGLETON_GET_PTR(BLogRecordQueueManagement);
    BZ_CHECK_RETURN_BOOL(NULL != pQueueManager);

    DWORD dwQueueID = g_HashString2ID(K_QUEUE_KEY_LOG_RECORD_NET);
    BSPLogRecordQueue spLogRecordQueue = pQueueManager->GetUniversalQueue(dwQueueID);
    BZ_CHECK_RETURN_BOOL(false == !spLogRecordQueue);

    while(bLoopFlag)
    {
         BSPLogRecord spLogRecord = spLogRecordQueue->PopNode();
// 
//         if (!spLogRecord && !(spLogRecord->GetLogRecordType() & K_LOG_RECORD_TYPE_NET))
//             continue;
    }

    return 0;
}

BZ_DECLARE_NAMESPACE_END