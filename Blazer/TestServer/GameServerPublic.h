#ifndef _K_GAME_SERVER_PUBLIC_H_
#define _K_GAME_SERVER_PUBLIC_H_

#include "Public.h"
#include "Synchronize/ThreadMutex.h"
using namespace BZ(sabre);

namespace kserver_gs
{
    /*------------------------------------------------------------------------------*/
    /*                         extern global variables                              */
    /*------------------------------------------------------------------------------*/
    extern BThreadMutex g_ConsoleCS;


    /*------------------------------------------------------------------------------*/
    /*                                global variables                              */
    /*------------------------------------------------------------------------------*/
    const char* const DEFAULT_GAMESERVER_INIFILE = "./settings/GameServer.ini";
    const char* const DEFAULT_CONSOLELOG_INIFILE = "./settings/ConsoleLog.ini";
    const char* const DEFAULT_FILELOG_INIFILE    = "./settings/FileLog.ini";
}

#endif