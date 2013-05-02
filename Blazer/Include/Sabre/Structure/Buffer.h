#ifndef __BLAZER_SABRE_BUFFER_H__
#define __BLAZER_SABRE_BUFFER_H__

#include "Public.h"
#include "SmartPointer/SharedPtr.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#define    DEFAULT_MAX_BUFFER_LEN 1024
/*-------------------------------------------------------------------------------------------*/
/* CLASS     : KFixedSizeBuffer                                                              */
/*-------------------------------------------------------------------------------------------*/
class BFixedSizeBuffer
{
protected:
    void  *m_pvData;            // ptr to the buffer
    size_t m_nTotalSize;        // total size of the buffer
    size_t m_nUsedSize;         // used size of the buffer

public:
    BFixedSizeBuffer();
    explicit BFixedSizeBuffer(size_t nTotalSize);
    BFixedSizeBuffer(IN void *const cpvData, size_t nTotalSize);
    virtual ~BFixedSizeBuffer();

public:
    void  *GetDataPtr(size_t nPos = 0) const;
    void  *GetLeftPtr() const;

    size_t GetTotalSize() const;
    size_t GetUsedSize()  const;
    size_t GetLeftSize()  const;

    BOOL   InsertDataIntoTail(IN const void *const cpcData, size_t nDataBytes);
    BOOL   InsertDataIntoHead(IN const void *const cpcData, size_t nDataBytes);
    BOOL   InsertDataIntoPos (const size_t nPos, IN const void *const cpcData, size_t nDataBytes);

    void   Reset();
};

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : KDynamicBuffer                                                                */
/*-------------------------------------------------------------------------------------------*/
#define MAX_BUFFER_RESERVE_SIZE 8
class BDynamicBuffer
{
protected:
    void  *m_pvData;            // ptr to the buffer
    size_t m_nTotalSize;        // total size of the buffer
    size_t m_nUsedSize;         // used size of the buffer

public:
    BDynamicBuffer();
    explicit BDynamicBuffer(size_t nTotalSize);
    BDynamicBuffer(IN void *const cpvData, size_t nTotalSize);
    virtual ~BDynamicBuffer();

public:
    void  *GetDataPtr(size_t nPos = 0) const;
    void  *GetLeftPtr() const;

    size_t GetTotalSize() const;
    size_t GetUsedSize()  const;
    size_t GetLeftSize()  const;

    BOOL   InsertDataIntoTail(IN const void *const cpcData, size_t nDataBytes);
    BOOL   InsertDataIntoHead(IN const void *const cpcData, size_t nDataBytes);
    BOOL   InsertDataIntoPos (size_t nPos, IN const void *const cpcData, size_t nDataBytes);

    void Reset();
protected:
    BOOL ReAllocate();
};

typedef       BDynamicBuffer *       PDynamicBuffer;
typedef const BDynamicBuffer *       PCDynamicBuffer;
typedef       BDynamicBuffer * const CPDynamicBuffer;
typedef const BDynamicBuffer * const CPCDynamicBuffer;
typedef BSharedPtr<BDynamicBuffer> BSPDynamicBuffer;

inline void BZ_CopyBufferData(BDynamicBuffer &dst, BDynamicBuffer &src)
{
    dst.Reset();
    dst.InsertDataIntoTail(src.GetDataPtr(), src.GetUsedSize());
}

inline void BZ_CopyBufferData(BDynamicBuffer &dst, BDynamicBuffer &src, const size_t nDataBytes)
{
    dst.Reset();
    dst.InsertDataIntoTail(src.GetDataPtr(), nDataBytes);
}

inline void BZ_CopyBufferData(BSPDynamicBuffer &spDst, BSPDynamicBuffer &spSrc)
{
    spDst->Reset();
    spDst->InsertDataIntoTail(spSrc->GetDataPtr(), spSrc->GetUsedSize());
}

inline void BZ_CopyBufferData(BSPDynamicBuffer &spDst, BSPDynamicBuffer &spSrc, const size_t nDataBytes)
{
    spDst->Reset();
    spDst->InsertDataIntoTail(spSrc->GetDataPtr(), nDataBytes);
}

BZ_DECLARE_NAMESPACE_END

#endif