#ifndef __BLAZER_SABRE_THREAD_H__
#define __BLAZER_SABRE_THREAD_H__

#ifdef WIN32
#include <process.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif

#include "Public.h"
#include "Synchronize/ThreadSemaphore.h"
#include "Synchronize/ThreadMutex.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#ifdef WIN32
    typedef UINT THREAD_ID;
typedef UINT THREAD_FUNC_RET_TYPE;
#else
    typedef pthread_t THREAD_ID;
typedef VOID *THREAD_FUNC_RET_TYPE;
#endif

class BThread : private BUnCopyable
{
private:
#ifdef WIN32
    HANDLE m_hThread;
#endif
    THREAD_ID m_threadID;

protected:
    BThread();
    virtual ~BThread();

public:
    virtual BOOL Init();

#ifdef WIN32
    HANDLE GetHandle() CONST;
    BOOL Wait(DWORD dwTimeOutMillis = 0) CONST;
#endif

    THREAD_ID GetThreadID() CONST;
    virtual BOOL Start();
    virtual BOOL Stop(DWORD dwExitCode = 0);
    virtual BOOL SuspendThread();
    virtual BOOL ResumeThread();

private:
#ifdef WIN32
    static THREAD_FUNC_RET_TYPE __stdcall ThreadFunction(VOID *);
#else
    static THREAD_FUNC_RET_TYPE *ThreadFunction(VOID *);
#endif
    virtual UINT Run() = 0;
};

BZ_DECLARE_NAMESPACE_END
#endif

