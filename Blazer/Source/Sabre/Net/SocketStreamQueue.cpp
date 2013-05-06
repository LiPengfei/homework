#include "Net/SocketStreamQueue.h"
#include <algorithm>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

BAsyncSocketStreamQueue::BAsyncSocketStreamQueue() : m_nLastWaitToProcessPos(0)
{
    m_vecAsyncSocketStream.clear();
}

BAsyncSocketStreamQueue::~BAsyncSocketStreamQueue()
{
}

BOOL BAsyncSocketStreamQueue::Wait(
    INT nMaxEventCount,
    INT &nEventCount,
    BSPAsyncSocketEventArray spEventArray)
{
    BOOL bRetCode = FALSE;
    bRetCode = WaitProcessRecvOrClose(nMaxEventCount, nEventCount, spEventArray);
    BZ_PROCESS_ERROR(bRetCode);
    return TRUE;
Exit0:
    return FALSE;
}

void BAsyncSocketStreamQueue::PushBack(BSPAsyncSocketStream &spAsyncSocketStream)
{
    m_vecAsyncSocketStream.push_back(spAsyncSocketStream);
}

BOOL BAsyncSocketStreamQueue::Remove(BSPAsyncSocketStream &spAsyncSocketStream)
{
    BSP_ASYNC_STREAM_VECTOR::iterator iterBegin = m_vecAsyncSocketStream.begin();
    BSP_ASYNC_STREAM_VECTOR::iterator iterEnd   = m_vecAsyncSocketStream.end();
    BSP_ASYNC_STREAM_VECTOR::iterator iter = std::find(iterBegin, iterEnd, spAsyncSocketStream);
    if (iter != iterEnd)
        m_vecAsyncSocketStream.erase(iter);
    return TRUE;
}

BOOL BAsyncSocketStreamQueue::WaitProcessRecvOrClose(
    INT nMaxEventCount,
    INT &nEventCount,
    BSPAsyncSocketEventArray spEventArray)
{
    BZ_CHECK_RETURN_BOOL(nMaxEventCount > 0 && spEventArray && nEventCount <= nMaxEventCount);

    size_t nStreamVectorLen = m_vecAsyncSocketStream.size();
    if (m_nLastWaitToProcessPos >= nStreamVectorLen)            // get the right target pos
        m_nLastWaitToProcessPos = 0;

    BSP_ASYNC_STREAM_VECTOR::iterator iterBegin = m_vecAsyncSocketStream.begin();
    BSP_ASYNC_STREAM_VECTOR::iterator iterEnd   = m_vecAsyncSocketStream.end();
    BSP_ASYNC_STREAM_VECTOR::iterator iterNext  = iterBegin;
    advance(iterNext, m_nLastWaitToProcessPos);                 // jump to the target pos

    size_t nToProcessEventCount = nStreamVectorLen;
    while(nToProcessEventCount--)
    {
        if (iterNext == iterEnd)                                // restart again
            iterNext = iterBegin;
        BSP_ASYNC_STREAM_VECTOR::iterator iterTemp = iterNext++;// current iterator

        BSPAsyncSocketStream spAsyncSocketStream = *iterTemp;
        assert(spAsyncSocketStream);

        // check need to close first in incase that KAsyncSocketStream::Close() was invoked before
        // ::WSARecv was invoked.
        if (spAsyncSocketStream->IsStreamNeedToClose())
        { // if stream should be closed, close then continue.
            iterNext  = m_vecAsyncSocketStream.erase(iterTemp);
            iterBegin = m_vecAsyncSocketStream.begin();
            iterEnd   = m_vecAsyncSocketStream.end();
            spAsyncSocketStream->UnInit();
            continue;
        }

        if (!spAsyncSocketStream->IsIOCPRecvCompleted())
        { // if iocp not recv completed, then continue.
            continue;
        }

        if (nEventCount >= nMaxEventCount)                      // if reach max event count, then break
            break;

        spEventArray[nEventCount].m_nEventType = ASYNC_SOCKET_EVENT_IN;
        spEventArray[nEventCount].m_spAsyncSocketStream = spAsyncSocketStream;
        nEventCount++;
    }

    m_nLastWaitToProcessPos = distance(iterBegin, iterNext);
    return TRUE;
}

BZ_DECLARE_NAMESPACE_END