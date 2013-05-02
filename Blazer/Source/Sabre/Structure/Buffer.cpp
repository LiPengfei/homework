#include "Structure/Buffer.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

    /*-------------------------------------------------------------------------------------------*/
    /* CLASS     : KFixedSizeBuffer                                                              */
    /*-------------------------------------------------------------------------------------------*/
BFixedSizeBuffer::BFixedSizeBuffer() : m_pvData(NULL),
    m_nTotalSize(DEFAULT_MAX_BUFFER_LEN),
    m_nUsedSize(0)
{
    while(NULL == m_pvData)
        m_pvData = ::malloc(m_nTotalSize);
}

BFixedSizeBuffer::BFixedSizeBuffer(size_t nTotalSize) : 
    m_pvData(NULL),
    m_nUsedSize(0)
{
    m_nTotalSize = nTotalSize > 0 ? nTotalSize : DEFAULT_MAX_BUFFER_LEN;
    while(NULL == m_pvData)
        m_pvData = ::malloc(m_nTotalSize);
}

BFixedSizeBuffer::BFixedSizeBuffer(IN void * cpvData, size_t nTotalSize) :
    m_pvData(cpvData), 
    m_nTotalSize(nTotalSize),
    m_nUsedSize(0)
{
    if (NULL == m_pvData || m_nTotalSize <=0)
    {
        BZ_FreePtr(m_pvData);
        m_nTotalSize = DEFAULT_MAX_BUFFER_LEN;
        while(NULL == m_pvData)
            m_pvData = ::malloc(m_nTotalSize);
    }
}

BFixedSizeBuffer::~BFixedSizeBuffer()
{
    BZ_FreePtr(m_pvData);
    m_nTotalSize = 0;
    m_nUsedSize  = 0;
}

void * BFixedSizeBuffer::GetDataPtr(size_t nPos) const
{
    BZ_CHECK_RETURN_CODE(nPos >= 0 && nPos < m_nUsedSize, NULL);
    char *pPos = static_cast<char *>(m_pvData);
    pPos       = pPos + nPos;
    return static_cast<void *>(pPos);
}

void * BFixedSizeBuffer::GetLeftPtr() const
{
    BZ_CHECK_RETURN_CODE(m_nTotalSize > 0 && m_nUsedSize < m_nTotalSize, NULL);
    PCHAR pLeft = static_cast<PCHAR>(m_pvData);
    pLeft       = pLeft + m_nUsedSize;
    return static_cast<void *>(pLeft);
}

size_t BFixedSizeBuffer::GetTotalSize() const
{
    assert(m_nTotalSize > 0);
    return m_nTotalSize;
}

size_t BFixedSizeBuffer::GetUsedSize() const
{
    assert(m_nTotalSize > 0 && m_nUsedSize <= m_nTotalSize);
    return m_nUsedSize;
}

size_t BFixedSizeBuffer::GetLeftSize() const
{
    assert(m_nTotalSize > 0 && m_nUsedSize <= m_nTotalSize);
    return m_nTotalSize - m_nUsedSize;
}

BOOL BFixedSizeBuffer::InsertDataIntoTail(IN const void *const cpcData, size_t nDataBytes)
{
    BZ_CHECK_RETURN_BOOL(NULL != cpcData  && nDataBytes >= 0);
    BZ_CHECK_RETURN_BOOL(m_nTotalSize > 0 && 
        m_nUsedSize <= m_nTotalSize && m_nUsedSize + nDataBytes <= m_nTotalSize);

    PCHAR pLeft = static_cast<PCHAR>(m_pvData) + m_nUsedSize;
    ::memcpy(pLeft, cpcData, nDataBytes);
    m_nUsedSize = m_nUsedSize + nDataBytes;

    return TRUE;
}

BOOL BFixedSizeBuffer::InsertDataIntoHead(IN const void *const cpcData, size_t nDataBytes)
{
    BZ_CHECK_RETURN_BOOL(NULL != cpcData  && nDataBytes >= 0);
    BZ_CHECK_RETURN_BOOL(m_nTotalSize > 0 && m_nUsedSize <= m_nTotalSize && m_nUsedSize + nDataBytes <= m_nTotalSize);

    const PCHAR pData = static_cast<const PCHAR>(m_pvData);
    ::memmove(pData + nDataBytes, pData, m_nUsedSize);
    ::memcpy(pData, cpcData, nDataBytes);

    m_nUsedSize += nDataBytes;
    return TRUE;
}

BOOL BFixedSizeBuffer::InsertDataIntoPos (size_t nPos, IN const void *const cpcData, size_t nDataBytes)
{
    BZ_CHECK_RETURN_BOOL(NULL != cpcData  && nDataBytes >= 0 && nPos >= 0 && nPos < m_nUsedSize);
    BZ_CHECK_RETURN_BOOL(m_nTotalSize > 0 && m_nUsedSize <= m_nTotalSize && m_nUsedSize + nDataBytes <= m_nTotalSize);

    PCHAR pPos = static_cast<PCHAR>(m_pvData);
    pPos       = pPos + nPos;

    ::memmove(pPos + nDataBytes, pPos, m_nUsedSize);
    ::memcpy(pPos, cpcData, nDataBytes);

    m_nUsedSize += nDataBytes;
    return TRUE;
}

VOID BFixedSizeBuffer::Reset()
{
    BZ_CHECK_RETURN_VOID(NULL != m_pvData && m_nTotalSize > 0);
    ::memset(m_pvData, 0, m_nTotalSize);
    m_nUsedSize = 0;
}

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : KDynamicBuffer                                                                */
/*-------------------------------------------------------------------------------------------*/
BDynamicBuffer::BDynamicBuffer() : m_pvData(NULL),
    m_nTotalSize(DEFAULT_MAX_BUFFER_LEN),
    m_nUsedSize(0)
{
    while(NULL == m_pvData)
        m_pvData = ::malloc(m_nTotalSize);
}

BDynamicBuffer::BDynamicBuffer(size_t nTotalSize) :
    m_pvData(NULL),
    m_nUsedSize(0)
{
    m_nTotalSize = nTotalSize > 0 ? nTotalSize : DEFAULT_MAX_BUFFER_LEN;
    while(NULL == m_pvData)
        m_pvData = ::malloc(m_nTotalSize);
}

BDynamicBuffer::BDynamicBuffer(IN void *const cpvData, size_t nTotalSize) :
    m_pvData(cpvData), 
    m_nTotalSize(nTotalSize),
    m_nUsedSize(0)
{
    if (NULL == m_pvData || m_nTotalSize <=0)
    {
        BZ_FreePtr(m_pvData);
        m_nTotalSize = DEFAULT_MAX_BUFFER_LEN;
        while(NULL == m_pvData)
            m_pvData = ::malloc(m_nTotalSize);
    }
}

BDynamicBuffer::~BDynamicBuffer()
{
    BZ_FreePtr(m_pvData);
    m_nTotalSize = 0;
    m_nUsedSize  = 0;
}

void *BDynamicBuffer::GetDataPtr(IN const size_t nPos) const
{
    BZ_CHECK_RETURN_CODE(nPos >= 0 && nPos < m_nUsedSize, NULL);
    PCHAR pPos = static_cast<PCHAR>(m_pvData);
    pPos       = pPos + nPos;
    return static_cast<void *>(pPos);
}

void * BDynamicBuffer::GetLeftPtr() const
{
    BZ_CHECK_RETURN_CODE(m_nTotalSize > 0 && m_nUsedSize < m_nTotalSize, NULL);
    PCHAR pLeft = static_cast<PCHAR>(m_pvData);
    pLeft       = pLeft + m_nUsedSize;
    return static_cast<void *>(pLeft);
}

size_t BDynamicBuffer::GetTotalSize() const
{
    assert(m_nTotalSize > 0);
    return m_nTotalSize;
}

size_t BDynamicBuffer::GetUsedSize() const
{
    assert(m_nTotalSize > 0 && m_nUsedSize <= m_nTotalSize);
    return m_nUsedSize;
}

size_t BDynamicBuffer::GetLeftSize() const
{
    assert(m_nTotalSize > 0 && m_nUsedSize <= m_nTotalSize);
    return m_nTotalSize - m_nUsedSize;
}

VOID BDynamicBuffer::Reset()
{
    BZ_CHECK_RETURN_VOID(NULL != m_pvData && m_nTotalSize > 0);
    ::memset(m_pvData, 0, m_nTotalSize);
    m_nUsedSize = 0;
}

BOOL BDynamicBuffer::InsertDataIntoTail(IN const void *const cpcData, size_t nDataBytes)
{
    BZ_CHECK_RETURN_BOOL(NULL != cpcData && m_nTotalSize > 0 && m_nUsedSize <= m_nTotalSize);
    while(m_nTotalSize - m_nUsedSize < nDataBytes)
        ReAllocate();

    size_t nUsedSize = m_nUsedSize + nDataBytes;
    while(m_nTotalSize - nUsedSize < DEFAULT_MAX_BUFFER_LEN)
        ReAllocate();

    PCHAR pLeft = static_cast<PCHAR>(m_pvData) + m_nUsedSize;
    ::memcpy(pLeft, cpcData, nDataBytes);
    m_nUsedSize = nUsedSize;

    return TRUE;
}

BOOL BDynamicBuffer::InsertDataIntoHead(IN const void *const cpcData, size_t nDataBytes)
{
    BZ_CHECK_RETURN_BOOL(NULL != cpcData && m_nTotalSize > 0 && m_nUsedSize <= m_nTotalSize);
    while(m_nTotalSize - m_nUsedSize < nDataBytes)
        ReAllocate();

    const PCHAR pData = static_cast<const PCHAR>(m_pvData);
    ::memmove(pData + nDataBytes, pData, m_nUsedSize);
    ::memcpy(pData, cpcData, nDataBytes);

    m_nUsedSize += nDataBytes;
    return TRUE;
}

BOOL BDynamicBuffer::InsertDataIntoPos (size_t nPos, IN const void *const cpcData, size_t nDataBytes)
{
    BZ_CHECK_RETURN_BOOL(nPos >= 0 && nPos < m_nUsedSize);
    BZ_CHECK_RETURN_BOOL(NULL != cpcData && m_nTotalSize > 0 && m_nUsedSize <= m_nTotalSize);

    while(m_nTotalSize - m_nUsedSize < nDataBytes)
        ReAllocate();

    PCHAR pPos = static_cast<PCHAR>(m_pvData);
    pPos       = pPos + nPos;

    ::memmove(pPos + nDataBytes, pPos, m_nUsedSize);
    ::memcpy(pPos, cpcData, nDataBytes);

    m_nUsedSize += nDataBytes;
    return TRUE;
}

BOOL BDynamicBuffer::ReAllocate()
{   // realloc the memory, so m_nTotalSize is changed.
    while (NULL == m_pvData)
        m_pvData = ::malloc(DEFAULT_MAX_BUFFER_LEN);

    void * pTemp = m_pvData;
    m_nTotalSize = m_nTotalSize + DEFAULT_MAX_BUFFER_LEN;
    m_pvData = ::realloc(m_pvData, m_nTotalSize);

    if (NULL == m_pvData)
        m_pvData = pTemp;
    return TRUE;
}

BZ_DECLARE_NAMESPACE_END