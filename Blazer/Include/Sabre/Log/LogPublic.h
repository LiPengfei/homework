#ifndef __BLAZER_SABRE_LOGPUBLIC_H__
#define __BLAZER_SABRE_LOGPUBLIC_H__

#include "Public.h"
#include "Structure/SimpleString.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

// log if open type
CONST BYTE  K_LOG_IF_OPEN_TYPE_NET         = 0x01;
CONST BYTE  K_LOG_IF_OPEN_TYPE_FILE        = 0x02;
CONST BYTE  K_LOG_IF_OPEN_TYPE_CONSOLE     = 0x03;

// log record type : net log, file log, console log
CONST BYTE  K_LOG_RECORD_TYPE_NET          = 0x01;
CONST BYTE  K_LOG_RECORD_TYPE_FILE         = 0x02;
CONST BYTE  K_LOG_RECORD_TYPE_CONSOLE      = 0x04;
CONST BYTE  K_LOG_RECORD_TYPE_DEFAULT      = K_LOG_RECORD_TYPE_CONSOLE;

// log level type : all, error, debug, warn, info, statistics
CONST DWORD K_LOG_RECORD_LEVEL_ALL         = 0x00000000;
CONST DWORD K_LOG_RECORD_LEVEL_ERROR       = 0x00000001;
CONST DWORD K_LOG_RECORD_LEVEL_DEBUG       = 0x00000002;
CONST DWORD K_LOG_RECORD_LEVEL_WARN        = 0x00000004;
CONST DWORD K_LOG_RECORD_LEVEL_INFO        = 0x00000008;
CONST DWORD K_LOG_RECORD_LEVEL_STATISTICS  = 0x00000010;

// for file log add by liepngfei 13/05/03
const char *const BZ_FILE_LOG_KEY_WARN          = "file_warning_key";
const char *const BZ_FILE_LOG_KEY_INFO          = "file_infomation_key";
const char *const BZ_FILE_LOG_KEY_ERROR         = "file_error_key";
const char *const BZ_FILE_LOG_KEY_DEBUG         = "file_debug_key";

// string id of log record queue // modified by lipengfei handler'id is the queue's id;
const char *const K_QUEUE_KEY_LOG_RECORD_NET         = "KLOG_NET_LOG_RECORD_QUEUE";
const char *const K_QUEUE_KEY_LOG_RECORD_FILE        = "KLOG_FILE_LOG_RECORD_QUEUE";
const char *const K_QUEUE_KEY_LOG_RECORD_CONSOLE     = "KLOG_CONSOLE_LOG_RECORD_QUEUE";

// string id of log handler
const char *const K_STRING_ID_OF_NET_LOG_HANDLER     = "K_STRING_ID_OF_NET_LOG_HANDLER";
const char *const K_STRING_ID_OF_FILE_LOG_HANDLER    = "K_STRING_ID_OF_FILE_LOG_HANDLER";
const char *const K_STRING_ID_OF_CONSOLE_LOG_HANDLER = "K_STRING_ID_OF_CONSOLE_LOG_HANDLER";

struct BPackageHead
{
    char  m_cNetFlag;
    char  m_cConsoleFlag;
    char  m_cFileFlag;
    char  m_cDbFlag;
    DWORD m_nNetID;
    DWORD m_nFileID;
    DWORD m_nDbID;

    BPackageHead()
    {
        ::BZ_ZeroMemory(this, sizeof(BPackageHead));
    }

    BPackageHead(char c1, char c2, char c3, char c4, DWORD n1, DWORD n2, DWORD n3)
        :m_cNetFlag(c1), m_cConsoleFlag(c2), m_cFileFlag(c3),
        m_cDbFlag(c4), m_nNetID(n1), m_nFileID(n2), m_nDbID(n3)
    { }
};

#define BZ_MIN_PACKAGE_DATA     256
#define BZ_MAX_PACKAGE_DATA     BZ_MIN_PACKAGE_DATA * 8
#define BZ_DEFAULT_PACKAGE_DATA BZ_MIN_PACKAGE_DATA * 4

class BPackageHandler
{
private:
    
    #define InitialDataMovedSize (sizeof(int) + sizeof(char) + sizeof(int) + sizeof(int))
    #define DataLenSize          (sizeof(int))
    #define InitialHEADMovedSize (InitialDataMovedSize - DataLenSize)
    #define AddHeadMovedSize     (sizeof(BZ(sabre)::BPackageHead) + InitialHEADMovedSize)
    
public:
    static inline int GetTotalLen(char *data)
    {
        return *(int *)data;
    }

    static inline char GetLevel(char *data)
    {
        return *(data + sizeof(int));
    }

    static inline int GetDataPosWithoutLen(char *data)
    {
        return *(int *)(data + sizeof(int) + sizeof(char));
    }

    static inline int GetDataWithLenPos(char *data)
    {
        return *(int *)(data + sizeof(int) + sizeof(char)) - sizeof(int);
    }

    static inline void SetDataLen(char *data, int nDataLen)
    {
        *(int *)(data + sizeof(int) + sizeof(char) + sizeof(int)) = nDataLen;
    }

    static inline void SetTotalLen(char *data, int nLen)
    {
        *(int *)data = nLen;
    }

    static inline void SetLevel(char *data, char nLevel)
    {
        *(data + sizeof(int)) = nLevel;
    }

    static inline void SetPos(char *data, int cPos)
    {
        *(int*)(data + sizeof(int) + sizeof(char)) = cPos;
    }

    /****************************************************************************/
    /* Package Format :
    -------------------------------------------------------------------------
    |    totalLen:int    |    level:char    |    dataPos:int                |
    |-----------------------------------------------------------------------|
    |    Head: {netFlag:char, consoleFlag:char, fileFlag:char,  dbFlag:char,|     
    |           netID :DWORD,                   fileID :DWORD,  dbID: DWORD}| 
    |-----------------------------------------------------------------------|
    |              any num of extdata as front                              |
    |-----------------------------------------------------------------------|
    |   totalLen:int   |   level:char   |   dataPos:int   |    datalen:int  |      
    |-----------------------------------------------------------------------|
    |                         the real data                                 |
    -------------------------------------------------------------------------
    /****************************************************************************/
    static bool AddHead(char * data, 
        int &nTotalLen, 
        const BPackageHead &head,
        int nMax)
    {
        BZ_CHECK_RETURN_BOOL(nMax >= BZ_MIN_PACKAGE_DATA);
        char cPreLevel = GetLevel(data);
        int  nPrePos   = GetDataPosWithoutLen(data);
        nTotalLen      = GetTotalLen(data);
        int nMoved     = nMax - AddHeadMovedSize;
        nMoved         = (nTotalLen < nMoved ? nTotalLen : nMoved);
        ::memmove(data + AddHeadMovedSize, data, nMoved);
        ::memcpy(data + InitialHEADMovedSize, &head, sizeof(head));

        bool bRet = true;
        nTotalLen += AddHeadMovedSize;
        if (nTotalLen > nMax)
        {
            bRet = false;
            nTotalLen = nMax;
        }

        SetTotalLen(data, nTotalLen);
        SetLevel(data, ++cPreLevel);
        SetPos(data, nPrePos + AddHeadMovedSize);

        return bRet;
    }

    static int GetHead(char *data, BPackageHead &head)
    {
        char cLevel = GetLevel(data);

        if (0 != cLevel)
        {
            head = *(BPackageHead *)(data + InitialHEADMovedSize);
            bool bRetCode = CheckPackageHead(cLevel, head);
            BZ_CHECK_RETURN_CODE(bRetCode, -1);
            return cLevel;
        }

        return 0;
    }

    static bool InitialData(char *data, int &nCurLen, int nMax)
    {
        BZ_CHECK_RETURN_BOOL(nMax >= BZ_MIN_PACKAGE_DATA);
        BZ_CHECK_RETURN_BOOL(nCurLen > 0);
        int nDataLen = nCurLen;
        int nMoved   = nMax - InitialDataMovedSize;
        nMoved       = (nCurLen < nMoved ? nCurLen : nMoved);
        ::memmove(data + InitialDataMovedSize, data, nMoved);

        nCurLen += InitialDataMovedSize;
        bool bRetCode = nCurLen <= nMax;

        if (!bRetCode) // nTotalLen > nMax
        {
            nCurLen = nMax;
        }

        SetTotalLen(data, nCurLen);
        SetLevel(data, 0);
        SetPos(data, InitialDataMovedSize);
        SetDataLen(data, nDataLen);
        return bRetCode;
    }

    static int MoveHead(char *data, int nMax, BPackageHead &head)
    {
        char cLevel = GetLevel(data);

        if (0 != cLevel)
        {
            head = *(BPackageHead *)(data + InitialHEADMovedSize);
            int nMoved    = GetTotalLen(data);
            int nMaxMoved = nMax - AddHeadMovedSize;
            nMoved     = 
                (nMoved < nMaxMoved ? nMoved : nMaxMoved);
            ::memmove(data, data + AddHeadMovedSize, nMoved);
            bool bRetCode = CheckPackageHead(cLevel, head);
            BZ_CHECK_RETURN_CODE(bRetCode, -1);
            return cLevel - 1;
        }

        return 0;
    }

    static BSimpleString GetData(char *data)
    {
        int TotalLen = GetTotalLen(data);
        int dataPos  = GetDataWithLenPos(data);
        if (TotalLen <= dataPos)
        {
            assert(false);
            return BSimpleString();
        }

        int nDatalen  = *(int *)(data + dataPos);
        char *pData  = data + dataPos + sizeof(int);

        return BSimpleString(pData, nDatalen);
    }

private:
    static inline bool CheckPackageHead(int level, const BPackageHead &head)
    {
        if (level > 1 && 0 == head.m_cNetFlag)
        {
            return false;
        }
        return true;
    }
};

BZ_DECLARE_NAMESPACE_END

#endif