#ifndef __BLAZER_SABRE_SOCKETAGENCY_H__
#define __BLAZER_SABRE_SOCKETAGENCY_H__

#include "SmartPointer/SharedPtr.h"
#include "Net/SocketAcceptor.h"
#include "Structure/Buffer.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

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
    BOOL UnInit();
};

BZ_DECLARE_NAMESPACE_END

#endif