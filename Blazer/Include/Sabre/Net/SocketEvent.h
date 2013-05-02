#ifndef __BLAZER_SABRE_SOCKETEVENT_H__
#define __BLAZER_SABRE_SOCKETEVENT_H__

#include "SmartPointer/SharedArrayPtr.h"
#include "Net/SocketStreamBuffer.h"
#include "Net/SocketStream.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

enum BAsyncSocketEventType
{
    ASYNC_SOCKET_EVENT_INVALID = 0,          // invalid event
    ASYNC_SOCKET_EVENT_ACCEPT  = 1,          // socket accept
    ASYNC_SOCKET_EVENT_IN      = 2,          // socket ¿É¶Á
    ASYNC_SOCKET_EVENT_OUT     = 3,          // socket ¿ÉÐ´

};

#define MAX_WAIT_ACCEPT_EVENT 32
#define MAX_SOCKET_EVENT (1024 + MAX_WAIT_ACCEPT_EVENT)

struct BAsyncSocketEvent
{
public:
    BAsyncSocketEventType m_nEventType;
    BSPAsyncSocketStream  m_spAsyncSocketStream;

public:
    BAsyncSocketEvent() : m_nEventType(ASYNC_SOCKET_EVENT_INVALID)
    {
    }

    ~BAsyncSocketEvent()
    {
    }

public:
    VOID Reset()
    {
        m_nEventType = ASYNC_SOCKET_EVENT_INVALID;
        m_spAsyncSocketStream = BSPAsyncSocketStream();
    }
};

typedef BAsyncSocketEvent *PAsyncSocketEvent;
typedef BSharedArrayPtr<BAsyncSocketEvent> BSPAsyncSocketEventArray;

BZ_DECLARE_NAMESPACE_END

#endif