#ifndef _K_GAME_SERVER_H_
#define _K_GAME_SERVER_H_

#include "Net/SocketAgency.h"
#include "Tool/Frame.h"
#include "Tool/Application.h"

#include "ClientAgency.h"
#include "GsLogService.h"

using namespace BZ(sabre);

CONST INT DEFAULT_IP_STRING_LEN              = 16;

namespace kserver_gs
{
    class KGameServer : public BApplication
    {
    private:
        BLogicFrameCtrl         m_logicFrameCtrl;
        KGsLogService           m_logServices;

        KSPNonBlockSocketAgency m_spClientAgency;
        CHAR                    m_pClientAgencyBindIp[DEFAULT_IP_STRING_LEN];
        INT                     m_nClientAgencyBindPort;

    public:
        KGameServer();
        virtual ~KGameServer();

    public:
        virtual BOOL Init();
        virtual BOOL Run();
        virtual BOOL UnInit();

    private:
        BOOL GameLoop();
        BOOL LoadIniFiles();
    };
}

#endif