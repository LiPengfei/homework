#ifndef __BZ_SABRE_THREADSYNCHRONIZE_H__
#define __BZ_SABRE_THREADSYNCHRONIZE_H__

#include "Public.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

template <class MutexType>
class BGuard : private BUnCopyable
{
private:
    MutexType &m_mutex;

public:
    explicit BGuard(MutexType &mutex) : m_mutex(mutex) { m_mutex.Lock(); }
    ~BGuard() { m_mutex.Unlock(); }
};

BZ_DECLARE_NAMESPACE_END

#endif

