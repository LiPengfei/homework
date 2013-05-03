#ifndef __BLAZER_SABRE_UNIVERSALMANAGER_H__
#define __BLAZER_SABRE_UNIVERSALMANAGER_H__

#include "Public.h"

#include <map>
using namespace std;

BZ_DECLARE_NAMESPACE_BEGIN(sabre)
    
    template <class T>
class BUniversalManager : private BUnCopyable
{
private:
    typedef std::map<DWORD, T> BElementMap;

private:
    BElementMap m_elementMap;

public:
    BUniversalManager()
    {
    }

    virtual ~BUniversalManager()
    {
    }

public:
    virtual BOOL Get(IN CONST DWORD dwID, INOUT T &element)
    {
        BElementMap::iterator iter = m_elementMap.find(dwID);
        if (m_elementMap.end() != iter)
        {
            element = iter->second;
            return TRUE;
        }

        return FALSE;
    }

    virtual BOOL Add(IN CONST DWORD dwID, IN CONST T &element)
    {
        BZ_CHECK_RETURN_BOOL(0 != dwID);
        m_elementMap[dwID] = element;
        return TRUE;
    }

    virtual BOOL Del(IN CONST DWORD dwID = 0)
    {
        BZ_CHECK_RETURN_BOOL(0 != dwID);
        m_elementMap.erase(dwID);
        return TRUE;
    }
};

BZ_DECLARE_NAMESPACE_END

#endif

