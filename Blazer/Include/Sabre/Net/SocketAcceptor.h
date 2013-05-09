#ifndef __BLAZER_SABRE_SOCKETACCEPTOR_H__
#define __BLAZER_SABRE_SOCKETACCEPTOR_H__

#include "Public.h"
#include "Net/SocketWrapper.h"
#include "SmartPointer/SharedPtr.h"
#include <string>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BSocketStream;
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