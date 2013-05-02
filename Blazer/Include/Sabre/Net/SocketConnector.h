#ifndef __BLAZER_SABRE_SOCKETCONNECTOR_H__
#define __BLAZER_SABRE_SOCKETCONNECTOR_H__

#include "Public.h"
#include "Net/SocketStream.h"
#include <WinSock2.h>
#include <string>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BSocketConnecor : public BUnCopyable
{
private:
    SOCKET               m_sock ;

public:
    BSocketConnecor();
    ~BSocketConnecor();

public:
    INT  Init();
    INT  Connect(const char* cpIp, unsigned short nPort);
    VOID UnInit();
};

BZ_DECLARE_NAMESPACE_END

#endif