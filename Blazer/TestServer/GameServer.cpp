#include "File/File.h"
#include "File/IniFile.h"
#include "GameServer.h"
#include "GameServerPublic.h"

#pragma warning(disable:4996)

namespace kserver_gs
{
    KGameServer::KGameServer() : m_nClientAgencyBindPort(0)
    {
        BZ_ZeroMemory(m_pClientAgencyBindIp, BZ_MAX_FILE_PATH_LEN);
    }

    KGameServer::~KGameServer()
    {
    }

    BOOL KGameServer::Init()
    {
        BOOL bRetCode = m_logServices.Init();
        BZ_CHECK_RETURN_BOOL(bRetCode);

        bRetCode = LoadIniFiles();
        BZ_CHECK_RETURN_BOOL(bRetCode);

        m_spClientAgency = KSPNonBlockSocketAgency(::new KClientAgency());
        BZ_CHECK_RETURN_BOOL(m_spClientAgency);

        bRetCode = m_spClientAgency->Init(m_pClientAgencyBindIp, static_cast<USHORT>(m_nClientAgencyBindPort));
        BZ_CHECK_RETURN_BOOL(bRetCode);

        m_logicFrameCtrl.Start();
        return TRUE;
    }

    BOOL KGameServer::Run()
    {
        BOOL bLoopFlag = TRUE;
        while(bLoopFlag)
        {
            bLoopFlag = GameLoop();
        }

        return TRUE;
    }

    BOOL KGameServer::UnInit()
    {
        return TRUE;
    }

    BOOL KGameServer::GameLoop()
    {
        BOOL bRetCode = FALSE;

        if (m_logicFrameCtrl.CanFrameCarryOn())
        {
            bRetCode = m_spClientAgency->Activate();
            assert(bRetCode);

            m_logicFrameCtrl.AddFrameCounter();
        }
        else
        {
            BZ_MilliSleep(1);
        }

        return TRUE;
    }

    BOOL KGameServer::LoadIniFiles()
    {
        BIniFile iniGameServer;
        BOOL bRetCode = FALSE;

        bRetCode = iniGameServer.LoadFile(DEFAULT_GAMESERVER_INIFILE);
        BZ_CHECK_RETURN_BOOL(bRetCode);

        bRetCode = iniGameServer.GetValue("ClientAgency", "AgencyIP", m_pClientAgencyBindIp);
        BZ_CHECK_RETURN_BOOL(bRetCode);
        bRetCode = iniGameServer.GetIntValue("ClientAgency", "AgencyPort", m_nClientAgencyBindPort);
        BZ_CHECK_RETURN_BOOL(bRetCode);

        return TRUE;
    }
}