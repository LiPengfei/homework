#include "Log/LogService.h"
using namespace BZ(sabre);

int main(int argc, char *argv[])
{
    BLogger logger;
    logger.Init();
    logger.Run();

    char buf[512]        = "heshaorong";
    int nCurLen           = strlen(buf);
    BPackageHandler::InitialData(buf, nCurLen, 1024);
    BPackageHead head(0, 1, 1, 1, 
        BZ_HashString2ID(BZ_NET_LOG_KEY_ONE), 
        BZ_HashString2ID(BZ_FILE_LOG_KEY_WARN), 
        BZ_HashString2ID(BZ_DB_LOG_KEY_DEBUG));

    BPackageHandler::AddHead(buf, nCurLen, head, 1024);
    head.m_cNetFlag = 1;
    BPackageHandler::AddHead(buf, nCurLen, head, 1024);
    
    int i = 0;

    while(i != 10)
    {
        BLogRecord  *r  = new BLogRecord(buf, nCurLen);
        logger.WriteLog(r);
        Sleep(500);
        ++i;
    }

    logger.UnInit();
    return 0;
}

