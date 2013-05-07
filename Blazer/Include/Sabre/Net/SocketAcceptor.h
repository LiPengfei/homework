#ifndef __BLAZER_SABRE_SOCKETACCEPTOR_H__
#define __BLAZER_SABRE_SOCKETACCEPTOR_H__

#include "Public.h"
#include "Net/SocketEvent.h"
#include "Net/SocketWrapper.h"
#include "Net/SocketStreamQueue.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#ifdef WIN32
    static void WINAPI IOCompletionCallBack(
    DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered,
    LPOVERLAPPED lpOverlapped
    );
#endif

// When function return false, pErrorCode indicates what has happened.
// 0  : indicates sucess
// 1  : non waiting connection or operations will complete later
// -1 : accept failed. this indicates a serious system error
BOOL BZ_AcceptToAsyncSocketStream(
    SOCKET hListenSocket,                           // listen socket
    PAsyncSocketStream &pAsyncSocketStream,         // async socket stream pointer
    INT *pErrorCode = NULL,                         // error code
    INT nEpollHandle = -1                           // used in linux
    );

// use smart pointer
BOOL BZ_AcceptToAsyncSocketStream(
    SOCKET hListenSocket,                           // listen socket
    BSPAsyncSocketStream spAsyncSocketStream,       // async socket stream pointer
    INT *pErrorCode = NULL,                         // error code
    INT nEpollHandle = -1                           // used in linux
    );

class BNonBlockSocketAcceptor : private BUnCopyable
{
private:
    SOCKET m_hListenSocket;
    std::string m_strIpAddress;
    USHORT m_usPort;

    BSPAsyncSocketStreamQueue m_spSocketStreamQueue;

public:
    BNonBlockSocketAcceptor();
    ~BNonBlockSocketAcceptor();

    BOOL Init(
        CONST std::string &strIpAddress,
        CONST USHORT &usPort
        );

    BOOL AttachSocketStreamQueue(BSPAsyncSocketStreamQueue &spQueue);

    BOOL Wait(
        INT nMaxEventCount,
        INT &nEventCount,
        BSPAsyncSocketEventArray spEventArray
        );

    BOOL UnInit();

private:
    BOOL WaitProcessAccept(
        INT nMaxEventCount,
        INT &nEventCount,
        BSPAsyncSocketEventArray spEventArray
        );
};

typedef BNonBlockSocketAcceptor * PNonBlockSocketAcceptor;
typedef BSharedPtr<BNonBlockSocketAcceptor> BSPNonBlockSocketAcceptor;

// add by lipengfei 2013/04/02
class BSocketAcceptor
{
private:
    SOCKET                    m_hListenSocket;
    std::string               m_strIpAddress;
    USHORT                    m_usPort;
public:
    BSocketAcceptor();
    ~BSocketAcceptor();

public:
    INT Init(
        CONST std::string &strIpAddress,
        CONST USHORT &usPort
        );

    INT Wait(IN HANDLE hIocp, OUT BSocketStream &skStream);

    INT AsynWait();

    BOOL UnInit();

};

typedef BSharedPtr<BSocketAcceptor> BSPSocketAcceptor;
// add end
BZ_DECLARE_NAMESPACE_END

#endif