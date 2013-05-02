#ifndef __BLAZER_SABRE_SCOPEDARRAYPTR_H__
#define __BLAZER_SABRE_SCOPEDARRAYPTR_H__

#include "Public.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

template <class T>
class BScopedArrayPtr : private BUnCopyable
{
private:
    T *m_ptr;

public:
    BScopedArrayPtr(T *ptr = NULL) : m_ptr(ptr)
    {
    }

    ~BScopedArrayPtr()
    {
        BZ_SafelyDeleteArrayPtr(m_ptr);
    }

public:
    T *GetPtr() CONST
    {
        return m_ptr;
    }

    VOID ResetPtr(T *ptr = NULL)
    {
        if (m_ptr != ptr)
            BScopedArrayPtr<T>(ptr).Swap(*this);
    }

    T &operator[](CONST UINT pos) CONST
    {
        assert(NULL != m_ptr && pos >= 0);
        return m_ptr[pos];
    }

    operator bool() CONST
    {
        return NULL != m_ptr;
    }

    bool operator !() CONST
    {
        return NULL == m_ptr;
    }

private:
    bool operator== (const BScopedArrayPtr &);
    bool operator!= (const BScopedArrayPtr &);

    VOID Swap(BScopedArrayPtr &r)
    {
        BZ_Swap(m_ptr, r.m_ptr);
    }
};

BZ_DECLARE_NAMESPACE_END

#endif