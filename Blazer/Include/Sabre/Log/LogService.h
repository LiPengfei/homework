#ifndef __BLAZER_SABRE_LOGSERVICE_H__
#define __BLAZER_SABRE_LOGSERVICE_H__

#include "File/File.h"
#include "Log/LogPublic.h"
#include "Log/LogManager.h"
#include "Tool/Application.h"
#include "DesignPattern/Singleton.h"
#include "File/IniFile.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BLogService                                                                   */
/*-------------------------------------------------------------------------------------------*/
class BLogService : public BService
{
    DECLARE_SINGLETON_PATTERN(BLogService);

private:
    BLogManager m_logManager;                   // log manager

    BOOL        m_bIsNetLogOpenedFlag;          // Dynamically control whether net     log opened or not
    BOOL        m_bIsFileLogOpenedFlag;         // Dynamically control whether file    log opened or not
    BOOL        m_bIsConsoleLogOpenedFlag;      // Dynamically control whether console log opened or not


public:
    BLogService();
    virtual ~BLogService();

public:
    virtual BOOL Init();
    virtual BOOL UnInit();
    virtual BOOL Run();

    BOOL GetNetLogOpenedFlag()     CONST;
    BOOL GetFileLogOpenedFlag()    CONST;
    BOOL GetConsoleLogOpenedFlag() CONST;

    void SetNetLogOpenedFlag    (IN CONST BOOL bIsNetLogOpenedFlag     = FALSE);
    void SetFileLogOpenedFlag   (IN CONST BOOL bIsFileLogOpenedFlag    = FALSE);
    void SetConsoleLogOpenedFlag(IN CONST BOOL bIsConsoleLogOpenedFlag = FALSE);

    BOOL OpenLog (IN CONST BYTE byLogType);
    BOOL CloseLog(IN CONST BYTE byLogType);

    void WriteLog(IN CONST BSPLogRecord &spLogRecord);

private:
    BOOL OpenNetLog();
    BOOL OpenFileLog();
    BOOL OpenConsoleLog();

    BOOL CloseNetLog();
    BOOL CloseFileLog();
    BOOL CloseConsoleLog();

};

//template class COMMON_API KSingleton<KLogService>;

typedef       BLogService *       PLogService;
typedef CONST BLogService *       CPLogService;
typedef       BLogService * CONST PCLogService;
typedef CONST BLogService * CONST CPCLogService;

inline void BZ_OpenLog(CONST BYTE byLogType)
{
    PLogService pLogService = BZ_SINGLETON_GET_PTR(BLogService);
    pLogService->OpenLog(byLogType);
}

inline void BZ_CloseLog(CONST BYTE byLogType)
{
    PLogService pLogService = BZ_SINGLETON_GET_PTR(BLogService);
    pLogService->CloseLog(byLogType);
}

inline void BZ_WriteLog(IN CONST BSPLogRecord &spLogRecord)
{
    PLogService pLogService = BZ_SINGLETON_GET_PTR(BLogService);
    pLogService->WriteLog(spLogRecord);
}


inline void BZ_WriteFileLogByFileID(
    IN CONST DWORD dwFileID             ,
    IN const char *const     cpcText              ,
    IN CONST BOOL  bAddTimeStamp = FALSE)
{
    BZ_CHECK_C_STRING_RETURN_VOID(cpcText);

    BLogRecordDetail detail;
    detail.m_byLogRecordType = K_LOG_RECORD_TYPE_FILE;
    detail.m_bIsAddTimestamp = bAddTimeStamp;
    detail.m_auxiliary.m_fileLogAuxiliary.m_dwLogFileID = dwFileID;

//    BSPLogRecord spLogRecord(::new BLogRecord());
//     spLogRecord->SetLogRecordDetail(detail);
//     spLogRecord->SetLogRecordContent(cpcText);

//    BZ_WriteLog(spLogRecord);
}

inline void BZ_WriteFileLogByFileName(
    IN const char *const    cpcFileName          ,
    IN const char *const    cpcText              ,
    IN CONST BOOL bAddTimeStamp = FALSE)
{
    BZ_CHECK_C_STRING_RETURN_VOID(cpcText);
    BZ_CHECK_C_STRING_RETURN_VOID(cpcFileName);

    //need rewrite

//     KSPFileInfo spFileInfo;
//     PFileInfoManager pFileInfoManager = SINGLETON_GET_PTR(KFileInfoManager);
//     BOOL bRetCode = pFileInfoManager->GetFileInfo(cpcFileName, spFileInfo);
//     CHECK_STATE_RETURN_VOID(bRetCode);
// 
//     CHAR szFilePath[DEFAULT_FILE_PATH_LEN];
//     spFileInfo->GetFilePath(szFilePath, DEFAULT_FILE_PATH_LEN);
//     g_WriteFileLogByFileID(szFilePath, cpcText, bAddTimeStamp);
}



class BLogger : public BService
{
public:
    DECLARE_SINGLETON_PATTERN(BLogger);
    
private:
    BSPLogManager m_spLogManager;
    BSPIniFile    m_spIniFile;

public:
    BLogger();
    virtual ~BLogger();

public:
    virtual BOOL Init();
    virtual BOOL UnInit();
    virtual BOOL Run();

public:
    BOOL SetLogManager(const BSPLogManager &spLogManger);
    BOOL SetLogManager(BLogManager *pLogManager);
    BOOL SetIniFile(const BSPIniFile &spIniFile);
    BOOL SetIniFIle(BIniFile *pIniFile);

public:
    INT StartFileLogger();         // -1 error happened, 0 did not start but no error, 1 start;
    INT StartDbLogger();
    INT StartNetLogger();
    INT StartConsoleLogger();

private:
    INT GetFlag(const char *cpName); // -1 error , value success;
};

BZ_DECLARE_NAMESPACE_END

#endif