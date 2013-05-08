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

// for net log add by lipengfei 13/05/08
const char *const BZ_NET_LOG_KEY_ONE            = "remote_log_server_1";
const char *const BZ_NET_LOG_KEY_TWO            = "remote_log_server_2";

// for file log add by liepngfei 13/05/03
const char *const BZ_FILE_LOG_KEY_WARN          = "file_warning_key";
const char *const BZ_FILE_LOG_KEY_INFO          = "file_infomation_key";
const char *const BZ_FILE_LOG_KEY_ERROR         = "file_error_key";
const char *const BZ_FILE_LOG_KEY_DEBUG         = "file_debug_key";

// for db log add by lipengfei 13/05/05
const char *const BZ_DB_LOG_KEY_WARN            = "db_warning_key";
const char *const BZ_DB_LOG_KEY_INFO            = "db_infomation_key";
const char *const BZ_DB_LOG_KEY_ERROR           = "db_error_key";
const char *const BZ_DB_LOG_KEY_DEBUG           = "db_debug_key";
// string id of log record queue // modified by lipengfei handler'id is the queue's id;
// const char *const K_QUEUE_KEY_LOG_RECORD_NET         = "KLOG_NET_LOG_RECORD_QUEUE";
// const char *const K_QUEUE_KEY_LOG_RECORD_FILE        = "KLOG_FILE_LOG_RECORD_QUEUE";
// const char *const K_QUEUE_KEY_LOG_RECORD_CONSOLE     = "KLOG_CONSOLE_LOG_RECORD_QUEUE";

// string id of log handler
const char *const BZ_STRING_ID_OF_NET_LOG_HANDLER     = "BZ_STRING_ID_OF_NET_LOG_HANDLER";
const char *const BZ_STRING_ID_OF_FILE_LOG_HANDLER    = "BZ_STRING_ID_OF_FILE_LOG_HANDLER";
const char *const BZ_STRING_ID_OF_CONSOLE_LOG_HANDLER = "BZ_STRING_ID_OF_CONSOLE_LOG_HANDLER";
const char *const BZ_STRING_ID_OF_DB_LOG_HANDLER      = "BZ_STRING_ID_OF_DB_LOG_HANDLER"; 
// add by lipengfei 13/05/05

// add by lipengfei 13/05/08
const char* const BZ_LOG_SERVICE_SERVER_CONFIG   = "config_log.ini";
const char* const BZ_LOG_SERVICE_CLIENT_CONFIG   = "config_log.ini";
// add end

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
#define BZ_MAX_PACKAGE_DATA     BZ_MIN_PACKAGE_DATA * 4 * 4
#define BZ_DEFAULT_PACKAGE_DATA BZ_MIN_PACKAGE_DATA * 4

class BPackageHandler
{
private:
    
    #define InitialDataMovedSize (sizeof(int) + sizeof(char) + sizeof(int) + sizeof(int))
    #define DataLenSize          (sizeof(int))
    #define InitialHEADMovedSize (InitialDataMovedSize - DataLenSize)
    #define AddHeadMovedSize     (sizeof(BPackageHead) + InitialHEADMovedSize)
    
public:
    static inline int GetTotalLen(const char *cpData)
    {
        return *(int *)cpData;
    }

    static inline char GetLevel(const char *cpData)
    {
        return *(cpData + sizeof(int));
    }

    static inline int GetDataPosWithoutLen(const char *cpData)
    {
        return *(int *)(cpData + sizeof(int) + sizeof(char));
    }

    static inline int GetDataWithLenPos(const char *cpData)
    {
        return *(int *)(cpData + sizeof(int) + sizeof(char)) - sizeof(int);
    }

    static inline void SetDataLen(char *cpData, int nDataLen)
    {
        *(int *)(cpData + sizeof(int) + sizeof(char) + sizeof(int)) = nDataLen;
    }

    static inline void SetTotalLen(char *cpData, int nLen)
    {
        *(int *)cpData = nLen;
    }

    static inline void SetLevel(char *cpData, char nLevel)
    {
        *(cpData + sizeof(int)) = nLevel;
    }

    static inline void SetPos(char *cpData, int cPos)
    {
        *(int*)(cpData + sizeof(int) + sizeof(char)) = cPos;
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
    static bool AddHead(char * cpData, 
        int &nTotalLen, 
        const BPackageHead &head,
        int nBufLen)
    {
        BZ_CHECK_RETURN_BOOL(nBufLen >= BZ_MIN_PACKAGE_DATA);
        char cPreLevel = GetLevel(cpData);
        int  nPrePos   = GetDataPosWithoutLen(cpData);
        nTotalLen      = GetTotalLen(cpData);
        int nMoved     = nBufLen - AddHeadMovedSize;
        nMoved         = (nTotalLen < nMoved ? nTotalLen : nMoved);
        ::memmove(cpData + AddHeadMovedSize, cpData, nMoved);
        ::memcpy(cpData + InitialHEADMovedSize, &head, sizeof(head));

        bool bRet = true;
        nTotalLen += AddHeadMovedSize;
        if (nTotalLen > nBufLen)
        {
            bRet = false;
            nTotalLen = nBufLen;
        }

        SetTotalLen(cpData, nTotalLen);
        SetLevel(cpData, ++cPreLevel);
        SetPos(cpData, nPrePos + AddHeadMovedSize);

        return bRet;
    }

    static int GetHead(const char *cpData, BPackageHead &head)
    {
        char cLevel = GetLevel(cpData);

        if (0 != cLevel)
        {
            head = *(BPackageHead *)(cpData + InitialHEADMovedSize);
            bool bRetCode = CheckPackageHead(cLevel, head);
            BZ_CHECK_RETURN_CODE(bRetCode, -1);
            return cLevel;
        }

        return 0;
    }

    static bool InitialData(char *cpData, int &nCurLen, int nBufLen)
    {
        BZ_CHECK_RETURN_BOOL(nBufLen >= BZ_MIN_PACKAGE_DATA);
        BZ_CHECK_RETURN_BOOL(nCurLen > 0);
        int nDataLen = nCurLen;
        int nMoved   = nBufLen - InitialDataMovedSize;
        nMoved       = (nCurLen < nMoved ? nCurLen : nMoved);
        ::memmove(cpData + InitialDataMovedSize, cpData, nMoved);

        nCurLen += InitialDataMovedSize;
        bool bRetCode = nCurLen <= nBufLen;

        if (!bRetCode) // nTotalLen > nMax
        {
            nCurLen = nBufLen;
        }

        SetTotalLen(cpData, nCurLen);
        SetLevel(cpData, 0);
        SetPos(cpData, InitialDataMovedSize);
        SetDataLen(cpData, nDataLen);
        return bRetCode;
    }

    static int MoveHead(char *cpData, int nBufLen, BPackageHead &head)
    {
        char cLevel = GetLevel(cpData);

        if (0 != cLevel)
        {
            head = *(BPackageHead *)(cpData + InitialHEADMovedSize);
            int nMoved    = GetTotalLen(cpData);
            int nMaxMoved = nBufLen - AddHeadMovedSize;
            nMoved     = 
                (nMoved < nMaxMoved ? nMoved : nMaxMoved);
            ::memmove(cpData, cpData + AddHeadMovedSize, nMoved);
            bool bRetCode = CheckPackageHead(cLevel, head);
            BZ_CHECK_RETURN_CODE(bRetCode, -1);
            return cLevel;
        }

        return 0;
    }

    static BSimpleString GetData(char *cpData)
    {
        int TotalLen = GetTotalLen(cpData);
        int dataPos  = GetDataWithLenPos(cpData);
        if (TotalLen <= dataPos)
        {
            assert(false);
            return BSimpleString();
        }

        int nDatalen    = *(int *)(cpData + dataPos);
        char *pData     = cpData + dataPos + sizeof(int);
        
        // make data terminal with '\0'
        pData[nDatalen] = '\0';
        return BSimpleString(pData, nDatalen + 1);
    }

private:
    static inline bool CheckPackageHead(int nLevel, const BPackageHead &head)
    {
        if (nLevel > 1 && 0 == head.m_cNetFlag)
        {
            return false;
        }
        return true;
    }
};

BZ_DECLARE_NAMESPACE_END

#endif