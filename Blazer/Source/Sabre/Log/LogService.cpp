#include "Log/LogHandler.h"
#include "Log/LogService.h"

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

BLogger::BLogger() : m_spIniFile(new BIniFile) { }
BLogger::~BLogger() { }

BOOL BLogger::Init() 
{
    BZ_CHECK_RETURN_BOOL(m_spLogManager);
    BZ_CHECK_RETURN_BOOL(m_spIniFile);
    
    BOOL bRet = m_spIniFile->LoadFile(BZ_LOG_SERVICE_SERVER_CONFIG);
    BZ_CHECK_RETURN_BOOL(bRet);
    return TRUE;
}

BOOL BLogger::UnInit() 
{
    BZ_CHECK_RETURN_BOOL(m_spLogManager);
    BZ_CHECK_RETURN_BOOL(m_spIniFile);

    return TRUE;
}

BOOL BLogger::Run() 
{
    BOOL bFileLog    = FALSE;
    BOOL bDbLog      = FALSE;
    BOOL bNetLog     = FALSE;
    BOOL bConsoleLog = FALSE;
    return TRUE;
}

BOOL BLogger::SetLogManager(const BSPLogManager &spLogManager)
{
    BZ_CHECK_RETURN_BOOL(spLogManager);

    m_spLogManager = spLogManager;
    return TRUE;
}

BOOL BLogger::SetLogManager(BLogManager *pLogManager)
{
    BZ_CHECK_RETURN_BOOL(pLogManager);

    m_spLogManager = BSPLogManager(pLogManager);
    return TRUE;
}

INT BLogger::StartFileLogger()
{
    return 0;
}
INT BLogger::StartDbLogger()
{
    return 0;
}

INT BLogger::StartNetLogger()
{
    int nRet = GetFlag("net");
    BZ_CHECK_RETURN_CODE(-1 != nRet, -1);

    if (0 == nRet)
    {
        return 0;
    }
}

INT BLogger::StartConsoleLogger()
{
    return 0;
}

INT BLogger::GetFlag(const char *cpName)
{
    int  nFlag = 0;
    BOOL bRet = m_spIniFile->GetIntValue(cpName, "enable", nFlag);

    BZ_CHECK_RETURN_CODE(bRet, -1);
    return nFlag;
}

BZ_DECLARE_NAMESPACE_END
