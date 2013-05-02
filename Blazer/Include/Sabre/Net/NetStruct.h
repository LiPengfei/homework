#ifndef __BLAZER_SABRE_NETSTRUCT_H__
#define __BLAZER_SABRE_NETSTRUCT_H__

#include "Public.h"
#include "DesignPattern/Singleton.h"
#include "Structure/SimpleString.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#define BZ_WINSOCKIOSIZE  4096

enum BZ_WINSOCKIO_TYPE
{
    BZ_SEND,
    BZ_RECV,
    BZ_ACCPET
};

class BWinSockIocp;

struct BSockInfo
{
    BWinSockIocp   *m_parent;
    HANDLE          m_handle;
};

struct BIoInfo
{
    OVERLAPPED        ovlped;
    // WSABuf struct;
    char              pchar[BZ_WINSOCKIOSIZE];
    int               len;
    // type
    BZ_WINSOCKIO_TYPE type;
    DWORD             dwBytesOfGet;
    DWORD             dwFalg;
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