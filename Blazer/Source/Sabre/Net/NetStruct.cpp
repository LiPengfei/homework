#include "Net/NetStruct.h"
#include "WinSock2.h"

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#else
#include <netinet/in.h>         // for sockaddr_in
#include <sys/types.h>          // for socket
#include <sys/socket.h>         // for socket
#endif

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

BNetService::BNetService() : m_lStarted(0)
{
    BNetService::Start();
}

BNetService::~BNetService()
{
    BNetService::Stop();
}

BOOL BNetService::Start(WORD wHighVersion, WORD wLowVersion)
{
#ifdef WIN32
    if (1 == BZ_AtomicIncrement(m_lStarted))
    {
        WSADATA wsaData;
        WORD wVersionRequested = MAKEWORD(wHighVersion, wLowVersion);
        BZ_CHECK_RETURN_BOOL(0 == ::WSAStartup(wVersionRequested, &wsaData));
    }
    else
        BZ_AtomicDecrement(m_lStarted);
#endif
    return TRUE;
}

BOOL BNetService::Stop()
{
#ifdef WIN32
    if (0 == BZ_AtomicDecrement(m_lStarted))
    {
        BZ_CHECK_RETURN_BOOL_QUIET(0 == ::WSACleanup());
    }
#endif
    return TRUE;
}

BZ_DECLARE_NAMESPACE_END

