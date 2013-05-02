#include "Net/SocketConnector.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

typedef std::string STRING;
BSocketConnecor::BSocketConnecor() : m_sock (INVALID_SOCKET)
{ }

BSocketConnecor::~BSocketConnecor()
{
    UnInit();
}

INT BSocketConnecor::Init()
{
    m_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    BZ_CHECK_SOCKET_RETURN_ERRCODE(m_sock);
}

INT BSocketConnecor::Connect(const char *cpIP, unsigned short nPort)
{
    sockaddr_in saAddr;
    BZ_ZeroMemory(&saAddr, sizeof(saAddr));
    saAddr.sin_family      = AF_INET;
    saAddr.sin_addr.s_addr = inet_addr(cpIP);
    saAddr.sin_port        = nPort;

    int nRetCode = ::connect(m_sock, (sockaddr *)&saAddr, sizeof(sockaddr));
    BZ_CHECK_SOCKET_RETURN_ERRCODE(nRetCode);
}

VOID BSocketConnecor::UnInit()
{
    BZ_CloseSocket(m_sock);
}

BZ_DECLARE_NAMESPACE_END