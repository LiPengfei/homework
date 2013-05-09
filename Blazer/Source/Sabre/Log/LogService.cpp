#include "Log/LogHandler.h"
#include "Log/LogService.h"
#include "Net/SocketConnector.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

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
