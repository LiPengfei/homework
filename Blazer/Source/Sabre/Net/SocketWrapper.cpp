#include "Net/SocketWrapper.h"

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#else
#include <netinet/in.h>         // for sockaddr_in
#include <sys/types.h>          // for socket
#include <sys/socket.h>         // for socket
#endif

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

INT    BZ_GetSocketLastError()
{
    INT nLastErrorCode = 0;
#ifdef WIN32
    nLastErrorCode = ::WSAGetLastError();
#else
    nLastErrorCode = error;
#endif
    return nLastErrorCode;
}

SOCKET BZ_CreateTCPSocket()
{
    SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    BZ_PROCESS_ERROR(s > 0);
    return s;
Exit0:
    BZ_CloseSocket(s);
    return INVALID_SOCKET;
}

SOCKET BZ_CreateUDPSocket()
{
    SOCKET s = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    BZ_CHECK_RETURN_BOOL(s > 0);
    return INVALID_SOCKET;
}

SOCKET BZ_CreateListenSocket(CONST CHAR *cszIP, USHORT usPort, INT *pErrorCode)
{
    SOCKET nListenSocket = BZ_CreateTCPSocket();
    ULONG ulAddress = INADDR_ANY;
    int nOptVal = 1, nRetCode = 0;
    sockaddr_in saLocalAddr;

    BZ_PROCESS_ERROR(INVALID_SOCKET != nListenSocket);
    if (NULL != cszIP && cszIP[0] != '\0')
    {
        if (INADDR_NONE == (ulAddress = ::inet_addr(cszIP)))
        {
            ulAddress = INADDR_ANY;
        }
    }

    nRetCode = ::setsockopt(nListenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&nOptVal, sizeof(nOptVal));
    BZ_PROCESS_ERROR(0 == nRetCode);

    saLocalAddr.sin_family = AF_INET;
    saLocalAddr.sin_addr.s_addr = ulAddress;
    saLocalAddr.sin_port = ::htons(usPort);

    nRetCode = ::bind(nListenSocket, (struct sockaddr *)&saLocalAddr, sizeof(saLocalAddr));
    BZ_PROCESS_ERROR(0 == nRetCode);

    nRetCode = ::listen(nListenSocket, BZ_MAX_BACK_LOG_NUM);
    BZ_PROCESS_ERROR(0 == nRetCode);
    return nListenSocket;
Exit0:
    BZ_SetCode(pErrorCode, BZ_GetSocketLastError());
    BZ_CloseSocket(nListenSocket);
    return nListenSocket;
}

BOOL BZ_CloseSocket(SOCKET &s)
{
    if (INVALID_SOCKET != s && SOCKET_ERROR != s)
    {
        struct linger li;
        li.l_onoff  = 1;
        li.l_linger = 0;
        ::setsockopt(s, SOL_SOCKET, SO_LINGER, (char *)&li, sizeof(li));

        BZ_CHECK_RETURN_BOOL(0 == ::closesocket(s));
        s = INVALID_SOCKET;
    }

    return TRUE;
}

BOOL   BZ_SetSocketNonBlock(SOCKET &s, INT *pErrorCode)
{
    INT nRetCode = 0;
    ULONG ulOption = 1;

    nRetCode = ::ioctlsocket(s, FIONBIO, &ulOption);
    BZ_PROCESS_ERROR(0 == nRetCode);
    return TRUE;
Exit0:
    BZ_SetCode(pErrorCode, BZ_GetSocketLastError());
    return FALSE;
}

BOOL   BZ_SetSocketRecvTimeout(SOCKET &s, DWORD dwMilliSeconds, INT *pErrorCode)
{
#ifdef WIN32
    INT nTimeOut = static_cast<INT>(dwMilliSeconds);
    INT nRetCode = 0;
    nRetCode = ::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (PCHAR)&nTimeOut, sizeof(INT));
#else
    struct timeval sTimeOut;
    sTimeOut.tv_sec = dwMilliSeconds / 1000;                                // second
    sTimeOut.tv_usec = (dwMilliSeconds - sTimeOut.tv_sec * 1000) * 1000;    // microseconds
    nRetCode = ::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (PCHAR)&sTimeOut, sizeof(struct timeval));
#endif
    BZ_PROCESS_ERROR(0 == nRetCode);
    return TRUE;
Exit0:
    BZ_SetCode(pErrorCode, BZ_GetSocketLastError());
    return FALSE;
}

BOOL   BZ_SetSocketSendTimeout(SOCKET &s, DWORD dwMilliSeconds, INT *pErrorCode)
{
#ifdef WIN32
    INT nTimeOut = static_cast<INT>(dwMilliSeconds);
    INT nRetCode = 0;
    nRetCode = ::setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (PCHAR)&nTimeOut, sizeof(INT));
#else
    struct timeval sTimeOut;
    sTimeOut.tv_sec = dwMilliSeconds / 1000;                                // second
    sTimeOut.tv_usec = (dwMilliSeconds - sTimeOut.tv_sec * 1000) * 1000;    // microseconds
    nRetCode = ::setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (PCHAR)&sTimeOut, sizeof(struct timeval));
#endif
    BZ_PROCESS_ERROR(0 == nRetCode);
    return TRUE;
Exit0:
    BZ_SetCode(pErrorCode, BZ_GetSocketLastError());
    return FALSE;
}

BZ_DECLARE_NAMESPACE_END