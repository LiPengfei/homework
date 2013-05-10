#include "Log/LogThread.h"
#include "Log/LogRecord.h"
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
//  modified by lipengfei 13/05/09 sigleton can't be wrapped by sharedptr
    m_pFileManager = BZ_SINGLETON_GET_PTR(BFileManager); 
    
// add by lipengfei 13/05/02
    BZ_CHECK_RETURN_BOOL(m_pFileManager);

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
    int           nWriteLen;
    while(bLoopFlag)
    {
        // modified by lipengfei 13/05/03
        spLogRecord = spLogRecordQueue->PopNode();
        BPackageHandler::GetHead(spLogRecord->m_cpContent, head);
        bRet        = m_pFileManager->Get(head.m_nFileID, spFile);
        res         = BPackageHandler::GetData(spLogRecord->m_cpContent);
        nWriteLen   = res.GetLen() - 1;

        spFile->WriteTextLine(res.ToCstr(), nWriteLen);
        spFile->Flush();
        assert(bRet);
    }
    return 0;
}

/************************************************************/
/* class BNetLogThread add by lipengfei 13/05/08            */
/************************************************************/

BNetLogThread::BNetLogThread()
{

}

BOOL BNetLogThread::Init()
{
    // modified bug by lipengfei 13/05/09, singleton can't be wrapped by sharedptr;
    m_pSockstrManager =
        BZ_SINGLETON_GET_PTR(BSocketStreamManager);

    // modified end
    BZ_CHECK_RETURN_BOOL(m_pSockstrManager);

    return TRUE;
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

    BSPLogRecord    spLogRecord;
    BPackageHead    head;
    BSPSocketStream spStream;
    int             nRet;
    int             nTotalLen;
    char            cpData[BZ_MAX_PACKAGE_DATA];

    while(bLoopFlag)
    {
          spLogRecord = spLogRecordQueue->PopNode();

          // modified by lipengfei 13/05/08 logThread need to remove head to send
          memcpy(cpData, spLogRecord->m_cpContent, BZ_MAX_PACKAGE_DATA);
          nRet = BPackageHandler::MoveHead(cpData, BZ_MAX_PACKAGE_DATA, head);
          BZ_CHECK_RETURN_CODE(0 != nRet, 1);

          nRet = m_pSockstrManager->Get(head.m_nNetID, spStream);
          BZ_CHECK_RETURN_CODE(nRet, 1);
          
          nTotalLen = BPackageHandler::GetTotalLen(cpData);
          spStream->Send(cpData, nTotalLen);
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
    m_pMysqlManager = BZ_SINGLETON_GET_PTR(BMysqlTableManager);
    BZ_CHECK_RETURN_BOOL(m_pMysqlManager);

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
        bRet     = m_pMysqlManager->Get(head.m_nDbID, &spMysqlTable);
        res      = BPackageHandler::GetData(spRecord->m_cpContent);

        //TODO: format data in mysql data;
        sprintf(cpFormatDbLog, "'%s', NULL", res.ToCstr());
        nAddRet  = spMysqlTable->AddTuple(cpFormatDbLog);
        BZ_CHECK_RETURN_CODE(0 == nAddRet, 1);
    }

    return 1;
}

BZ_DECLARE_NAMESPACE_END
