#ifndef _K_GAME_SERVER_CLIENT_AGENCY_H_
#define _K_GAME_SERVER_CLIENT_AGENCY_H_

#include "Net/SocketAgency.h"
using namespace BZ(sabre);

namespace kserver_gs
{
    class KClientAgency : public BNonBlockSocketAgency
    {
    private:

    public:
        KClientAgency();
        virtual ~KClientAgency();

    private:
        virtual BOOL OnClientConnected(BSPAsyncSocketStream &spAsyncSocketStream);
        virtual BOOL OnCloseConnection(BSPAsyncSocketStream &spAsyncSocketStream);
        virtual BOOL OnRecvdClientData(BSPAsyncSocketStream &spAsyncSocketStream, BSPDynamicBuffer &spBuffer);
    };
}

#endif