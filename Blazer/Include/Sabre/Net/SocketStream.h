#ifndef __BLAZER_SABRE_SOCKETSTREM_H__
#define __BLAZER_SABRE_SOCKETSTREM_H__

#include "Public.h"
#include "SmartPointer/SharedPtr.h"
#include "Structure/UniversalManager.h"
#include <WinSock2.h>
#include <string>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#define MAX_NET_PACKAGE_SIZE 65500

/************************************************************************/
/* class SocketStream                                                   */
/************************************************************************/
class BSockIoInfo;

class BSocketStream
{
private:
    friend class BSocketConnector;
    typedef std::string STRING;

private:
    SOCKET  m_sock;
    STRING  m_strRemoteIp;      // remote ip
    USHORT  m_usRemotePort;     // remote port

    friend class BSocketConnector;
    friend class BSockIoHandleThread;
public:
    BSocketStream();
    ~BSocketStream();

public:
    void Init(SOCKET sock, const char *cpIp, USHORT usPort);
    void Close();
    void UnInit();

public:
    BOOL Recv(OUT char *cpData, int Len);
    int  AsynRecv(IN BSockIoInfo *pSockInfo);
    BOOL Send(const char *, int Len);
};

typedef BSharedPtr<BSocketStream>          BSPSocketStream;
typedef BUniversalManager<BSPSocketStream> BSocketStreamManager;
typedef BSharedPtr<BSocketStreamManager>   BSPSocketStreamManager;

BZ_DECLARE_NAMESPACE_END

#endif