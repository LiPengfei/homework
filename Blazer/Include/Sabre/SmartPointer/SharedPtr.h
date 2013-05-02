#ifndef __BLAZER_SABRE_SHAREDPTR_H__
#define __BLAZER_SABRE_SHAREDPTR_H__

#include "SmartPointer/RefCounter.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

template<class T> class BWeakPtr;
template<class T> class BSharedPtr;

template<class T>
class BSharedPtr
{
public:
    typedef T elem_type;
private:
    template <class D> friend class BSharedPtr;
    template <class D> friend class BWeakPtr;
    typedef BSharedPtr<T> this_type;

private:
    T *m_ptr;
    BStrongCounter m_counter;

public:
    BSharedPtr() : m_ptr(NULL), m_counter()
    {
    }

    ~BSharedPtr()
    {

    }

    // the "explicit" keyword is necessary, or user may mistake to use KSharedPtr like this:
    // KSharedPtr<INT> sp = ::new INT(5); sp = ::new INT(6); then a memory leak will occur.
    template <class D>
    explicit BSharedPtr(D *ptr) : m_ptr(ptr), m_counter(ptr)
    {
    }

    template <class D, class Dtr>
    BSharedPtr(D *ptr, Dtr dtr) : m_ptr(ptr), m_counter(ptr, dtr)
    {
    }

    BSharedPtr(CONST BSharedPtr &r) : m_ptr(r.m_ptr), m_counter(r.m_counter)
    {
    }

    template <class D>
    BSharedPtr(CONST BSharedPtr<D> &r) : m_ptr(r.m_ptr), m_counter(r.m_counter)
    {
    }

    template <class D>
    explicit BSharedPtr(CONST BWeakPtr<D> &r) : m_ptr(r.m_ptr), m_counter(r.m_counter)
    {
        if (m_counter.IsEmpty())
        {
            assert(FALSE);  // for debug mode
            m_ptr = NULL;
        }
    }

    template <class D>
    explicit BSharedPtr(std::auto_ptr<D> &r) : m_ptr(r.get()), m_counter(r)
    {
    }

    BSharedPtr &operator=(const BSharedPtr &rhs)
    {
        this_type(rhs).Swap(*this);
        return *this;
    }

    template <class D>
    BSharedPtr &operator=(const BSharedPtr<D> &rhs)
    {
        this_type(rhs).Swap(*this);
        return *this;
    }

    template <class D>
    BSharedPtr &operator=(std::auto_ptr<D> &rhs)
    {
        this_type(rhs).Swap(*this);
        return *this;
    }

    // right value reference, r is a temp object
    BSharedPtr(BSharedPtr &&r) : m_ptr(r.m_ptr), m_counter()
    {
        m_counter.Swap(r.m_counter);
        r.m_ptr = NULL;
    }

    template <class D>
    BSharedPtr(BSharedPtr<D> &&r) : m_ptr(r.m_ptr), m_counter()
    {
        m_counter.Swap(r.m_counter);
        r.m_ptr = NULL;
    }

    BSharedPtr &operator=(BSharedPtr &&rhs)
    {
        this_type(static_cast<BSharedPtr &&>(rhs)).Swap(*this);
        return *this;
    }

    template <class D>
    BSharedPtr &operator=(BSharedPtr<D> &&rhs)
    {
        this_type(static_cast<BSharedPtr<D> &&>(rhs)).Swap(*this);
        return *this;
    }

    VOID Reset()
    {
        this_type().Swap(*this);
    }

    template <class D>
    VOID Reset(D *ptr)
    {
        assert(NULL == ptr || m_ptr != ptr);
        this_type(ptr).Swap(*this);
    }

    template <class D, class Dtr>
    VOID Reset(D *ptr, Dtr dtr)
    {
        assert(NULL == ptr || m_ptr != ptr);
        this_type(ptr, dtr).Swap(*this);
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

    VOID Swap(BSharedPtr &r)
    {
        BZ_Swap(m_ptr, r.m_ptr);
        m_counter.Swap(r.m_counter);
    }
};

template <class T>
inline bool operator==(CONST BSharedPtr<T> &lhs, CONST BSharedPtr<T> &rhs)
{
    return lhs.Get() == rhs.Get();
}

template <class T>
inline bool operator!=(CONST BSharedPtr<T> &lhs, CONST BSharedPtr<T> &rhs)
{
    return lhs.Get() != rhs.Get();
}

template <class T1, class T2>
inline bool operator==(CONST BSharedPtr<T1> &lhs, CONST BSharedPtr<T2> &rhs)
{
    return lhs.Get() == rhs.Get();
}

template <class T1, class T2>
inline bool operator!=(CONST BSharedPtr<T1> &lhs, CONST BSharedPtr<T2> &rhs)
{
    return lhs.Get() != rhs.Get();
}

template <class T>
inline VOID g_SwapSP(CONST BSharedPtr<T> &lhs, CONST BSharedPtr<T> &rhs)
{
    return lhs.Swap(rhs);
}

template<class T> 
class BWeakPtr
{
public:
    typedef T elem_type;
private:
    template <class D> friend class BSharedPtr;
    template <class D> friend class BWeakPtr;
    typedef BWeakPtr<T> this_type;

private:
    T *m_ptr;
    BWeakCounter m_counter;

private:
    template<class D> BWeakPtr(D *ptr);

public:
    BWeakPtr() : m_ptr(NULL), m_counter()
    {
    }

    BWeakPtr(CONST BWeakPtr &r) : m_ptr(r.Lock().Get()), m_counter(r.m_counter)
    {
    }

    template <class D>
    BWeakPtr(CONST BWeakPtr<D> &r) : m_ptr(r.Lock().Get()), m_counter(r.m_counter)
    {
    }

    template <class D>
    BWeakPtr(CONST BSharedPtr<D> &r) : m_ptr(r.m_ptr), m_counter(r.m_counter)
    {
    }

    BWeakPtr &operator=(CONST BWeakPtr &rhs)
    {
        m_ptr = rhs.Lock().Get();
        m_counter = rhs.m_counter;
        return *this;
    }

    template<class D>
    BWeakPtr &operator=(CONST BWeakPtr<D> &rhs)
    {
        m_ptr = rhs.Lock().Get();
        m_counter = rhs.m_counter;
        return *this;
    }

    template<class D>
    BWeakPtr &operator=(CONST BSharedPtr<D> &rhs)
    {
        m_ptr = rhs.m_ptr;
        m_counter = rhs.m_counter;
        return *this;
    }

    // right value reference, r is a temp object
    BWeakPtr(BWeakPtr &&r) : m_ptr(r.Lock().Get()), m_counter()
    {
        m_counter.Swap(r.m_counter);
        r.m_ptr = NULL;
    }

    template <class D>
    BWeakPtr(BWeakPtr<D> &&r) : m_ptr(r.Lock().Get()), m_counter()
    {
        m_counter.Swap(r.m_counter);
        r.m_ptr = NULL;
    }

    BWeakPtr &operator=(BWeakPtr &&rhs)
    {
        this_type(static_cast<BWeakPtr &&>(rhs)).Swap(*this);
        return *this;
    }

    template <class D>
    BWeakPtr &operator=(BWeakPtr<D> &&rhs)
    {
        this_type(static_cast<BWeakPtr<D> &&>(rhs)).Swap(*this);
        return *this;
    }

    BSharedPtr<T> Lock() CONST
    {
        return BSharedPtr<T>(*this);
    }

    BOOL IsEpired() CONST
    {
        return 0 == m_counter.GetStrongRefCounter();
    }

    LONG GetStrongRefCounter() CONST
    {
        return m_counter.GetStrongRefCounter();
    }

    LONG GetWeakRefCounter() CONST
    {
        return m_counter.GetWeakRefCounter();
    }

    VOID Reset()
    {
        BWeakPtr().swap(*this);
    }

    VOID Swap(BWeakPtr &r)
    {
        BZ_Swap(m_ptr, r.m_ptr);
        m_counter.Swap(r.m_counter);
    }
};

template <class T>
inline bool operator==(CONST BWeakPtr<T> &lhs, CONST BWeakPtr<T> &rhs)
{
    return lhs.Get() == rhs.Get();
}

template <class T>
inline bool operator!=(CONST BWeakPtr<T> &lhs, CONST BWeakPtr<T> &rhs)
{
    return lhs.Get() != rhs.Get();
}

template <class T1, class T2>
inline bool operator==(CONST BWeakPtr<T1> &lhs, CONST BWeakPtr<T2> &rhs)
{
    return lhs.Get() == rhs.Get();
}

template <class T1, class T2>
inline bool operator!=(CONST BWeakPtr<T1> &lhs, CONST BWeakPtr<T2> &rhs)
{
    return lhs.Get() != rhs.Get();
}

template <class T>
inline VOID g_SwapSP(CONST BWeakPtr<T> &lhs, CONST BWeakPtr<T> &rhs)
{
    return lhs.Swap(rhs);
}

BZ_DECLARE_NAMESPACE_END

#endif