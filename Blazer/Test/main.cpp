#include "GameServer.h"
#include "Log/LogHandler.h"
#include "Log/LogManager.h"
#include "Log/LogRecord.h"
#include "File/File.h"
#include "File/IniFile.h"
#include <iostream>
using namespace std;
using namespace BZ(sabre);

int main(int argc, char *argv[])
{
    //     kserver_gs::KGameServer game_server;
    //     if (game_server.Init())
    //     {
    //         game_server.Run();
    //         game_server.UnInit();
    //     }
    //     return EXIT_SUCCESS;

    BIniFile i;
    i.LoadFile("congf.ini");

    BLogManager *m            = new BLogManager;
    BLogHandler *n            = NULL;
    BLogHandler *f            = NULL;
    BLogHandler *c            = NULL;
    int          nNetFlag     = 0;
    BOOL         bNetRet      = FALSE;
    int          nFileFlag    = 0;
    BOOL         bFileRet     = FALSE;
    int          nConsoleFlag = 0;
    BOOL         bConsoleRet  = FALSE;
    m->Init();
    
    bNetRet  = i.GetIntValue("net", "enable", nNetFlag);
    BZ_PROCESS_ERROR(bNetRet);

    if (0 != nNetFlag)
    {   n = new BNetLogHandler;
        n->SetLogHandlerID(BZ_HashString2ID(K_STRING_ID_OF_NET_LOG_HANDLER));
        // TODO : add id;
        m->AttachHandler(n);
    }

    bFileRet  = i.GetIntValue("file", "enable", nFileFlag);
    BZ_PROCESS_ERROR(bFileRet);

    if (0 != nFileFlag)
    {
        f = new BFileLogHandler;
        f->SetLogHandlerID(BZ_HashString2ID(K_STRING_ID_OF_FILE_LOG_HANDLER));
        // TODO: add id;
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
            BSPFile spLogFile = BSPFile(new BFile);
            spLogFile->Open(segNode[i].GetValue().ToCstr(), "a+");
            pFileManager->Add(BZ_HashString2ID(segNode[i].GetKey().ToCstr()),
                spLogFile);
        }
        m->AttachHandler(f);
    }

    bConsoleRet  = i.GetIntValue("console", "enable", nConsoleFlag);
    BZ_PROCESS_ERROR(bConsoleRet);

    if (0 != nConsoleFlag)
    {
        c = new BConsoleLogHandler;
        c->SetLogHandlerID(BZ_HashString2ID(K_STRING_ID_OF_CONSOLE_LOG_HANDLER));
        m->AttachHandler(c);
    }

//    n->Init();
    f->Init();
    c->Init();

Exit0:

    char buf[512]        = "lipengfei";
    int nCurLen           = strlen(buf) + 1;
    BPackageHandler::InitialData(buf, nCurLen, 1024);
    BPackageHead head(0, 1, 1, 0, 0, BZ_HashString2ID(BZ_FILE_LOG_KEY_WARN), 0);

    BPackageHandler::AddHead(buf, nCurLen, head, 1024);
    BLogRecord  *r  = new BLogRecord(buf, nCurLen);
    BSPLogRecord spR(r);
    m->SetLogRecord(spR);
    m->NotifyHandler();
    Sleep(10000);

    return 0;
}

// #include "GameServer.h"

// int main(int argc, char *argv[])
// {
// 
//     kserver_gs::KGameServer game_server;
//     if (game_server.Init())
//     {
//         game_server.Run();
//         game_server.UnInit();
//     }
//     return EXIT_SUCCESS;
// }