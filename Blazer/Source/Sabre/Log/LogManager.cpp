#include "Log/LogManager.h"

#include <algorithm>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

BLogManager::BLogManager()
{
}

BLogManager::~BLogManager()
{
}

BOOL BLogManager::Init()
{
    return TRUE;
}

BOOL BLogManager::NotifyHandler()
{
    for (BLogHandlerVector::iterator it = m_logHandlerVector.begin(); it != m_logHandlerVector.end(); it++)
    {
        (*it)->Dispatch(this);
    }

    return TRUE;
}

BOOL BLogManager::AttachHandler(IN CONST BSPLogHandler &spLogHandler)
{
    BZ_CHECK_RETURN_BOOL(spLogHandler);
    m_logHandlerVector.push_back(spLogHandler);
    return TRUE;
}

BOOL BLogManager::DetachHandler(IN CONST BSPLogHandler &spLogHandler)
{
    BZ_CHECK_RETURN_BOOL(spLogHandler);
    BLogHandlerVector::iterator iter = std::find(m_logHandlerVector.begin(), m_logHandlerVector.end(), spLogHandler);

    if (m_logHandlerVector.end() != iter)
    {
        (*iter)->UnInit();
        m_logHandlerVector.erase(iter);
    }

    return TRUE;
}

BOOL BLogManager::AttachHandler(IN CPLogHandler cpLogHandler)
{
    BZ_CHECK_RETURN_BOOL(cpLogHandler);
    m_logHandlerVector.push_back(BSPLogHandler(cpLogHandler));
    return TRUE;
}

BOOL BLogManager::DetachHandler(IN CPLogHandler cpLogHandler)
{
    BZ_CHECK_RETURN_BOOL(cpLogHandler);
    BLogHandlerVector::iterator iter = std::find(m_logHandlerVector.begin(), m_logHandlerVector.end(), BSPLogHandler(cpLogHandler));

    if (m_logHandlerVector.end() != iter)
    {
        (*iter)->UnInit();
        m_logHandlerVector.erase(iter);
    }

    return TRUE;
}

BOOL BLogManager::DetachHandler(IN CONST DWORD dwLogHandlerID)
{
    BLogHandlerVector::iterator iter = m_logHandlerVector.begin();
    for (; iter != m_logHandlerVector.end();)
    {
        if ((*iter)->GetLogHandlerID() == dwLogHandlerID)
        {
            (*iter)->UnInit();
            iter = m_logHandlerVector.erase(iter);
        }
        else
        {
            iter++;
        }
    }

    return TRUE;
}

BOOL BLogManager::SetLogRecord(IN CONST BSPLogRecord &spLogRecord)
{
    BZ_CHECK_RETURN_BOOL(spLogRecord);
    m_spLogRecord = spLogRecord;
    return TRUE;
}

BSPLogRecord BLogManager::GetLogRecord() CONST
{
    assert(m_spLogRecord);
    return m_spLogRecord;
}

BZ_DECLARE_NAMESPACE_END