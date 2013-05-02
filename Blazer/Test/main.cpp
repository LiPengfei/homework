#include "GameServer.h"
#include "Log/LogHandler.h"
#include "Log/LogManager.h"
#include "Log/LogRecord.h"
#include <iostream>
using namespace std;
using namespace BZ(sabre);

int main(int argc, char *argv[])
{
    // 
    //     kserver_gs::KGameServer game_server;
    //     if (game_server.Init())
    //     {
    //         game_server.Run();
    //         game_server.UnInit();
    //     }
    //     return EXIT_SUCCESS;
    BConsoleLogHandler *c = new BConsoleLogHandler;
    c->Init();

    BLogManager        *m = new BLogManager;
    m->Init();
    m->AttachHandler(c);

    char buf[512]        = "lipengfei";
    int nCurLen           = strlen(buf) + 1;
    BPackageHandler::InitialData(buf, nCurLen, 1024);
    BPackageHead head(0, 1, 0, 0, 0, 0, 0);

    BPackageHandler::AddHead(buf, nCurLen, head, 1024);
    BLogRecord  *r  = new BLogRecord(buf, nCurLen);
    BSPLogRecord spR(r);
    m->SetLogRecord(spR);
    m->NotifyHandler();
    Sleep(5000);
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