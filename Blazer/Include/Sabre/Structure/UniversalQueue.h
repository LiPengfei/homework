#ifndef __BLAZER_SABRE_UNIVERSALQUEUE_H__
#define __BLAZER_SABRE_UNIVERSALQUEUE_H__

#include "Public.h"
#include "Synchronize/ThreadSynchronize.h"
#include "Synchronize/ThreadMutex.h"
#include "Synchronize/ThreadSemaphore.h"
#include "SmartPointer/SharedPtr.h"
#include "DesignPattern/Singleton.h"
#include <map>
#include <list>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

    template <class T>
class BUniversalQueue : private BUnCopyable
{
private:
    typedef std::list<T> KElementQueue;

private:
    DWORD            m_dwQueueID;                      // the id of the universal queue
    KElementQueue    m_elementQueue;                   // the real queue
    BThreadSemaphore m_semaphore;                      // the manually reset event
    BThreadMutex     m_cs;                             // the critical section

public:
    BUniversalQueue() : m_dwQueueID(0)
    {
    }

    virtual ~BUniversalQueue()
    {
    }

public:
    BOOL  SetQueueID(CONST DWORD dwQueueID)
    {
        assert(0 != dwQueueID);
        m_dwQueueID = dwQueueID;
        return TRUE;
    }

    DWORD GetQueueID() CONST
    {
        return m_dwQueueID;
    }

    BOOL  PushNode(CONST T &node)
    {
        BGuard<BThreadMutex> guard(m_cs);
        m_elementQueue.push_back(node);
        m_semaphore.ReleaseSemaphore();
        return TRUE;
    }

    BOOL  PopNode (T &node)
    {
        if(!m_semaphore.WaitSemaphore())
        {
            return FALSE;
        }

        BGuard<BThreadMutex> guard(m_cs);
        node = *(m_elementQueue.begin());
        m_elementQueue.pop_front();
        return TRUE;
    }

    T   PopNode()
    {
        m_semaphore.WaitSemaphore();
        BGuard<BThreadMutex> guard(m_cs);
        T node = *(m_elementQueue.begin());     // copy the node then delete it.
        m_elementQueue.pop_front();
        return node;
    }
};

template <class T>
class BUniversalQueueManagement : private BUnCopyable
{
    DECLARE_SINGLETON_PATTERN(BUniversalQueueManagement);

private:
    typedef BUniversalQueue<T>                     BUniversalTempQueue;
    typedef BUniversalQueue<T> *                   PUniversalTempQueue;
    typedef BSharedPtr<BUniversalTempQueue>        BSPUniversalTempQueue;
    typedef std::map<DWORD, BSPUniversalTempQueue> BUniversalQueueMap;

private:
    BUniversalQueueMap m_universalQueueMap;

public:
    BUniversalQueueManagement()
    {
        m_universalQueueMap.clear();
    }

    ~BUniversalQueueManagement()
    {
        m_universalQueueMap.clear();
    }

public:
    BOOL GetUniversalQueue(DWORD dwQueueID, BSPUniversalTempQueue &pQueue)
    {
        // modified by lipengfei 13/05/03
        BUniversalQueueMap::iterator iter = m_universalQueueMap.find(dwQueueID);
        if (m_universalQueueMap.end() != iter)
        {
            pQueue = iter->second;
            return TRUE;
        }
        assert(false);

        return FALSE;
    }

    BOOL AddUniversalQueue(DWORD dwQueueID, BSPUniversalTempQueue spUniversalQueue)
    {
        BZ_CHECK_RETURN_BOOL(spUniversalQueue);
        m_universalQueueMap[dwQueueID] = spUniversalQueue;
        return TRUE;
    }

    // add by lipengfei 13/05/03
    BOOL DelUniVersalQueue(DWORD dwQueueID)
    {
        BUniversalQueueMap::iterator iter = m_universalQueueMap.find(dwQueueID);
        if (m_universalQueueMap.end() != iter)
        {
            m_universalQueueMap.erase(iter);
            return TRUE;
        }

        assert(false);
        return FALSE;     
    }
};

//template class COMMON_API KSingleton<KNetService>;
BZ_DECLARE_NAMESPACE_END

#endif

