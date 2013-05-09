#include "Net/SocketStream.h"
#include "Net/SocketWrapper.h"
#include "Net/NetStruct.h"
#include "Structure/Buffer.h"
#include "SmartPointer/SharedPtr.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

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