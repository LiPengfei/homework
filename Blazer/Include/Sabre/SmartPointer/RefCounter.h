#ifndef __BLAZER_SABRE_REFCOUNTER_H__
#define __BLAZER_SABRE_REFCOUNTER_H__

#include "Public.h"

#include <memory>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BRefCounterBase : private BUnCopyable
{
private:
    // The strong-counter use weak-reference-counter once.
    LONG m_lStrongRefCounter;   // control the life-cycle of res 
    LONG m_lWeakRefCounter;     // control the life cycle of KRefCounter and res-pointer

public:
    BRefCounterBase() : m_lStrongRefCounter(1), m_lWeakRefCounter(1)
    {
    }

    virtual ~BRefCounterBase()
    {
    }

private:
    virtual VOID DestroyRes() = 0;   // derived class realizes how to release resources.
    virtual VOID DestroySelf()       // destroy KRefCounter instance
    {
        BZ_SafelyDeleteConstPtr(this);
    }

public:
    VOID DelStrongRef()
    {
        if (0 == g_AtomicDecrement(m_lStrongRefCounter))
        {
            DestroyRes();
            DelWeakRef();
        }
    }

    VOID DelWeakRef()
    {
        if (0 == g_AtomicDecrement(m_lWeakRefCounter))
        {
            DestroySelf();
        }
    }

    VOID AddStrongRef()
    {
        LONG lRetValue = g_AtomicIncrement(m_lStrongRefCounter);
        assert(lRetValue > 0);
    }

    VOID AddWeakRef()
    {
        LONG lRetValue = g_AtomicIncrement(m_lWeakRefCounter);
        assert(lRetValue > 0);
    }

    LONG GetStrongRefCounter() CONST
    {
        return static_cast<CONST LONG volatile &>(m_lStrongRefCounter);
    }

    LONG GetWeakRefCounter() CONST
    {
        return static_cast<CONST LONG volatile &>(m_lWeakRefCounter);
    }

    virtual PVOID GetDeleter() = 0;
};

template <class T>
class BRefCounter : public BRefCounterBase
{
private:
    typedef VOID (*Dtr)(T *&p);
    typedef BRefCounter<T> this_type;

private:
    T *m_ptr;       // ptr to the real res
    Dtr m_deteler;  // deleter

public:
    BRefCounter() : m_ptr(NULL), m_deteler(BZ_SafelyDeletePtr<T>)
    {
    }

    explicit BRefCounter(T *ptr) : m_ptr(ptr), m_deteler(BZ_SafelyDeletePtr<T>)
    {
    }

    BRefCounter(T *ptr, Dtr dtr) : m_ptr(ptr), m_deteler(dtr)
    {
    }

    virtual ~BRefCounter()
    {
    }

public:
    virtual PVOID GetDeleter()
    {
        return static_cast<PVOID>(m_deteler);
    }

private:
    virtual VOID DestroyRes()
    {
        m_deteler(m_ptr);
    }
};

class BWeakCounter;
class BStrongCounter
{
private:
    friend class BWeakCounter;

private:
    BRefCounterBase *m_pCounter;

public:
    BStrongCounter() : m_pCounter(NULL)
    {
    }

    template <class D>
    explicit BStrongCounter(D *ptr) : m_pCounter(NULL)
    {
        m_pCounter = ::new BRefCounter<D>(ptr);
        assert(NULL != m_pCounter);
    }

    template <class D, class Dtr>
    BStrongCounter(D *ptr, Dtr dtr) : m_pCounter(NULL)
    {
        m_pCounter = ::new BRefCounter<D>(ptr, dtr);
        assert(NULL != m_pCounter);
    }

    template <class D>
    explicit BStrongCounter(std::auto_ptr<D> &autoPtr)
    {
        m_pCounter = ::new BRefCounter<T>(autoPtr.get());
        assert(NULL != m_pCounter);
        autoPtr.release();
    }

    BStrongCounter(CONST BStrongCounter &r) : m_pCounter(r.m_pCounter)
    {
        if(NULL != m_pCounter)
        {
            m_pCounter->AddStrongRef();
        }
    }

    explicit BStrongCounter(CONST BWeakCounter &r);

    ~BStrongCounter()
    {
        if(NULL != m_pCounter)
        {
            m_pCounter->DelStrongRef();
        }
    }

    BStrongCounter (BStrongCounter &&r) : m_pCounter(r.m_pCounter)
    {
        r.m_pCounter = NULL;
    }

    BStrongCounter &operator=(CONST BStrongCounter &rhs)
    {
        BRefCounterBase *pTmp = rhs.m_pCounter;

        if(pTmp != m_pCounter)
        { // check if is self
            if(NULL != pTmp) pTmp->AddStrongRef();
            if(NULL != m_pCounter) m_pCounter->DelStrongRef();
            m_pCounter = pTmp;
        }

        return *this;
    }

    VOID Swap(BStrongCounter &r)
    {
        BZ_Swap(m_pCounter, r.m_pCounter);
    }

    LONG GetStrongRefCounter() CONST
    {
        return m_pCounter != NULL ? m_pCounter->GetStrongRefCounter() : 0;
    }

    LONG GetWeakRefCounter() CONST
    {
        return m_pCounter != NULL ? m_pCounter->GetWeakRefCounter() : 0;
    }

    BOOL IsUnique() CONST
    {
        return GetStrongRefCounter() == 1;
    }

    BOOL IsEmpty() CONST
    {
        return m_pCounter == NULL;
    }

    friend inline bool operator==(CONST BStrongCounter &lhs, CONST BStrongCounter &rhs)
    {
        return lhs.m_pCounter == rhs.m_pCounter;
    }

    friend inline bool operator!=(const BStrongCounter &lhs, const BStrongCounter &rhs)
    {
        return lhs.m_pCounter != rhs.m_pCounter;
    }
};

class BWeakCounter
{
private:
    friend class BStrongCounter;

private:
    BRefCounterBase  *m_pCounter;

public:
    BWeakCounter() : m_pCounter(NULL)
    {
    }

    BWeakCounter(CONST BWeakCounter &r) : m_pCounter(r.m_pCounter)
    {
        if(NULL != m_pCounter)
        {
            m_pCounter->AddWeakRef();
        }
    }

    BWeakCounter(CONST BStrongCounter &r) : m_pCounter(r.m_pCounter)
    {
        if(NULL != m_pCounter)
        {
            m_pCounter->AddWeakRef();
        }
    }

    BWeakCounter(BWeakCounter &&r) : m_pCounter(r.m_pCounter)
    {
        m_pCounter = NULL;
    }

    BWeakCounter &operator=(CONST BWeakCounter &rhs)
    {
        BRefCounterBase *pTmp = rhs.m_pCounter;
        if(NULL != pTmp) pTmp->AddWeakRef();
        if(NULL != m_pCounter) m_pCounter->DelWeakRef();
        m_pCounter = pTmp;
        return *this;
    }

    BWeakCounter &operator=(CONST BStrongCounter &rhs)
    {
        BRefCounterBase *pTmp = rhs.m_pCounter;
        if(NULL != pTmp) pTmp->AddWeakRef();
        if(NULL != m_pCounter) m_pCounter->DelWeakRef();
        m_pCounter = pTmp;
        return *this;
    }

    ~BWeakCounter()
    {
        if(NULL != m_pCounter)
        {
            m_pCounter->DelWeakRef();
        }
    }

    VOID Swap(BWeakCounter &r)
    {   
        BZ_Swap(m_pCounter, r.m_pCounter);
    }

    LONG GetStrongRefCounter() CONST
    {
        return m_pCounter != 0 ? m_pCounter->GetStrongRefCounter() : 0;
    }

    LONG GetWeakRefCounter() CONST
    {
        return m_pCounter != 0 ? m_pCounter->GetWeakRefCounter() : 0;
    }

    BOOL IsEmpty() CONST
    {
        return m_pCounter == 0;
    }

    friend inline bool operator==(CONST BWeakCounter &lhs, CONST BWeakCounter &rhs)
    {
        return lhs.m_pCounter == rhs.m_pCounter;
    }

    friend inline bool operator!=(CONST BWeakCounter &lhs, CONST BWeakCounter &rhs)
    {
        return lhs.m_pCounter != rhs.m_pCounter;
    }
};

inline BStrongCounter::BStrongCounter(CONST BWeakCounter &r) : m_pCounter(r.m_pCounter)
{
    if (NULL != m_pCounter && 0 != m_pCounter->GetStrongRefCounter())
    { // TODO : here is not thread safe
        m_pCounter->AddStrongRef();
    }
    else
        m_pCounter = NULL;
}

BZ_DECLARE_NAMESPACE_END

#endif