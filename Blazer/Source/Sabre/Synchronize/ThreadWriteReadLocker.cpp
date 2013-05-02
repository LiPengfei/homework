#include "Synchronize/ThreadWriteReadLocker.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#ifdef _WIN32

BThreadWriteReadLocker::BThreadWriteReadLocker()
{
    ::InitializeSRWLock(&m_rw);
}

BThreadWriteReadLocker::~BThreadWriteReadLocker()
{
}

void BThreadWriteReadLocker::ReadLock()
{
    ::AcquireSRWLockShared(&m_rw);
}

void BThreadWriteReadLocker::WriteLock()
{
    ::AcquireSRWLockExclusive(&m_rw);
}

void BThreadWriteReadLocker::ReadUnlock()
{
    ::ReleaseSRWLockShared(&m_rw);
}

void BThreadWriteReadLocker::WriteUnlock()
{
    ::ReleaseSRWLockExclusive(&m_rw);
}
#else
BThreadWriteReadLocker::BThreadWriteReadLocker(const pthread_rwlockattr_t *attr)
{
    pthread_rwlock_init(&m_rw, attr);
}

BThreadWriteReadLocker::~BThreadWriteReadLocker()
{
    pthread_rwlock_destory(&m_rw);
}

void BThreadWriteReadLocker::ReadLock()
{
    pthread_rwlock_rdlock(&m_rw);
}

void BThreadWriteReadLocker::WriteLock()
{
    pthread_rwlock_wrlock(&m_rw);
}

void BThreadWriteReadLocker::ReadUnlock()
{
    pthread_rwlock_unlock(&m_rw);
}

void BThreadWriteReadLocker::WriteUnlock()
{
    pthread_rwlock_unlock(&m_rw);
}
#endif

BZ_DECLARE_NAMESPACE_END

