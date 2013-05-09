#include "Net/SocketAcceptor.h"
#include "Net/SocketStream.h"
#include "Net/NetStruct.h"
#include <errno.h>
#include <string>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

// add by lipengfei 2013/04/26
/************************************************************************/
/* Class BSocketAcceptor                                                */
/************************************************************************/
BSocketAcceptor::BSocketAcceptor() 
    : m_hListenSocket(INVALID_SOCKET), 
      m_usPort(0)
{ }

BSocketAcceptor::~BSocketAcceptor()
{
    UnInit();
}

INT BSocketAcceptor::Init(CONST std::string &strIpAddress, CONST USHORT &usPort)
{
    m_strIpAddress  = strIpAddress;
    m_usPort        = usPort;
    m_hListenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    BZ_PROCESS_ERROR(INVALID_SOCKET != m_hListenSocket);

    INT         retCode;
    sockaddr_in saLocalAddr;
    BZ_ZeroMemory(&saLocalAddr, sizeof(sockaddr_in));
    saLocalAddr.sin_family      = AF_INET;
    saLocalAddr.sin_port        = htons(usPort);
    saLocalAddr.sin_addr.s_addr = 
        ('\0' == strIpAddress[0] ? INADDR_ANY : 
                                   ::inet_addr(m_strIpAddress.c_str()));

    retCode = ::bind(m_hListenSocket, (sockaddr *)(&saLocalAddr), sizeof(sockaddr_in));
    BZ_PROCESS_ERROR(SOCKET_ERROR != retCode);

    retCode = ::listen(m_hListenSocket, BZ_MAX_BACK_LOG_NUM);
    BZ_PROCESS_ERROR(SOCKET_ERROR != retCode);
    return 0;

Exit0:
    BZ_CloseSocket(m_hListenSocket);
    return ::WSAGetLastError();
}

INT BSocketAcceptor::Wait(IN HANDLE hIocp, OUT BSocketStream &skStream)
{
    BZ_CHECK_RETURN_CODE(INVALID_HANDLE_VALUE != hIocp 
        && NULL != hIocp, -1);

    skStream.Close();
    skStream.UnInit();

    sockaddr_in          saRemoteAddr;
    SOCKET               hRemoteSocket = INVALID_SOCKET;
    BOOL   bRetCode                    = FALSE;
    INT    nAddrLen                    = sizeof(struct sockaddr_in);
    BZ_ZeroMemory(&saRemoteAddr, nAddrLen);

    hRemoteSocket = ::accept(m_hListenSocket, (sockaddr *)&saRemoteAddr, &nAddrLen);
    BZ_PROCESS_ERROR(INVALID_SOCKET != hRemoteSocket);

    skStream.Init(hRemoteSocket, ::inet_ntoa(saRemoteAddr.sin_addr),
        saRemoteAddr.sin_port);

    hIocp = ::CreateIoCompletionPort((HANDLE)hRemoteSocket, 
        hIocp,
        (ULONG_PTR)&skStream, 
        0);
    return 0;

Exit0:
    if (INVALID_SOCKET == hRemoteSocket)
    {
        return ::WSAGetLastError();
    }

    return -1;
}

INT BSocketAcceptor ::AsynWait()
{
    return 0;
}

BOOL BSocketAcceptor::UnInit()
{
    BOOL bRetCode = FALSE;

    bRetCode = BZ_CloseSocket(m_hListenSocket);
    BZ_CHECK_RETURN_BOOL(bRetCode);

    return TRUE;
}

// add end

BZ_DECLARE_NAMESPACE_END