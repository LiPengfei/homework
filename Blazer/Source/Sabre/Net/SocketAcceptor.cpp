#include "Net/SocketAcceptor.h"
#include "Net/SocketEvent.h"
#include "Net/SocketStream.h"
#include <errno.h>
#include <string>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#ifdef WIN32
    void WINAPI IOCompletionCallBack(
    DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered,
    LPOVERLAPPED lpOverlapped)
{
    PAsyncSocketStream pAsyncSocketStream = CONTAINING_RECORD(lpOverlapped, BAsyncSocketStream, m_wsaOverlapped);
    assert(pAsyncSocketStream);

    if (dwNumberOfBytesTransfered == 0)
        dwErrorCode = WSAEDISCON;

    pAsyncSocketStream->OnRecvCompleted(dwErrorCode, dwNumberOfBytesTransfered, lpOverlapped);
}
#endif

BOOL BZ_AcceptToAsyncSocketStream(
    SOCKET hListenSocket,                           // listen socket
    PAsyncSocketStream &pAsyncSocketStream,         // async socket stream pointer
    INT *pErrorCode,                                // error code
    INT nEpollHandle)                               // used in linux
{
#ifdef WIN32
    UNREFERENCED_PARAMETER(nEpollHandle);
#else
    BZ_CHECK_RETURN_BOOL(nEpollHandle > 0);
#endif
    BZ_CHECK_RETURN_BOOL(INVALID_SOCKET != hListenSocket);        
    BZ_SetCode(pErrorCode, 0);
    struct sockaddr_in saRemoteAddr;
    SOCKET hRemoteSocket = INVALID_SOCKET;
    BOOL bLoopFlag = TRUE, bRetCode = FALSE;
    INT nAddrLen = sizeof(struct sockaddr_in);

    while(bLoopFlag)
    {
        BZ_ZeroMemory(&saRemoteAddr, nAddrLen);
        hRemoteSocket = (SOCKET)::accept(hListenSocket, (struct sockaddr *)&saRemoteAddr, &nAddrLen);

        if (SOCKET_ERROR == hRemoteSocket)
        {
            INT nErrorCode = BZ_GetSocketLastError();
            if (EINTR == nErrorCode || WSAEINTR == nErrorCode)
            { // interrupted system call, try again
                assert(FALSE);
                continue;
            }

            // WSA_IO_PENDING : overlapped operations will complete later.
            // WSAEWOULDBLOCK : non waiting connection in listen-socket queue.
            if(WSA_IO_PENDING == nErrorCode || WSAEWOULDBLOCK == nErrorCode)
            { // accept success, try next loop, do not need assert here.
                BZ_SetCode(pErrorCode, 1);
                return FALSE;
            }

            // accept fail. need assert here to indicate a serious error.
            bRetCode = FALSE;
            BZ_PROCESS_ERROR(bRetCode);
        }

        pAsyncSocketStream = ::new BAsyncSocketStream();
        BZ_PROCESS_ERROR(NULL != pAsyncSocketStream);

        bRetCode = pAsyncSocketStream->Init(
            hRemoteSocket, 
            ::inet_ntoa(saRemoteAddr.sin_addr),
            saRemoteAddr.sin_port);
        BZ_PROCESS_ERROR(bRetCode);

#ifdef WIN32
        bRetCode = ::BindIoCompletionCallback((HANDLE)hRemoteSocket, IOCompletionCallBack, 0);
        BZ_PROCESS_ERROR(bRetCode);
#else
#endif
        break;
    }

    return TRUE;
Exit0:
    if (NULL != pAsyncSocketStream)
    {
        pAsyncSocketStream->UnInit();
        BZ_SafelyDeletePtr(pAsyncSocketStream);
    }
    BZ_SetCode(pErrorCode, -1);
    return FALSE;
}

BOOL BZ_AcceptToAsyncSocketStream(
    SOCKET hListenSocket,                           // listen socket
    BSPAsyncSocketStream spAsyncSocketStream,       // async socket stream pointer
    INT *pErrorCode,                                // error code
    INT nEpollHandle)                               // used in linux
{
    BOOL bRetCode = FALSE;
    PAsyncSocketStream pAsyncSocketStream = NULL;
    bRetCode = BZ_AcceptToAsyncSocketStream(hListenSocket, pAsyncSocketStream, pErrorCode, nEpollHandle);
    spAsyncSocketStream = BSPAsyncSocketStream(pAsyncSocketStream);
    return bRetCode;
}

BNonBlockSocketAcceptor::BNonBlockSocketAcceptor() : m_hListenSocket(INVALID_SOCKET),
    m_strIpAddress(""), m_usPort(0)
{
}

BNonBlockSocketAcceptor::~BNonBlockSocketAcceptor()
{
}

BOOL BNonBlockSocketAcceptor::Init(
    CONST std::string &strIpAddress,
    CONST USHORT &usPort)
{
    m_usPort = usPort;
    m_strIpAddress = strIpAddress;

    BOOL bRetCode = FALSE;

    m_hListenSocket = BZ_CreateListenSocket(m_strIpAddress.c_str(), m_usPort);
    BZ_PROCESS_ERROR(INVALID_SOCKET != m_hListenSocket);

    bRetCode = BZ_SetSocketNonBlock(m_hListenSocket);
    BZ_PROCESS_ERROR(bRetCode);

    return TRUE;
Exit0:
    BZ_CloseSocket(m_hListenSocket);
    return FALSE;
}

BOOL BNonBlockSocketAcceptor::AttachSocketStreamQueue(BSPAsyncSocketStreamQueue &spQueue)
{
    m_spSocketStreamQueue = spQueue;
    return TRUE;
}

BOOL BNonBlockSocketAcceptor::UnInit()
{
    BOOL bRetCode = FALSE;

    bRetCode = BZ_CloseSocket(m_hListenSocket);
    BZ_CHECK_RETURN_BOOL(bRetCode);

    return TRUE;
}

BOOL BNonBlockSocketAcceptor::Wait(
    INT nMaxEventCount,
    INT &nEventCount,
    BSPAsyncSocketEventArray spEventArray)
{
    BOOL bRetCode = FALSE;

    bRetCode = WaitProcessAccept(nMaxEventCount, nEventCount, spEventArray);
    BZ_PROCESS_ERROR(bRetCode);

    return TRUE;
Exit0:
    return FALSE;
}

BOOL BNonBlockSocketAcceptor::WaitProcessAccept(
    INT nMaxEventCount,
    INT &nEventCount,
    BSPAsyncSocketEventArray spEventArray)
{
    BOOL bRetCode = FALSE;
    INT  nSuccessEventCount = 0;
    BAsyncSocketStream *pAsyncSocketStream = NULL;

    BZ_CHECK_RETURN_BOOL(nMaxEventCount > 0 && spEventArray);
    BZ_CHECK_RETURN_BOOL(nEventCount <= nMaxEventCount);

    while (nEventCount < nMaxEventCount)
    {
        INT nErrorCode = 0;
        bRetCode = BZ_AcceptToAsyncSocketStream(m_hListenSocket, pAsyncSocketStream, &nErrorCode);

        if (!bRetCode)
        {
            if (1 == nErrorCode)               // this status is valid.
                break;

            bRetCode = FALSE;
            BZ_PROCESS_ERROR(bRetCode);              // a system error is occurred.
        }

        BSPAsyncSocketStream spAsyncSocketStream(pAsyncSocketStream);
        if (m_spSocketStreamQueue)
            m_spSocketStreamQueue->PushBack(spAsyncSocketStream);

        spEventArray[nEventCount].m_nEventType = ASYNC_SOCKET_EVENT_ACCEPT;
        spEventArray[nEventCount].m_spAsyncSocketStream = spAsyncSocketStream;

        nEventCount++;
        nSuccessEventCount++;
    }

    return TRUE;
Exit0:
    while(nSuccessEventCount--)
    { // roll back
        nEventCount--;
        spEventArray[nEventCount].Reset();
    }
    return FALSE;
}



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
    saLocalAddr.sin_port        = usPort;
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
    BSPAsyncSocketStream spScokStream;
    BOOL   bRetCode                    = FALSE;
    INT    nAddrLen                    = sizeof(struct sockaddr_in);
    BZ_ZeroMemory(&saRemoteAddr, nAddrLen);

    hRemoteSocket = ::accept(m_hListenSocket, (sockaddr *)&saRemoteAddr, &nAddrLen);
    BZ_PROCESS_ERROR(INVALID_SOCKET != hRemoteSocket);

    hIocp = ::CreateIoCompletionPort((HANDLE)hRemoteSocket, 
        hIocp,
        (ULONG_PTR)this, 
        0);

    skStream.Init(hRemoteSocket, ::inet_ntoa(saRemoteAddr.sin_addr),
        ntohs(saRemoteAddr.sin_port));

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