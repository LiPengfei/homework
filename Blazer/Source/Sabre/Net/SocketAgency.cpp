#include "Net/NetStruct.h"
#include "Net/SocketAgency.h"
#include "Console/ConsoleSimulationWindow.h"
#include "DesignPattern/Singleton.h"
#include <iostream>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

BNonBlockSocketAgency::BNonBlockSocketAgency()
{
}

BNonBlockSocketAgency::~BNonBlockSocketAgency()
{
}

BOOL BNonBlockSocketAgency::Init(CONST std::string &strIp, CONST USHORT &usPort)
{
    BOOL bRetCode = FALSE;

    bRetCode = SINGLETON_GET_REF(BNetService).Start();
    BZ_CHECK_RETURN_BOOL(bRetCode);

    PNonBlockSocketAcceptor pAccepor = ::new BNonBlockSocketAcceptor();
    PAsyncSocketStreamQueue pQueue   = ::new BAsyncSocketStreamQueue();
    PAsyncSocketEvent pEventArray    = ::new BAsyncSocketEvent[MAX_SOCKET_EVENT];
    BZ_PROCESS_ERROR(NULL != pAccepor && NULL != pQueue && NULL != pEventArray);

    m_spAcceptor    = BSPNonBlockSocketAcceptor(pAccepor);
    m_spEventArray  = BSPAsyncSocketEventArray(pEventArray);
    m_spStreamQueue = BSPAsyncSocketStreamQueue(pQueue);

    bRetCode = pAccepor->Init(strIp, usPort);
    BZ_PROCESS_ERROR(bRetCode);

    bRetCode = pAccepor->AttachSocketStreamQueue(m_spStreamQueue);
    BZ_PROCESS_ERROR(bRetCode);

    return TRUE;
Exit0:
    BZ_SafelyDeletePtr(pAccepor);
    BZ_SafelyDeletePtr(pQueue);
    BZ_SafelyDeleteArrayPtr(pEventArray);
    return FALSE;
}

BOOL BNonBlockSocketAgency::UnInit()
{
    m_spAcceptor->UnInit();
    m_spAcceptor.Reset();
    m_spEventArray.Reset();
    return TRUE;
}

BOOL BNonBlockSocketAgency::Activate()
{
    BOOL bRetCode = FALSE;
    bRetCode = ProcessNetEvent();
    BZ_CHECK_RETURN_BOOL(bRetCode);
    return TRUE;
}

BOOL BNonBlockSocketAgency::ProcessNetEvent()
{
    INT nErrorCode = 0;
    BOOL bLoopFlag = TRUE;
    BOOL bRetCode  = FALSE;

    while(bLoopFlag)
    {
        INT nEventCount = 0;
        m_spAcceptor->Wait(MAX_SOCKET_EVENT, nEventCount, m_spEventArray);
        m_spStreamQueue->Wait(MAX_SOCKET_EVENT, nEventCount, m_spEventArray);

        if (0 == nEventCount)   // no event
            break;

        for (INT i = 0; i < nEventCount; i++)
        {
            BSPAsyncSocketStream &spAsyncSocketStream = m_spEventArray[i].m_spAsyncSocketStream;
            switch (m_spEventArray[i].m_nEventType)
            {
            case ASYNC_SOCKET_EVENT_ACCEPT:
                {
                    OnClientConnected(spAsyncSocketStream);
                    bRetCode = spAsyncSocketStream->OnStreamEstablish(&nErrorCode);
                    if (!bRetCode && nErrorCode == -1)
                    {
                        OnCloseConnection(spAsyncSocketStream);
                        break;
                    }
                }
                break;
            case ASYNC_SOCKET_EVENT_IN:
                {
                    ProcessAllCompletePackage(spAsyncSocketStream);
                }
                break;
            default:
                break;
            }

            m_spEventArray[i].Reset();
        }
    }
    return TRUE;
}

BOOL BNonBlockSocketAgency::ProcessAllCompletePackage(BSPAsyncSocketStream &spAsyncSocketStream)
{
    INT nErrorCode = 0;
    BSPDynamicBuffer spBuffer;

    BOOL bLoopFlag = TRUE;
    BOOL bRetCode  = FALSE;

    while(bLoopFlag)
    {
        bRetCode = spAsyncSocketStream->Recv(spBuffer, &nErrorCode);
        if (!bRetCode && nErrorCode == -1)
        { // a serious error was occured
            OnCloseConnection(spAsyncSocketStream);
            return FALSE;
        }

        if (!bRetCode && nErrorCode == 1)
        { // there is no more complete package
            return FALSE;
        }

        OnRecvdClientData(spAsyncSocketStream, spBuffer);
    }

    return TRUE;
}

BOOL BNonBlockSocketAgency::OnRecvdClientData(BSPAsyncSocketStream &spAsyncSocketStream, BSPDynamicBuffer &spBuffer)
{
    std::cout << "[INFO] Receive a client data from: ip - " 
        << spAsyncSocketStream->GetRemoteIp()
        << ", port - "
        << spAsyncSocketStream->GetRemotePort()
        << "\ndata - ";
    BZ_PrintNumArrayInHex((PCHAR)spBuffer->GetDataPtr(), spBuffer->GetUsedSize());

    std::cout << std::endl;

    spAsyncSocketStream->Send(spBuffer);
    return TRUE;
}

BOOL BNonBlockSocketAgency::OnClientConnected(BSPAsyncSocketStream &spAsyncSocketStream)
{
    std::cout << "[INFO] Accept a new connection from: ip - " 
        << spAsyncSocketStream->GetRemoteIp()
        << ", port - "
        << spAsyncSocketStream->GetRemotePort()
        << std::endl;
    return TRUE;
}

BOOL BNonBlockSocketAgency::OnCloseConnection(BSPAsyncSocketStream &spAsyncSocketStream)
{
    std::cout << "[INFO] Close a new connection from: ip - " 
        << spAsyncSocketStream->GetRemoteIp()
        << ", port - "
        << spAsyncSocketStream->GetRemotePort()
        << std::endl;
    spAsyncSocketStream->Close();
    return TRUE;
}


/************************************************************************/
/* class BSocketAgency   add by lipengfei 13/05/07                      */
/************************************************************************/
BSocketAgency::BSocketAgency() { }
BSocketAgency::~BSocketAgency() { }

BOOL BSocketAgency::Init()
{
    return TRUE;
}

BOOL BSocketAgency::Start()
{
    return TRUE;
}

BOOL BSocketAgency::Unit()
{
    return TRUE;
}

BZ_DECLARE_NAMESPACE_END
