#ifndef _K_GAME_SERVER_LOG_SERVICE_H_
#define _K_GAME_SERVER_LOG_SERVICE_H_

#include "Tool/Application.h"

using namespace BZ(sabre);

namespace kserver_gs
{
    class KGsLogService : public BService
    {
    public:
        KGsLogService();
        virtual ~KGsLogService();

    public:
        virtual BOOL Init();
        virtual BOOL UnInit();
        virtual BOOL Run();

    private:
        BOOL LoadIniFiles();

        BOOL LoadNetLogIniFile();
        BOOL LoadFileLogIniFile();
        BOOL LoadConsoleLogIniFile();
    };
}

#endif
