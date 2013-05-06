#include "Log/LogHandler.h"
#include "File/File.h"
#include "Structure/UniversalQueue.h"
#include "DesignPattern/Singleton.h"
#include "Structure/SimpleString.h"
#include "Log/LogRecord.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)
   
typedef std::string STRING;

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BConsoleLogThread                                                             */
/*-------------------------------------------------------------------------------------------*/
BConsoleLogThread::BConsoleLogThread() { }

BConsoleLogThread::~BConsoleLogThread() { }

BOOL BConsoleLogThread::Init()
{
    // modified by lipengfei 13/05/03
    return TRUE;
}

UINT BConsoleLogThread::Run()
{
    BOOL bLoopFlag = TRUE;

    PLogRecordQueueManagement pQueueManager = BZ_SINGLETON_GET_PTR(BLogRecordQueueManagement);
    BZ_CHECK_RETURN_CODE(NULL != pQueueManager, 0);

    DWORD dwQueueID = BZ_HashString2ID(BZ_STRING_ID_OF_CONSOLE_LOG_HANDLER);
    BSPLogRecordQueue spLogRecordQueue;
    BOOL  bRetCode   = pQueueManager->GetUniversalQueue(dwQueueID, spLogRecordQueue);
    BZ_CHECK_RETURN_CODE(bRetCode, 0);

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
//  modified by lipengfei 13/05/02
    m_fileManager = BSPFileManager(
        BZ_SINGLETON_GET_PTR(BFileManager)
        ); // add by lipengfei 13/05/02
    BZ_CHECK_RETURN_BOOL(m_fileManager);

    return TRUE;
}

UINT BFileLogThread::Run()
{
    BOOL bLoopFlag = TRUE;
    BOOL bRetCode  = FALSE;

    PLogRecordQueueManagement pQueueManager = BZ_SINGLETON_GET_PTR(BLogRecordQueueManagement);
    BZ_CHECK_RETURN_BOOL(NULL != pQueueManager);

    DWORD dwQueueID = BZ_HashString2ID(BZ_STRING_ID_OF_FILE_LOG_HANDLER);
    BSPLogRecordQueue spLogRecordQueue;
    bRetCode = pQueueManager->GetUniversalQueue(dwQueueID, spLogRecordQueue);
    BZ_CHECK_RETURN_CODE(bRetCode, 0);

    BSPLogRecord  spLogRecord;
    BPackageHead  head;
    BSPFile       spFile;
    BOOL          bRet; 
    BSimpleString res;
    while(bLoopFlag)
    {
        // modified by lipengfei 13/05/03
        spLogRecord = spLogRecordQueue->PopNode();
        BPackageHandler::GetHead(spLogRecord->m_cpContent, head);
        bRet        = m_fileManager->Get(head.m_nFileID, spFile);
        res         = BPackageHandler::GetData(spLogRecord->m_cpContent);
        
        spFile->WriteTextLine(res.ToCstr(), res.GetLen() - 1);
        assert(bRet);
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

    PLogRecordQueueManagement pQueueManager = BZ_SINGLETON_GET_PTR(BLogRecordQueueManagement);
    BZ_CHECK_RETURN_BOOL(NULL != pQueueManager);

    DWORD dwQueueID = BZ_HashString2ID(BZ_STRING_ID_OF_NET_LOG_HANDLER);
    BSPLogRecordQueue spLogRecordQueue; 
    BOOL  bRetcode  = pQueueManager->GetUniversalQueue(dwQueueID, spLogRecordQueue);
    BZ_CHECK_RETURN_BOOL(bRetcode);

    while(bLoopFlag)
    {
         BSPLogRecord spLogRecord = spLogRecordQueue->PopNode();
// 
//         if (!spLogRecord && !(spLogRecord->GetLogRecordType() & K_LOG_RECORD_TYPE_NET))
//             continue;
    }

    return 0;
}


/************************************************************************/
/* Class BDbLogThread    lipengfei 13/05/06                             */
/************************************************************************/

BDbLogThread::BDbLogThread() { }

BDbLogThread::~BDbLogThread() { }

BOOL BDbLogThread::Init()
{
    m_mysqlManager = BSPMysqlTableManager(BZ_SINGLETON_GET_PTR(BMysqlTableManager));
    BZ_CHECK_RETURN_BOOL(m_mysqlManager);

    return TRUE;
}

UINT BDbLogThread::Run()
{
    BOOL bLoopFlag = TRUE;
    BOOL bRetCode  = FALSE;

    PLogRecordQueueManagement pQueueManager = BZ_SINGLETON_GET_PTR(BLogRecordQueueManagement);
    BZ_CHECK_RETURN_BOOL(NULL != pQueueManager);

    DWORD dwQueueID = BZ_HashString2ID(BZ_STRING_ID_OF_DB_LOG_HANDLER);
    BSPLogRecordQueue spLogRecordQueue;
    bRetCode = pQueueManager->GetUniversalQueue(dwQueueID, spLogRecordQueue);
    BZ_CHECK_RETURN_CODE(bRetCode, 0);
    
    BSPLogRecord  spRecord;
    BPackageHead  head;
    BSPMysqlTable spMysqlTable;
    BOOL          bRet; 
    BSimpleString res;
    int           nAddRet;
    char          cpFormatDbLog[1024];
    while (bLoopFlag)
    {
        spRecord = spLogRecordQueue->PopNode();
        BPackageHandler::GetHead(spRecord->m_cpContent, head);
        bRet     = m_mysqlManager->Get(head.m_nDbID, &spMysqlTable);
        res      = BPackageHandler::GetData(spRecord->m_cpContent);

        //TODO: format data in mysql data;
        sprintf(cpFormatDbLog, "'%s', NULL", res.ToCstr());
        nAddRet  = spMysqlTable->AddTuple(cpFormatDbLog);
        BZ_CHECK_RETURN_CODE(0 == nAddRet, -1);
    }

    return -1;
}

BZ_DECLARE_NAMESPACE_END