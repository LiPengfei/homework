#ifndef __BLAZER_SABRE_SOCKETAGENCY_H__
#define __BLAZER_SABRE_SOCKETAGENCY_H__

#include "SmartPointer/SharedPtr.h"
#include "Net/SocketEvent.h"
#include "Net/SocketAcceptor.h"
#include "Structure/Buffer.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BNonBlockSocketAgency : private BUnCopyable
{
private:
    BSPNonBlockSocketAcceptor m_spAcceptor;                 // non blocked acceptor
    BSPAsyncSocketStreamQueue m_spStreamQueue;              // client socket stream queue
    BSPAsyncSocketEventArray  m_spEventArray;               // net event array

public:
    BNonBlockSocketAgency();
    virtual ~BNonBlockSocketAgency();

public:
    BOOL Init(CONST std::string &strIp, CONST USHORT &usPort);
    BOOL UnInit();
    BOOL Activate();

private:
    BOOL ProcessNetEvent();
    BOOL ProcessAllCompletePackage(BSPAsyncSocketStream &spAsyncSocketStream);

    virtual BOOL OnRecvdClientData(BSPAsyncSocketStream &spAsyncSocketStream, BSPDynamicBuffer &spBuffer);
    virtual BOOL OnClientConnected(BSPAsyncSocketStream &spAsyncSocketStream);
    virtual BOOL OnCloseConnection(BSPAsyncSocketStream &spAsyncSocketStream);
};

typedef BNonBlockSocketAgency * PNonBlockSocketAgency;
typedef BSharedPtr<BNonBlockSocketAgency> KSPNonBlockSocketAgency;


class BSocketAgency : public BUnCopyable
{
private:
    BSPSocketAcceptor m_spSockAcceptor;
    HANDLE            m_hIocp;

public:
    BSocketAgency();
    ~BSocketAgency();

public:
    BOOL Init();
    BOOL Start();
    BOOL Unit();
};

BZ_DECLARE_NAMESPACE_END

#endif