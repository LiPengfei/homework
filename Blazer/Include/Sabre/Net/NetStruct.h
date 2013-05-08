#ifndef __BLAZER_SABRE_NETSTRUCT_H__
#define __BLAZER_SABRE_NETSTRUCT_H__

#include "Public.h"
#include "DesignPattern/Singleton.h"
#include "Structure/SimpleString.h"

#include <WinSock2.h>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#define BZ_WIN_IOCP_ASYBUFFER  1024 * 4

const char* const BZ_SOCKET_AGENCY_CONFIG        = "config_server.ini";

enum BZ_WINSOCKIO_TYPE
{
    BZ_WINSOCK_SEND,
    BZ_WINSOCK_RECV,
    BZ_WINSOCK_ACCPET
};


class BWinSockIocp;
struct BSockInfo
{
    BWinSockIocp   *m_parent;
    HANDLE          m_handle;
};

class BSockIoInfo
{
private:
    friend class BSocketStream;
    friend class BSockIoHandleThread;

private:
    OVERLAPPED        m_ovlped;

    // WSABuf struct; 
    WSABUF            m_sbuffer;
    int               m_len;
    char              m_cpBuffer[BZ_WIN_IOCP_ASYBUFFER];

    // type
    BZ_WINSOCKIO_TYPE m_type;
    DWORD             m_dwBytesOfGet;
    DWORD             m_dwFalg;

public:
    void Reset()
    {
        BZ_ZeroMemory(this, sizeof(BSockIoInfo));
        m_len          = BZ_WIN_IOCP_ASYBUFFER;
        m_sbuffer.len  = m_len;
        m_sbuffer.buf  = m_cpBuffer;
        m_dwFalg       = 0;
        m_type         = BZ_WINSOCK_RECV;
        m_dwBytesOfGet = 0;
    }
};

class BNetService : private BUnCopyable
{
    DECLARE_SINGLETON_PATTERN(BNetService);
private:
    LONG m_lStarted;

private:
    BNetService();
public:
    ~BNetService();

public:
    BOOL Start(WORD wHighVersion = 2, WORD wLowVersion = 2);
    BOOL Stop();
};

BZ_DECLARE_NAMESPACE_END

#endif