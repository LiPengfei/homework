#include "Net/NetStruct.h"
#include "Net/SocketAgency.h"
#include "Net/SockIoHandleThread.h"
#include "Net/SocketStream.h"
#include "DesignPattern/Singleton.h"
#include "File/IniFile.h"
#include <iostream>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

/************************************************************************/
/* class BSocketAgency   add by lipengfei 13/05/07                      */
/************************************************************************/
BSocketAgency::BSocketAgency() { }
BSocketAgency::~BSocketAgency() { }

BOOL BSocketAgency::Init()
{
    m_hIocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    m_spSockAcceptor = BSPSocketAcceptor(new BSocketAcceptor);
    
    BIniFile iniFile;
    // start net server
    BOOL bRet = iniFile.LoadFile(BZ_SOCKET_AGENCY_CONFIG);
    BZ_CHECK_RETURN_BOOL(bRet);

    char cpIp[64] = {0};
    bRet = iniFile.GetValue("agency", "ip", cpIp);

    int  nPort = 0;
    bRet = iniFile.GetIntValue("agency", "port", nPort);
    BZ_CHECK_RETURN_BOOL(bRet);

    m_spSockAcceptor->Init(cpIp, (unsigned short)nPort);

    return TRUE;
}

BOOL BSocketAgency::Start()
{
    SYSTEM_INFO sysInfo;
    ::GetSystemInfo(&sysInfo);

    int nThreadNum = 2 * sysInfo.dwNumberOfProcessors;
    for (int i = 0; i != nThreadNum; ++i)
    {
        // NOTE: here will assign heap memory, and will not release them, 
        // but it has no problem with the program. just leave it like this;
        BSockIoHandleThread *pIoHandler(new BSockIoHandleThread(m_hIocp));
        pIoHandler->Init();
        pIoHandler->Start();
    }

    int bRet = 0;
    while (true)
    {
        BSocketStream *pSockStream = new BSocketStream;
        bRet = m_spSockAcceptor->Wait(m_hIocp, *pSockStream);
        BZ_CHECK_RETURN_BOOL(0 == bRet);

        BSockIoInfo *pskInfo = new BSockIoInfo;
        pskInfo->Reset();
        pSockStream->AsynRecv(pskInfo); // first recv
    }
    return TRUE;
}

BOOL BSocketAgency::UnInit()
{
    return TRUE;
}

BZ_DECLARE_NAMESPACE_END
