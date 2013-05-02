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
        typedef BUniversalQueue<T>                 BUniversalQueue;
        typedef BUniversalQueue *                  PUniversalQueue;
        typedef BSharedPtr<BUniversalQueue>        BSPUniversalQueue;
        typedef std::map<DWORD, BSPUniversalQueue> BUniversalQueueMap;

    private:
        BUniversalQueueMap m_universalQueueMap;
        BThreadMutex       m_mutex;

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
        BSPUniversalQueue GetUniversalQueue(DWORD dwQueueID)
        {
            BOOL bRetCode = FALSE;

            BGuard<BThreadMutex> locker(m_mutex);
            BUniversalQueueMap::iterator iter = m_universalQueueMap.find(dwQueueID);
            if (m_universalQueueMap.end() != iter)
            {
                return iter->second;
            }

            BSPUniversalQueue spUniversalQueue(::new BUniversalQueue());
            bRetCode = spUniversalQueue->SetQueueID(dwQueueID);
            AddUniversalQueue(dwQueueID, spUniversalQueue);

            return spUniversalQueue;
        }

        BOOL AddUniversalQueue(DWORD dwQueueID, BSPUniversalQueue spUniversalQueue)
        {
            BZ_CHECK_RETURN_BOOL(spUniversalQueue);
            m_universalQueueMap[dwQueueID] = spUniversalQueue;
            return TRUE;
        }
    };

    //template class COMMON_API KSingleton<KNetService>;
BZ_DECLARE_NAMESPACE_END

#endif

