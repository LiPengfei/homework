#ifndef __BLAZER_SABRE_SOCKETSTREAMQUEUE_H__
#define __BLAZER_SABRE_SOCKETSTREAMQUEUE_H__

#include "SmartPointer/SharedPtr.h"
#include "Net/SocketEvent.h"
#include <vector>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BAsyncSocketStreamQueue
{
private:
    typedef std::vector<BSPAsyncSocketStream> BSP_ASYNC_STREAM_VECTOR;

private:
    BSP_ASYNC_STREAM_VECTOR m_vecAsyncSocketStream;
    size_t m_nLastWaitToProcessPos;                    // the pos of stream vector processed next time

public:
    BAsyncSocketStreamQueue();
    ~BAsyncSocketStreamQueue();

public:
    BOOL Wait(
        INT nMaxEventCount,
        INT &nEventCount,
        BSPAsyncSocketEventArray spEventArray
        );

    VOID PushBack(BSPAsyncSocketStream &spAsyncSocketStream);
    BOOL Remove(BSPAsyncSocketStream &spAsyncSocketStream);

private:
    BOOL WaitProcessRecvOrClose(
        INT nMaxEventCount,
        INT &nEventCount,
        BSPAsyncSocketEventArray spEventArray
        );
};

typedef BAsyncSocketStreamQueue *PAsyncSocketStreamQueue;
typedef BSharedPtr<BAsyncSocketStreamQueue> BSPAsyncSocketStreamQueue;

BZ_DECLARE_NAMESPACE_END

#endif