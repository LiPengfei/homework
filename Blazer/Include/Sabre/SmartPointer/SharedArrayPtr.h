#ifndef __BLAZER_SABRE_SHAREDARRAYPTR_H__
#define __BLAZER_SABRE_SHAREDARRAYPTR_H__

#include "Public.h"
#include "SmartPointer/RefCounter.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

template <class T>
class BSharedArrayPtr
{
private:
    typedef VOID (*Dtr)(T *&p);
    typedef BSharedArrayPtr<T> this_type;
public:
    typedef T elem_type;

private:
    T *m_ptr;
    BStrongCounter m_counter;

public:
    // the "explicit" keyword is necessary, or user may mistake to use KSharedArrayPtr like this:
    // KSharedArrayPtr<INT> sp = ::new INT[5]; sp = ::new INT[6]; then a memory leak will occur.
    explicit BSharedArrayPtr(T *ptr = NULL) : m_ptr(ptr), m_counter(ptr, BZ_SafelyDeleteArrayPtr<T>)
    {
    }

    template <class Dtr>
    BSharedArrayPtr(T *ptr, Dtr dtr) : m_ptr(ptr), m_counter(ptr, dtr)
    {
    }

    BSharedArrayPtr(CONST BSharedArrayPtr &r) : m_ptr(r.m_ptr), m_counter(r.m_counter)
    {
    }

    BSharedArrayPtr &operator=(CONST BSharedArrayPtr &rhs)
    {
        this_type(rhs).Swap(*this);
        return *this;
    }

    VOID Reset(T *ptr = NULL)
    {
        assert(NULL == ptr || m_ptr != ptr);
        this_type(ptr).Swap(*this);
    }

    template <class Dtr>
    VOID Reset(T *ptr, Dtr dtr)
    {
        assert(NULL == ptr || m_ptr != ptr);
        this_type(ptr, dtr).Swap(*this);
    }

    VOID Swap(BSharedArrayPtr &r)
    {
        BZ_Swap(m_ptr, r.m_ptr);
        m_counter.Swap(r.m_counter);
    }

    T &operator[](std::ptrdiff_t i) CONST
    {
        assert(m_ptr != NULL && i >= 0);
        return m_ptr[i];
    }

    operator bool() CONST
    {
        return (NULL != m_ptr && !m_counter.IsEmpty());
    }

    bool operator !() CONST
    {
        return (NULL == m_ptr || m_counter.IsEmpty());
    }

    T *Get() CONST
    {
        return m_ptr;
    }

    BOOL IsUnique() CONST
    {
        return m_counter.IsUnique();
    }

    LONG GetStrongRefCounter() CONST
    {
        return m_counter.GetStrongRefCounter();
    }

    LONG GetWeakRefCounter() CONST
    {
        return m_counter.GetWeakRefCounter();
    }
};

template <class T>
inline bool operator==(CONST BSharedArrayPtr<T> &lhs, CONST BSharedArrayPtr<T> &rhs)
{
    return lhs.Get() == rhs.Get();
}

template <class T>
inline bool operator!=(CONST BSharedArrayPtr<T> &lhs, CONST BSharedArrayPtr<T> &rhs)
{
    return lhs.Get() != rhs.Get();
}

template <class T>
inline VOID g_SwapSP(CONST BSharedArrayPtr<T> &lhs, CONST BSharedArrayPtr<T> &rhs)
{
    return lhs.Swap(rhs);
}

BZ_DECLARE_NAMESPACE_END

#endif