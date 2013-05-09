#include "Log/LogHandler.h"
#include "Log/LogService.h"
#include "Net/SocketConnector.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

BLogService::BLogService() : m_bIsNetLogOpenedFlag(FALSE), 
    m_bIsFileLogOpenedFlag(FALSE),
    m_bIsConsoleLogOpenedFlag(FALSE)
{
}

BLogService::~BLogService()
{
}

BOOL BLogService::Init()
{
    m_bIsNetLogOpenedFlag     = FALSE;
    m_bIsFileLogOpenedFlag    = FALSE;
    m_bIsConsoleLogOpenedFlag = FALSE;
    return TRUE;
}

BOOL BLogService::UnInit()
{
    return TRUE;
}

BOOL BLogService::Run()
{
    return TRUE;
}

BOOL BLogService::GetNetLogOpenedFlag() CONST
{
    return m_bIsNetLogOpenedFlag;
}

BOOL BLogService::GetFileLogOpenedFlag() CONST
{
    return m_bIsFileLogOpenedFlag;
}

BOOL BLogService::GetConsoleLogOpenedFlag() CONST
{
    return m_bIsConsoleLogOpenedFlag;
}

void BLogService::SetNetLogOpenedFlag(IN CONST BOOL bIsNetLogOpenedFlag)
{
    m_bIsNetLogOpenedFlag = bIsNetLogOpenedFlag;
}

void BLogService::SetFileLogOpenedFlag(IN CONST BOOL bIsFileLogOpenedFlag)
{
    m_bIsFileLogOpenedFlag = bIsFileLogOpenedFlag;
}

void BLogService::SetConsoleLogOpenedFlag(IN CONST BOOL bIsConsoleLogOpenedFlag)
{
    m_bIsConsoleLogOpenedFlag = bIsConsoleLogOpenedFlag;
}

BOOL BLogService::OpenLog (IN CONST BYTE byLogType)
{
    BOOL bRetCode = FALSE;
    switch(byLogType)
    {
    case K_LOG_IF_OPEN_TYPE_NET:
        {
            bRetCode = OpenNetLog();
        }
        break;
    case K_LOG_IF_OPEN_TYPE_FILE:
        {
            bRetCode = OpenFileLog();
        }
        break;
    case K_LOG_IF_OPEN_TYPE_CONSOLE:
        {
            bRetCode = OpenConsoleLog();
        }
        break;
    default:
        {
        }
        break;
    }

    return bRetCode;
}

BOOL BLogService::OpenNetLog()
{
    BOOL bRetCode = FALSE;

    if (GetNetLogOpenedFlag())
        return TRUE;

    PLogHandler pLogHandler = ::new BNetLogHandler();
    BZ_CHECK_RETURN_BOOL(NULL != pLogHandler);

    BSPLogHandler spLogHandler(pLogHandler);
    DWORD dwLogHandlerID = BZ_HashString2ID(BZ_STRING_ID_OF_NET_LOG_HANDLER);

    BZ_CHECK_RETURN_BOOL(0 != dwLogHandlerID);
    spLogHandler->SetLogHandlerID(dwLogHandlerID);

    bRetCode = spLogHandler->Init();
    BZ_CHECK_RETURN_BOOL(bRetCode);

    bRetCode = g_AttachLogHandler(m_logManager, spLogHandler);
    BZ_CHECK_RETURN_BOOL(bRetCode);

    SetNetLogOpenedFlag(TRUE);
    return TRUE;
}

BOOL BLogService::OpenFileLog()
{
    BOOL bRetCode = FALSE;

    if (GetFileLogOpenedFlag())
        return TRUE;

    PLogHandler pLogHandler = ::new BFileLogHandler();
    BZ_CHECK_RETURN_BOOL(NULL != pLogHandler);

    BSPLogHandler spLogHandler(pLogHandler);
    DWORD dwLogHandlerID = BZ_HashString2ID(BZ_STRING_ID_OF_FILE_LOG_HANDLER);

    BZ_CHECK_RETURN_BOOL(0 != dwLogHandlerID);
    spLogHandler->SetLogHandlerID(dwLogHandlerID);

    bRetCode = spLogHandler->Init();
    BZ_CHECK_RETURN_BOOL(bRetCode);

    bRetCode = g_AttachLogHandler(m_logManager, spLogHandler);
    BZ_CHECK_RETURN_BOOL(bRetCode);

    SetFileLogOpenedFlag(TRUE);
    return TRUE;
}

BOOL BLogService::OpenConsoleLog()
{
    BOOL bRetCode = FALSE;

    if (GetConsoleLogOpenedFlag())
        return TRUE;

    PLogHandler pLogHandler = ::new BConsoleLogHandler();
    BZ_CHECK_RETURN_BOOL(NULL != pLogHandler);

    BSPLogHandler spLogHandler(pLogHandler);
    DWORD dwLogHandlerID = BZ_HashString2ID(BZ_STRING_ID_OF_CONSOLE_LOG_HANDLER);

    BZ_CHECK_RETURN_BOOL(0 != dwLogHandlerID);
    spLogHandler->SetLogHandlerID(dwLogHandlerID);

    bRetCode = spLogHandler->Init();
    BZ_CHECK_RETURN_BOOL(bRetCode);

    bRetCode = g_AttachLogHandler(m_logManager, spLogHandler);
    BZ_CHECK_RETURN_BOOL(bRetCode);

    SetConsoleLogOpenedFlag(TRUE);
    return TRUE;
}

BOOL BLogService::CloseLog(IN CONST BYTE byLogType)
{
    BOOL bRetCode = FALSE;
    switch(byLogType)
    {
    case K_LOG_IF_OPEN_TYPE_NET:
        {
            bRetCode = CloseNetLog();
        }
        break;
    case K_LOG_IF_OPEN_TYPE_FILE:
        {
            bRetCode = CloseFileLog();
        }
        break;
    case K_LOG_IF_OPEN_TYPE_CONSOLE:
        {
            bRetCode = CloseConsoleLog();
        }
        break;
    default:
        {
        }
        break;
    }

    return bRetCode;
}

BOOL BLogService::CloseNetLog()
{
    BOOL bRetCode = FALSE;

    if (!GetNetLogOpenedFlag())
        return TRUE;

    DWORD dwLogHandlerID = BZ_HashString2ID(BZ_STRING_ID_OF_NET_LOG_HANDLER);
    bRetCode = m_logManager.DetachHandler(dwLogHandlerID);
    BZ_CHECK_RETURN_BOOL(bRetCode);
    SetNetLogOpenedFlag(FALSE);

    return TRUE;
}

BOOL BLogService::CloseFileLog()
{
    BOOL bRetCode = FALSE;

    if (!GetFileLogOpenedFlag())
        return TRUE;

    DWORD dwLogHandlerID = BZ_HashString2ID(BZ_STRING_ID_OF_FILE_LOG_HANDLER);
    bRetCode = m_logManager.DetachHandler(dwLogHandlerID);
    BZ_CHECK_RETURN_BOOL(bRetCode);
    SetFileLogOpenedFlag(FALSE);

    return TRUE;
}

BOOL BLogService::CloseConsoleLog()
{
    BOOL bRetCode = FALSE;

    if (!GetConsoleLogOpenedFlag())
        return TRUE;

    DWORD dwLogHandlerID = BZ_HashString2ID(BZ_STRING_ID_OF_CONSOLE_LOG_HANDLER);
    bRetCode = m_logManager.DetachHandler(dwLogHandlerID);
    BZ_CHECK_RETURN_BOOL(bRetCode);
    SetConsoleLogOpenedFlag(FALSE);

    return TRUE;
}

void BLogService::WriteLog(IN CONST BSPLogRecord &spLogRecord)
{
    BZ_WriteLog(m_logManager, spLogRecord);
}

/************************************************************************/
/* class BLogger                                                        */
/************************************************************************/

BLogger::BLogger() { }
BLogger::~BLogger() { }

BOOL BLogger::Init() 
{
    m_pLogManager = BZ_SINGLETON_GET_PTR(BLogManager);
    BZ_CHECK_RETURN_BOOL(m_pLogManager);

    m_spIniFile    = BSPIniFile(new BIniFile);
    BZ_CHECK_RETURN_BOOL(m_spIniFile);

    BOOL bRet = m_spIniFile->LoadFile(BZ_LOG_SERVICE_SERVER_CONFIG);
    BZ_CHECK_RETURN_BOOL(bRet);

    bRet = m_pLogManager->Init();
    BZ_CHECK_RETURN_BOOL(bRet);
    
    return TRUE;
}

BOOL BLogger::UnInit() 
{
    BZ_CHECK_RETURN_BOOL(m_pLogManager);
    BZ_CHECK_RETURN_BOOL(m_spIniFile);

    return TRUE;
}

BOOL BLogger::Run() 
{
    int nRet = -1;

    nRet     = PrepareFileLogger();
    BZ_CHECK_RETURN_BOOL(-1 != nRet);

    nRet     = PrepareDbLogger();
    BZ_CHECK_RETURN_BOOL(-1 != nRet);

    nRet     = PrepareNetLogger();
    BZ_CHECK_RETURN_BOOL(-1 != nRet);

    nRet     = PrepareConsoleLogger();
    BZ_CHECK_RETURN_BOOL(-1 != nRet);

    int  nHandlerNum = m_pLogManager->m_logHandlerVector.size();
    for (int i = 0; i != nHandlerNum; ++i)
    {
        m_pLogManager->m_logHandlerVector[i]->Init();
    }

    return TRUE;
}

void BLogger::WriteLog(BLogRecord *pRecord)
{
    m_pLogManager->SetLogRecord(BSPLogRecord(pRecord));
    m_pLogManager->NotifyHandler();
}

INT BLogger::PrepareFileLogger()
{
    int nRet = GetFlag("file");
    BZ_CHECK_RETURN_CODE(-1 != nRet, -1);

    if (0 == nRet)
    {
        return 0;
    }

    BFileLogHandler *pFileHandler(new BFileLogHandler);

    pFileHandler->SetLogHandlerID(BZ_HashString2ID(BZ_STRING_ID_OF_FILE_LOG_HANDLER));

    // add file id;
    int           nRegTotal    = 0;
    int           nRealTotal   = 0;
    BSegmentNode  segNode;
    BFileManager *pFileManager = BZ_SINGLETON_GET_PTR(BFileManager);

    m_spIniFile->GetIntValue("filelist", "total", nRegTotal);
    m_spIniFile->GetSegment("filelist", segNode);
    nRealTotal = segNode.GetSize();
    BZ_CHECK_RETURN_CODE(nRealTotal == (nRegTotal + 1), -1);

    for (int i = 1; i != nRealTotal; ++i)
    {
        BSPFile spLogFile(new BFile);
        spLogFile->Open(segNode[i].GetValue().ToCstr(), "a+");
        pFileManager->Add(BZ_HashString2ID(segNode[i].GetKey().ToCstr()),
            spLogFile);
    }

    m_pLogManager->AttachHandler(pFileHandler);
    return 1;
}

INT BLogger::PrepareDbLogger()
{
    int nRet = GetFlag("db");
    BZ_CHECK_RETURN_CODE(-1 != nRet, -1);

    if (0 == nRet)
    {
        return 0;
    }

    BDbLogHandler *pDbHandler(new BDbLogHandler);
    pDbHandler->SetLogHandlerID(BZ_HashString2ID(BZ_STRING_ID_OF_DB_LOG_HANDLER));

    int                 nRegTotal    = 0;
    int                 nRealTotal   = 0;
    BSegmentNode        segNode;
    BMysqlTableManager *pDbManager = BZ_SINGLETON_GET_PTR(BMysqlTableManager);

    m_spIniFile->GetIntValue("dblist", "total", nRegTotal);
    m_spIniFile->GetSegment("dblist", segNode);
    nRealTotal = segNode.GetSize();
    BZ_CHECK_RETURN_CODE(nRealTotal == (nRegTotal + 1), -1);

    BSimpleString ssDbInfo;
    std::vector<std::string> vecString;
    for (int i = 1; i != nRealTotal; ++i)
    {
        ssDbInfo  = segNode[i].GetValue();
        vecString = BZ_StringSplit(ssDbInfo.ToCstr(), ':');
        BZ_CHECK_RETURN_CODE(6 == vecString.size(), -1);

        BSPMysqlTable spMysqlTable(new BMysqlTable);
        spMysqlTable->Init();
        int nConRet = spMysqlTable->Connect(
            vecString[0].c_str(), 
            vecString[1].c_str(),
            vecString[2].c_str(),
            vecString[3].c_str(),
            vecString[4].c_str()
            );
        BZ_CHECK_RETURN_CODE(-1 != nConRet, -1);
        spMysqlTable->SetName(vecString[5].c_str());

        pDbManager->Add(BZ_HashString2ID(segNode[i].GetKey().ToCstr()),
            spMysqlTable);
    }
    m_pLogManager->AttachHandler(pDbHandler);
    return 1;
}

INT BLogger::PrepareNetLogger()
{
    int nRet = GetFlag("net");
    BZ_CHECK_RETURN_CODE(-1 != nRet, -1);

    if (0 == nRet)
    {
        return 0;
    }

    BNetLogHandler *pNetHandler(new BNetLogHandler); 
    pNetHandler->SetLogHandlerID(BZ_HashString2ID(BZ_STRING_ID_OF_NET_LOG_HANDLER));

    int                   nRegTotal    = 0;
    int                   nRealTotal   = 0;
    BSegmentNode          segNode;
    BSocketStreamManager *pSockManager = BZ_SINGLETON_GET_PTR(BSocketStreamManager);

    m_spIniFile->GetIntValue("netlist", "total", nRegTotal);
    m_spIniFile->GetSegment("netlist", segNode);
    nRealTotal = segNode.GetSize();
    BZ_CHECK_RETURN_CODE(nRealTotal == (nRegTotal + 1), -1);

    BSimpleString            ssNetInfo;
    std::vector<std::string> vecString;
    int                      nSizeOfPara;
    unsigned short           usPort;

    for (int i = 1; i != nRealTotal; ++i)
    {
        ssNetInfo   = segNode[i].GetValue();
        vecString   = BZ_StringSplit(ssNetInfo.ToCstr(), ':');
        nSizeOfPara = vecString.size();
        BZ_CHECK_RETURN_CODE(2 == nSizeOfPara, -1);

        usPort      = (unsigned short)atoi(vecString[1].c_str());

        BSocketStream *pSockStream(new BSocketStream);
        BSocketConnector connctor;
        nRet        = connctor.Connect(vecString[0].c_str(), usPort, pSockStream);
        BZ_CHECK_RETURN_CODE(0 == nRet, -1);

        BSPSocketStream spSockStream(pSockStream);
        ssNetInfo   = segNode[i].GetKey();
        pSockManager->Add(
            BZ_HashString2ID(ssNetInfo.ToCstr()), 
            spSockStream);
    }
    m_pLogManager->AttachHandler(pNetHandler);

    return 1;
}

INT BLogger::PrepareConsoleLogger()
{
    int nRet = GetFlag("console");
    BZ_CHECK_RETURN_CODE(-1 != nRet, -1);

    if (0 == nRet)
    {
        return 0;
    }

    BConsoleLogHandler *pConsoleHandler(new BConsoleLogHandler);
    pConsoleHandler->SetLogHandlerID(BZ_HashString2ID(BZ_STRING_ID_OF_CONSOLE_LOG_HANDLER));
    m_pLogManager->AttachHandler(pConsoleHandler);

    return 1;
}

INT BLogger::GetFlag(const char *cpName)
{
    int  nFlag = 0;
    BOOL bRet = m_spIniFile->GetIntValue(cpName, "enable", nFlag);

    BZ_CHECK_RETURN_CODE(bRet, -1);
    return nFlag;
}

BZ_DECLARE_NAMESPACE_END
