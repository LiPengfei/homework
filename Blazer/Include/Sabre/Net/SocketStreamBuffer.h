#ifndef __BLAZER_SABRE_SOCKETSTREAMBUFFER_H__
#define __BLAZER_SABRE_SOCKETSTREAMBUFFER_H__

#include "Public.h"
#include "Structure/Buffer.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#define DEFAULT_MAX_SOCKET_STREAM_BUFFER    (1024 * 1024)

class BSocketStreamBuffer : private BUnCopyable
{
private:
    char  *m_pBuffer;                   // the real buffer ptr managed by BSocketStreamBuffer.
    size_t m_nTotalSize;                // the total size of m_pBuffer.
    // if m_nDataHeadPos == m_nDataTailPos, it indicates there is no un-read data.
    // if m_nDataTailPos == m_nTotalSize, it indicates the left room of m_pBuffer is used out.
    size_t m_nDataHeadPos;              // the begin pos of un-read data.
    size_t m_nDataTailPos;              // the next pos of un-read data's end pos.

public:
    BSocketStreamBuffer();
    BSocketStreamBuffer(size_t nTotalSize);
    ~BSocketStreamBuffer();

public:
    size_t GetTotalSize() const;        // return the total size of buffer.
    size_t GetUsedSize()  const;        // return the used size of buffer.
    size_t GetLeftSize()  const;        // return the left size of buffer.
    char  *GetLeftPtr()   const;
    size_t GetUnReadDataSize() const;   // return the size of data that is not read by outer function.

    // if return "TRUE", it indicates one complete package is received.
    // if return "FALSE", it indicates no complete package is received.
    // changed by lipengfei, the preview signiture is BOOL AddUsedSize(size_t nDataBytes);
    virtual BOOL GetPackage(BSPDynamicBuffer &spBuffer);       // get one complete package every time.

    BOOL AddUsedSize(size_t nDataBytes);  // add the used size by outer function manually.

    //add by lipengfei 2013/04/25
    virtual BOOL RemovePackage(BSPDynamicBuffer &spBuffer);       // get one complete package every time, then remove the data from buffer.
    
    void RemoveReadData();
    // add end
};

BZ_DECLARE_NAMESPACE_END

#endif

