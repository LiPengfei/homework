#ifndef __BLAZER_SABRE_THREADWRITEREADLOCKER_H__
#define __BLAZER_SABRE_THREADWRITEREADLOCKER_H__

/*=================================================================
*ע�⣺
*      windows�¸���ֻ����vista�����ϰ汾��ʹ�ã�ʹ������Ƕ��ʹ�ã�
*      ��Ҫ������KThreadWriteReadLocker t;
*                t.WriteLock();
*                t.WriteLock();
*                t.WriteUnLock();
*                t.WriteUnLock();
*
* ==============================================================*/

#include "Public.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BThreadWriteReadLocker : public BUnCopyable
{
#ifdef _WIN32
private:
    SRWLOCK             m_rw;

public:
    BThreadWriteReadLocker();

#else // for linux
private:
    pthread_rwlock_t    m_rw;

public:
    BThreadWriteReadLocker(const pthread_rwlockattr_t * attr);
#endif

//common
public:
    virtual ~BThreadWriteReadLocker();

public:
    void ReadLock();
    void WriteLock();
    void ReadUnlock();
    void WriteUnlock();

};

BZ_DECLARE_NAMESPACE_END

#endif
