#include "Net/SocketConnector.h"
#include "Net/SocketWrapper.h"
#include "Net/SocketStream.h"

#include <string>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

typedef std::string STRING;
BSocketConnector::BSocketConnector() : m_sock (INVALID_SOCKET)
{
    m_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    BZ_CHECK_SOCKET_RETURN_VOID(m_sock);
}

BSocketConnector::~BSocketConnector()
{ }

INT BSocketConnector::Connect(
    const char *cpIP,
    unsigned short nPort,
    OUT BSocketStream *skStream)
{
    sockaddr_in saAddr;
    BZ_ZeroMemory(&saAddr, sizeof(saAddr));
    saAddr.sin_family      = AF_INET;
    saAddr.sin_addr.s_addr = inet_addr(cpIP);
    saAddr.sin_port        = htons(nPort);

    int nRetCode           = ::connect(m_sock, (sockaddr *)&saAddr, sizeof(sockaddr));

    BZ_CHECK_RETURN_CODE(0 == nRetCode, -1);

    if (skStream)
    {
        skStream->Init(m_sock, cpIP, nPort);
    }

    return 0;
}

void BSocketConnector::Close()
{
    ::closesocket(m_sock);
}

BZ_DECLARE_NAMESPACE_END
