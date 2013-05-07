#include "Net/SocketStream.h"
#include "Net/SocketWrapper.h"
#include "Net/NetStruct.h"
#include "Structure/Buffer.h"
#include "SmartPointer/SharedPtr.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

BAsyncSocketStream::BAsyncSocketStream()
    :m_hSocket(INVALID_SOCKET),
    m_strRemoteIp(""),
    m_usRemotePort(0),
    m_bDelayedToCloseFlag(FALSE),
    m_bIOCPRecvCompletedFlag(FALSE),
    m_nIOCPRecvCompletedErrorCode(0),
    m_nIOCPRecvCompletedSize(0)
{ }

BAsyncSocketStream::~BAsyncSocketStream()
{
    BZ_CloseSocket(m_hSocket);
}

BOOL BAsyncSocketStream::Init(SOCKET hRemoteSocket, std::string strRemoteIp, USHORT usRemotePort)
{
    m_hSocket            = hRemoteSocket;
    m_strRemoteIp        = strRemoteIp;
    m_usRemotePort       = usRemotePort;
    return TRUE;
}

BOOL BAsyncSocketStream::Close()
{
    BZ_CloseSocket(m_hSocket);
    m_bDelayedToCloseFlag = TRUE;
    return TRUE;
}

BOOL BAsyncSocketStream::UnInit()
{
    BZ_CloseSocket(m_hSocket);  // make sure socket will be closed.
    m_hSocket               = INVALID_SOCKET;
    m_strRemoteIp                 = "";
    m_usRemotePort                = 0;
    m_bDelayedToCloseFlag         = FALSE;
    m_bIOCPRecvCompletedFlag      = FALSE;
    m_nIOCPRecvCompletedErrorCode = 0;
    m_nIOCPRecvCompletedSize      = 0;

    return TRUE;
}

BOOL BAsyncSocketStream::IsStreamNeedToClose() CONST
{
    return m_bDelayedToCloseFlag;
}

BOOL BAsyncSocketStream::IsIOCPRecvCompleted() CONST
{
    return m_bIOCPRecvCompletedFlag;
}

BOOL BAsyncSocketStream::OnStreamEstablish(OUT INT *pErrorCode)
{
    assert(FALSE == m_bIOCPRecvCompletedFlag);
    if (!TryToActivateNextRecv())
    {
        BZ_SetCode(pErrorCode, -1);
        return FALSE;
    }
    return TRUE;
}

BOOL BAsyncSocketStream::Send(IN BSPDynamicBuffer &spBuffer, OUT INT *pErrorCode)
{
    BZ_SetCode(pErrorCode, 0);              // initialise error code

    BZ_CHECK_RETURN_BOOL_QUIET(INVALID_SOCKET != m_hSocket && m_hSocket > 0);

    WORD wDataSize = static_cast<WORD>(spBuffer->GetUsedSize());
    spBuffer->InsertDataIntoHead(&wDataSize, sizeof(WORD));

    PCHAR pPackage = static_cast<PCHAR>(spBuffer->GetDataPtr());
    BZ_CHECK_RETURN_BOOL(NULL != pPackage);

    size_t nPackageSize = spBuffer->GetUsedSize();
    while(nPackageSize > 0)
    {
        INT nRetCode = ::send(m_hSocket, pPackage, nPackageSize, 0);
        if (SOCKET_ERROR != nRetCode)
        {
            pPackage     += nRetCode;
            nPackageSize -= nRetCode;
            continue;
        }

        INT nLastError = BZ_GetSocketLastError();
        if (EINTR == nLastError)
            continue;

        if (WSA_IO_PENDING == nLastError || WSA_IO_INCOMPLETE == nLastError || WSAEWOULDBLOCK == nLastError)
        {
            assert(FALSE);
            return FALSE;
        }

        return FALSE;
    }

    return TRUE;
}

BOOL BAsyncSocketStream::Recv(IN BSPDynamicBuffer &spBuffer, OUT INT *pErrorCode)
{
    BZ_SetCode(pErrorCode, 0);              // initialise error code

    if (FALSE == m_bIOCPRecvCompletedFlag)
    { // there is no data from client
        BZ_SetCode(pErrorCode, 1);
        return FALSE;
    }

    if (0 != m_nIOCPRecvCompletedErrorCode)
    { // inner system error occurs
        BZ_SetCode(pErrorCode, -1);
        return FALSE;
    }

    m_Buffer.AddUsedSize(m_nIOCPRecvCompletedSize);
    if (!m_Buffer.RemovePackage(spBuffer))
    { // data reached but there is no complete package in the buffer
        BZ_SetCode(pErrorCode, 1);
        if (!TryToActivateNextRecv())
            BZ_SetCode(pErrorCode, -1);
        return FALSE;
    }

    if (!TryToActivateNextRecv())
    {
        BZ_SetCode(pErrorCode, -1);
        return FALSE;
    }

    return TRUE;
}

BOOL BAsyncSocketStream::TryToActivateNextRecv()
{
    BOOL bLoopFlag = TRUE;
    while(bLoopFlag)
    {
        m_wsaBuffer.len = m_Buffer.GetLeftSize();
        m_wsaBuffer.buf = m_Buffer.GetLeftPtr();
        ::memset(&m_wsaOverlapped, 0, sizeof(m_wsaOverlapped));

        m_nIOCPRecvCompletedErrorCode = 0;          // reset before WSARecv
        m_nIOCPRecvCompletedSize      = 0;          // clear before WSARecv
        m_bIOCPRecvCompletedFlag      = FALSE;      // reset before WSARecv

        DWORD dwRecv = 0, dwFlags = 0;
        INT nRetCode = ::WSARecv(m_hSocket, &m_wsaBuffer, 1, &dwRecv, &dwFlags, &m_wsaOverlapped, NULL);

        if (SOCKET_ERROR == nRetCode)
        {
            INT nLastError = BZ_GetSocketLastError();
            if (EINTR == nLastError)
                continue;

            if (WSA_IO_PENDING == nLastError || WSA_IO_INCOMPLETE == nLastError || WSAEWOULDBLOCK == nLastError)
            { // no data or in processing
                return TRUE;
            }

            // other error : should close the stream.
            return FALSE;
        }

        return TRUE;
    }

    return TRUE;
}

BOOL BAsyncSocketStream::OnRecvCompleted(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    UNREFERENCED_PARAMETER(lpOverlapped);
    m_nIOCPRecvCompletedErrorCode = dwErrorCode;
    m_nIOCPRecvCompletedSize      = dwNumberOfBytesTransfered;
    m_bIOCPRecvCompletedFlag      = TRUE;
    return TRUE;
}

std::string BAsyncSocketStream::GetRemoteIp() CONST
{
    return m_strRemoteIp;
}

USHORT BAsyncSocketStream::GetRemotePort() CONST
{
    return m_usRemotePort;
}

/************************************************************************/
/* Class BSocketStream                                                  */
/************************************************************************/

BSocketStream::BSocketStream()
    : m_sock(INVALID_SOCKET),
    m_strRemoteIp(""),
    m_usRemotePort(0)
{ }

BSocketStream::~BSocketStream()
{
    BZ_CloseSocket(m_sock);
}

void BSocketStream::Init(SOCKET sock, const char *cpIp, USHORT usPort)
{
    m_sock         = sock;
    m_strRemoteIp  = cpIp;
    m_usRemotePort = usPort;
}

void BSocketStream::Close()
{
    BZ_CloseSocket(m_sock);
}

void BSocketStream::UnInit()
{
    m_sock         = INVALID_SOCKET;
    m_strRemoteIp  = "";
    m_usRemotePort = 0;
}

BOOL BSocketStream::Recv(OUT char *cpData, int Len)
{
    return TRUE;
}

int  BSocketStream::AsynRecv(IN BSockIoInfo *pSockInfo)
{
    return ::WSARecv(
        m_sock,
        &pSockInfo->m_sbuffer,
        1,
        NULL,
        &pSockInfo->m_dwFalg,
        &pSockInfo->m_ovlped,
        NULL);
}

BOOL BSocketStream::Send(const char *cpData, int nLen)
{
    int nPos = 0;
    while (nPos < nLen)
    {
        nPos += ::send(m_sock, cpData + nPos, nLen - nPos, 0);
    }

    BZ_CHECK_RETURN_BOOL(nPos == nLen);
    return TRUE;
}

BZ_DECLARE_NAMESPACE_END