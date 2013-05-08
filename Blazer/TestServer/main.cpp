#include "GameServer.h"
#include "Log/LogHandler.h"
#include "Log/LogManager.h"
#include "Log/LogRecord.h"
#include "File/File.h"
#include "File/IniFile.h"
#include "Net/SocketConnector.h"
#include <iostream>
using namespace std;
using namespace BZ(sabre);

// int main(int argc, char *argv[])
// {
//     kserver_gs::KGameServer game_server;
//     if (game_server.Init())
//     {
//         game_server.Run();
//         game_server.UnInit();
//     }
//     return EXIT_SUCCESS;
// }

int main(int argc, char *argv[])
{
   BIniFile i;
   i.LoadFile("config.ini");

    BLogManager *m            = new BLogManager;
    BLogHandler *n            = NULL;
    BLogHandler *f            = NULL;
    BLogHandler *c            = NULL;
    BLogHandler *d            = NULL;

    int          nNetFlag     = 0;
    BOOL         bNetRet      = FALSE;
    int          nFileFlag    = 0;
    BOOL         bFileRet     = FALSE;
    int          nConsoleFlag = 0;
    BOOL         bConsoleRet  = FALSE;
    int          nDbFlag      = 0;
    BOOL         bDbRet       = FALSE;
    m->Init();
    
    // Net
    bNetRet  = i.GetIntValue("net", "enable", nNetFlag);
    BZ_PROCESS_ERROR(bNetRet);

    if (0 != nNetFlag)
    {   n = new BNetLogHandler;
        n->SetLogHandlerID(BZ_HashString2ID(BZ_STRING_ID_OF_NET_LOG_HANDLER));
        // TODO : add id;
        int                   nRegTotal    = 0;
        int                   nRealTotal   = 0;
        BSegmentNode          segNode;
        BSocketStreamManager *pSockManager = BZ_SINGLETON_GET_PTR(BSocketStreamManager);
        
        i.GetIntValue("netlist", "total", nRegTotal);
        i.GetSegment("netlist", segNode);
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
            connctor.Connect(vecString[0].c_str(), usPort, pSockStream);
            BSPSocketStream spSockStream(pSockStream);
            ssNetInfo   = segNode[i].GetKey();
            pSockManager->Add(
                BZ_HashString2ID(ssNetInfo.ToCstr()), 
                spSockStream);
        }
        m->AttachHandler(n);
    }

    // File 
    bFileRet  = i.GetIntValue("file", "enable", nFileFlag);
    BZ_PROCESS_ERROR(bFileRet);

    if (0 != nFileFlag)
    {
        f = new BFileLogHandler;
        f->SetLogHandlerID(BZ_HashString2ID(BZ_STRING_ID_OF_FILE_LOG_HANDLER));
        // TODO: add file id;
        int           nRegTotal    = 0;
        int           nRealTotal   = 0;
        BSegmentNode  segNode;
        BFileManager *pFileManager = BZ_SINGLETON_GET_PTR(BFileManager);

        i.GetIntValue("filelist", "total", nRegTotal);
        i.GetSegment("filelist", segNode);
        nRealTotal = segNode.GetSize();
        BZ_PROCESS_ERROR(nRealTotal == (nRegTotal + 1));

        for (int i = 1; i != nRealTotal; ++i)
        {
            BSPFile spLogFile(new BFile);
            spLogFile->Open(segNode[i].GetValue().ToCstr(), "a+");
            pFileManager->Add(BZ_HashString2ID(segNode[i].GetKey().ToCstr()),
                spLogFile);
        }
        m->AttachHandler(f);
    }

    // Console
    bConsoleRet  = i.GetIntValue("console", "enable", nConsoleFlag);
    BZ_PROCESS_ERROR(bConsoleRet);

    if (0 != nConsoleFlag)
    {
        c = new BConsoleLogHandler;
        c->SetLogHandlerID(BZ_HashString2ID(BZ_STRING_ID_OF_CONSOLE_LOG_HANDLER));
        m->AttachHandler(c);
    }

    // Db
    bDbRet  = i.GetIntValue("db", "enable", nDbFlag);
    BZ_PROCESS_ERROR(bDbRet);

    if (0 != nDbFlag)
    {
        d = new BDbLogHandler;
        d->SetLogHandlerID(BZ_HashString2ID(BZ_STRING_ID_OF_DB_LOG_HANDLER));

        int                 nRegTotal    = 0;
        int                 nRealTotal   = 0;
        BSegmentNode        segNode;
        BMysqlTableManager *pDbManager = BZ_SINGLETON_GET_PTR(BMysqlTableManager);

        i.GetIntValue("dblist", "total", nRegTotal);
        i.GetSegment("dblist", segNode);
        nRealTotal = segNode.GetSize();
        BZ_PROCESS_ERROR(nRealTotal == (nRegTotal + 1));
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
        m->AttachHandler(d);
    }

    // Init
    if (n)
    {
        n->Init();
    }

    if (f)
    {
        f->Init();
    }

    if (c)
    {
        c->Init();
    }

    if (d)
    {
        d->Init();
    }

Exit0:

    char buf[512]        = "heshaorong";
    int nCurLen           = strlen(buf);
    BPackageHandler::InitialData(buf, nCurLen, 1024);
    BPackageHead head(1, 1, 1, 1, 
        BZ_HashString2ID(BZ_NET_LOG_KEY_ONE), 
        BZ_HashString2ID(BZ_FILE_LOG_KEY_WARN), 
        BZ_HashString2ID(BZ_DB_LOG_KEY_DEBUG));

    BPackageHandler::AddHead(buf, nCurLen, head, 1024);
    BLogRecord  *r  = new BLogRecord(buf, nCurLen);
    BSPLogRecord spR(r);
    m->SetLogRecord(spR);
    m->NotifyHandler();
    Sleep(30000);

    return 0;
}

// #include "GameServer.h"
// #include "Database/MysqlManager.h"
// #include "Net/NetStruct.h"

// using namespace BZ(sabre);
// int main(int argc, char *argv[])
// {
//     BNetService *pNet =  BZ_SINGLETON_GET_PTR(BNetService);
//     pNet->Start();
// 
//     BSocketAgency a;
//     a.Init();
//     a.Start();
//     while(true);
// }

