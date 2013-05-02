#include "Net/SocketStreamBuffer.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

BSocketStreamBuffer::BSocketStreamBuffer() : m_pBuffer(NULL), m_nTotalSize(DEFAULT_MAX_SOCKET_STREAM_BUFFER),
    m_nDataHeadPos(0), m_nDataTailPos(0)
{
    while(NULL == m_pBuffer)
        m_pBuffer = (PCHAR)::malloc(m_nTotalSize);
}

BSocketStreamBuffer::BSocketStreamBuffer(size_t nTotalSize) : m_pBuffer(NULL), m_nTotalSize(nTotalSize),
    m_nDataHeadPos(0), m_nDataTailPos(0)
{
    assert(nTotalSize > 0);
    while(NULL == m_pBuffer)
        m_pBuffer = (PCHAR)::malloc(m_nTotalSize);
}

BSocketStreamBuffer::~BSocketStreamBuffer()
{
    BZ_SafelyDeleteConstArrayPtr(m_pBuffer);
}

size_t BSocketStreamBuffer::GetTotalSize() const
{
    assert(m_nTotalSize > 0);
    return m_nTotalSize;
}

size_t BSocketStreamBuffer::GetUsedSize()  const
{
    assert(m_nTotalSize > 0 && m_nDataTailPos <= m_nTotalSize);
    return m_nDataTailPos;
}

size_t BSocketStreamBuffer::GetLeftSize()  const
{
    assert(m_nTotalSize > 0 && m_nDataTailPos <= m_nTotalSize);
    return m_nTotalSize - GetUsedSize();
}

PCHAR  BSocketStreamBuffer::GetLeftPtr()   const
{
    size_t nUsedSize = GetUsedSize();
    assert(m_nTotalSize > 0 && nUsedSize < m_nTotalSize);
    PCHAR pLeft = static_cast<PCHAR>(m_pBuffer);
    pLeft = pLeft + nUsedSize;
    return pLeft;
}

size_t BSocketStreamBuffer::GetUnReadDataSize() const
{
    assert(m_nTotalSize > 0 && m_nDataTailPos <= m_nTotalSize);
    assert(m_nDataHeadPos <= m_nDataTailPos);
    return m_nDataTailPos - m_nDataHeadPos;
}

BOOL BSocketStreamBuffer::GetPackage(BSPDynamicBuffer &spBuffer)
{
    size_t nPackageSize     = 0;
    size_t nPackageHeadSize = sizeof(WORD);
    size_t nPackageBodySize = 0;
    PCHAR  pPackage = NULL;
    BOOL bRetCode = FALSE;

    if (GetUnReadDataSize() >= nPackageHeadSize)
    {
        pPackage = m_pBuffer + m_nDataHeadPos;
        nPackageBodySize = *(WORD *)(pPackage);
        nPackageSize = nPackageBodySize + nPackageHeadSize;
    }

    if (GetUnReadDataSize() >= nPackageSize)
    { // a complete package reached.
        spBuffer = BSPDynamicBuffer(::new BDynamicBuffer());
        spBuffer->Reset();                      // clear buffer
        PCHAR pData      = pPackage + nPackageHeadSize;
        size_t nDataSize = nPackageSize - nPackageHeadSize;
        spBuffer->InsertDataIntoTail(pData, nDataSize);    // push data
        m_nDataHeadPos += nPackageSize;
        bRetCode = TRUE;
    }

    if (m_nDataTailPos == m_nTotalSize)
    { // reach the end of the buffer
        RemoveReadData();
    }

    return bRetCode;
}

BOOL BSocketStreamBuffer::AddUsedSize(size_t nDataBytes)
{
    assert(m_nTotalSize > 0 && nDataBytes <= GetLeftSize());
    m_nDataTailPos += nDataBytes;
    return TRUE;
}

// add by lipengfei 2013/04/25
BOOL BSocketStreamBuffer::RemovePackage(BSPDynamicBuffer &spBuffer)
{
    size_t nPackageSize     = 0;
    size_t nPackageHeadSize = sizeof(WORD);
    size_t nPackageBodySize = 0;
    PCHAR  pPackage = NULL;
    BOOL bRetCode = FALSE;

    if (GetUnReadDataSize() >= nPackageHeadSize)
    {
        pPackage = m_pBuffer + m_nDataHeadPos;
        nPackageBodySize = *(WORD *)(pPackage);
        nPackageSize = nPackageBodySize + nPackageHeadSize;
    }

    if (GetUnReadDataSize() >= nPackageSize)
    { // a complete package reached.
        spBuffer = BSPDynamicBuffer(::new BDynamicBuffer());
        spBuffer->Reset();                      // clear buffer
        PCHAR pData      = pPackage + nPackageHeadSize;
        size_t nDataSize = nPackageSize - nPackageHeadSize;
        spBuffer->InsertDataIntoTail(pData, nDataSize);    // push data
        m_nDataHeadPos += nPackageSize;
        bRetCode = TRUE;
    }

    RemoveReadData();
    return bRetCode;
}

void BSocketStreamBuffer::RemoveReadData()
{
    size_t nUnReadDataSize = m_nDataTailPos - m_nDataHeadPos;
    if (nUnReadDataSize > 0)
        ::memmove(m_pBuffer, m_pBuffer + m_nDataHeadPos, nUnReadDataSize);
    m_nDataHeadPos = 0;
    m_nDataTailPos = nUnReadDataSize; 
}
// add end

BZ_DECLARE_NAMESPACE_END

