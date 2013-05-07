#include "Thread/Thread.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#ifdef WIN32
    BThread::BThread() : m_hThread(0), m_threadID(0)
#else
    BThread::BThread() : m_threadID(0)
#endif
{
}

BThread::~BThread()
{
    ::CloseHandle(m_hThread); //modified by lipengfei 2013/05/07
}

BOOL BThread::Init()
{
    return TRUE;
}

#ifdef WIN32
HANDLE BThread::GetHandle() CONST
{
    return m_hThread;
}
#endif

THREAD_ID BThread::GetThreadID() CONST
{
    return m_threadID;
}

#ifdef WIN32
THREAD_FUNC_RET_TYPE __stdcall BThread::ThreadFunction(void *pValue)
#else
THREAD_FUNC_RET_TYPE BThread::ThreadFunction(void *pValue)
#endif
{
    BThread* pThisThread = (BThread *)pValue;
    return (THREAD_FUNC_RET_TYPE)(pThisThread ? pThisThread->Run() : 0);
}

BOOL BThread::Start()
{
#ifdef WIN32
    if (m_hThread <= 0 || INVALID_HANDLE_VALUE == m_hThread) {
        m_hThread = (HANDLE)::_beginthreadex(0, 0, ThreadFunction, (void *)this, 0, &m_threadID);
    }

    return m_hThread > 0;
#else
    int iRetCode = 0;
    if (m_threadID <= 0) {
        iRetCode = ::pthread_create(&m_threadID, NULL, ThreadFunction, (void *)this);
    }

    return nRetCode == 0;
#endif
}

BOOL BThread::Stop(DWORD dwExitCode)
{
#ifdef WIN32
    BZ_CHECK_RETURN_BOOL(m_hThread > 0 && m_hThread != INVALID_HANDLE_VALUE);
    BOOL bRetCode = ::TerminateThread(m_hThread, dwExitCode);
    if (bRetCode)
    {
        CloseHandle(m_hThread);
        return TRUE;
    }

    return FALSE;
#else
    return 0 == ::pthread_cancel(m_threadID);
#endif
}

BOOL BThread::SuspendThread()
{
    BZ_CHECK_RETURN_BOOL(m_hThread > 0 && m_hThread != INVALID_HANDLE_VALUE);
#ifdef WIN32
    DWORD dwRetCode = ::SuspendThread(m_hThread);
    if (static_cast<DWORD>(-1) == dwRetCode)
    {
        return FALSE;
    }
#else
#endif
    return TRUE;
}

BOOL BThread::ResumeThread()
{
    BZ_CHECK_RETURN_BOOL(m_hThread > 0 && m_hThread != INVALID_HANDLE_VALUE);
#ifdef WIN32
    DWORD dwRetCode = ::ResumeThread(m_hThread);
    if (static_cast<DWORD>(-1) == dwRetCode)
    {
        return FALSE;
    }
#else
#endif
    return TRUE;
}

#ifdef WIN32
BOOL BThread::Wait(DWORD dwTimeOutMillis) CONST
{
    BZ_CHECK_RETURN_BOOL(m_hThread > 0 && m_hThread != INVALID_HANDLE_VALUE);
    DWORD dwResult = ::WaitForSingleObject(m_hThread, dwTimeOutMillis);
    return WAIT_OBJECT_0 == dwResult;
}
#endif

BZ_DECLARE_NAMESPACE_END

