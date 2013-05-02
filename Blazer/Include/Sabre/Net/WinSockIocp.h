#ifndef __BLAZER_SABRE_WINSOCKIOCP_H__
#define __BLAZER_SABRE_WINSOCKIOCP_H__

#include "Public.h"
#include "NetStruct.h"
#include <vector>

#ifdef  _WIN32

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BWinSockIocp
{
private:
    typedef std::vector<BSockInfo> VecDevices;

public:
    HANDLE          m_hIocp;  
    VecDevices      m_hArray;

public:
    BWinSockIocp()
    {
        m_hIocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    }

    BOOL AssociateDevice(BSockInfo &d, ULONG_PTR key)
    {
        HANDLE h = ::CreateIoCompletionPort(d.m_handle, m_hIocp, key, 0);
        if (m_hIocp == h)
        {
            d.m_parent = this;
            m_hArray.push_back(d);
            return TRUE;
        }

        return FALSE;
    }

    BOOL DetechDevice(HANDLE h)
    {
        VecDevices::iterator it = m_hArray.begin();
        while (m_hArray.end() != it)
        {
            if (h == it->m_handle)
            {
                m_hArray.erase(it);
                return TRUE;
            }
            ++it;
        }

        return FALSE;
    }

    BOOL WaitStatus(DWORD *pdwNumBytes, ULONG_PTR *ppkey, OVERLAPPED **ppolped)
    {
        return ::GetQueuedCompletionStatus(
            m_hIocp, 
            pdwNumBytes, 
            ppkey, 
            ppolped, 
            INFINITE);
    }

    BOOL SendStatus(DWORD dwNumBytes, ULONG_PTR dwKey, OVERLAPPED *polped)
    {
        return ::PostQueuedCompletionStatus(m_hIocp, dwNumBytes, dwKey, polped);
    }

    virtual ~BWinSockIocp()
    {
        ::CloseHandle(m_hIocp);
        m_hArray.clear();
    }
};

BZ_DECLARE_NAMESPACE_END

#endif //  _WIN32

#endif