#ifndef __BLAZER_SABRE_SOCKETCONNECTOR_H__
#define __BLAZER_SABRE_SOCKETCONNECTOR_H__

#include "Public.h"
#include <WinSock2.h>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BSocketStream;

class BSocketConnector : public BUnCopyable
{
private:
    SOCKET m_sock;

public:
    BSocketConnector();
    ~BSocketConnector();

public:
    INT  Connect(const char* cpIp, unsigned short nPort, OUT BSocketStream *skStream);
    void Close();
};

BZ_DECLARE_NAMESPACE_END

#endif