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
{
    BZ_CloseSocket(m_sock);
}


INT BSocketConnector::Connect(
    const char *cpIP,
    unsigned short nPort,
    OUT BSocketStream *skStream)
{
    sockaddr_in saAddr;
    BZ_ZeroMemory(&saAddr, sizeof(saAddr));
    saAddr.sin_family      = AF_INET;
    saAddr.sin_addr.s_addr = inet_addr(cpIP);
    saAddr.sin_port        = nPort;

    int nRetCode = ::connect(m_sock, (sockaddr *)&saAddr, sizeof(sockaddr));
    BZ_CHECK_SOCKET_RETURN_ERRCODE(nRetCode);

    skStream->Init(m_sock, cpIP, nPort);
    return 0;
}

BZ_DECLARE_NAMESPACE_END