#ifndef __BLAZER_SABRE_SCOPEDPTR_H__
#define __BLAZER_SABRE_SCOPEDPTR_H__

#include "Public.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

template <class T>
class BScopedPtr : private BUnCopyable
{
private:
    T *m_ptr;

public:
    BScopedPtr(T *ptr = NULL) : m_ptr(ptr)
    {
    }

    ~BScopedPtr() 
    {
        BZ_SafelyDeletePtr(m_ptr);
    }

public:
    T *GetPtr() CONST
    {
        return m_ptr;
    }

    void ResetPtr(T *ptr = NULL)
    {
        if (m_ptr != ptr)
            BScopedPtr<T>(ptr).Swap(*this);
    }

    T &operator*() CONST
    {
        assert(NULL != m_ptr);
        return *m_ptr;
    }

    T *operator->() CONST
    {
        assert(NULL != m_ptr);
        return m_ptr;
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
    bool operator==(const BScopedPtr &);
    bool operator!=(const BScopedPtr &);

    void Swap(BScopedPtr &r)
    {
        BZ_Swap(m_ptr, r.m_ptr);
    }
};

BZ_DECLARE_NAMESPACE_END

#endif