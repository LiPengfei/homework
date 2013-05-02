#include "GameServerPublic.h"
#include "GsLogService.h"

#include "Log/LogManager.h"
#include "File/File.h"
#include "File/IniFile.h"
#include "Console/ConsoleSimulationWindow.h"
using namespace BZ(sabre);

namespace kserver_gs
{
    KGsLogService::KGsLogService()
    {
    }

    KGsLogService::~KGsLogService()
    {
    }

    BOOL KGsLogService::Init()
    {
        BOOL bRetCode = FALSE;

        bRetCode = LoadIniFiles();
        BZ_CHECK_RETURN_BOOL(bRetCode);

        return TRUE;
    }

    BOOL KGsLogService::UnInit()
    {
        return TRUE;
    }

    BOOL KGsLogService::Run()
    {
        return TRUE;
    }

    BOOL KGsLogService::LoadIniFiles()
    {
        BOOL bRetCode = FALSE;

        bRetCode = LoadNetLogIniFile();
        BZ_CHECK_RETURN_BOOL(bRetCode);

        bRetCode = LoadFileLogIniFile();
        BZ_CHECK_RETURN_BOOL(bRetCode);

        bRetCode = LoadConsoleLogIniFile();
        BZ_CHECK_RETURN_BOOL(bRetCode);

        return TRUE;
    }

    BOOL KGsLogService::LoadNetLogIniFile()
    {
        return TRUE;
    }

    BOOL KGsLogService::LoadFileLogIniFile()
    {
//         BIniFile     iniFileLog;
//         KSPSmallFile spSmallFile;
//         STRING       strFilePath;
// 
//         CHAR szFileName[BZ_MAX_FILE_NAME_LEN];
//         CHAR szLogBaseFilePath[BZ_MAX_FILE_PATH_LEN];
//         CHAR szExtensionName[BZ_MAX_FILE_EXT_LEN];
// 
//         BOOL bRetCode = iniFileLog.LoadFile(DEFAULT_FILELOG_INIFILE);
//         BZ_CHECK_RETURN_BOOL(bRetCode);
// 
//         BOOL bIsOpenFlag = 0;
//         bRetCode = iniFileLog.GetInt("Main", "OpenFlag", 0, &bIsOpenFlag);
//         BZ_CHECK_RETURN_BOOL(bRetCode);
// 
//         if (!bIsOpenFlag)
//             return TRUE;
//         BZ_OpenLog(K_LOG_IF_OPEN_TYPE_FILE);
// 
//         BZ_ZeroMemory(szLogBaseFilePath, BZ_MAX_FILE_PATH_LEN);
//         iniFileLog.GetString("Main", "FileLogBasePath", "", szLogBaseFilePath, BZ_MAX_FILE_PATH_LEN);
//         BZ_CHECK_C_STRING_RETURN_BOOL(szLogBaseFilePath);
// 
//         BZ_ZeroMemory(szExtensionName, BZ_MAX_FILE_EXT_LEN);
//         iniFileLog.GetString("Main", "ExtensionName", "", szExtensionName, BZ_MAX_FILE_EXT_LEN);
// 
//         BZ_ZeroMemory(szFileName, BZ_MAX_FILE_NAME_LEN);
//         iniFileLog.GetString("AllLogFile", "FileName", "", szFileName, DEFAULT_FILE_NAME_LEN);
// 
//         strFilePath.clear();
//         strFilePath = strFilePath + szLogBaseFilePath + szFileName + szExtensionName;
//         bRetCode = g_ProduceSmallFile(spSmallFile, strFilePath.c_str());
//         BZ_CHECK_RETURN_BOOL(bRetCode);
// 
//         PSmallFileManager pFileManager = SINGLETON_GET_PTR(KSmallFileManager);
//         bRetCode = pFileManager->Add(spSmallFile->GetFileID(), spSmallFile);
//         BZ_CHECK_RETURN_BOOL(bRetCode);
// 
//         PFileInfoManager pFileInfoManager = SINGLETON_GET_PTR(KFileInfoManager);
//         KSPFileInfo spFileInfo(::new KFileInfo());
//         BZ_CHECK_RETURN_BOOL(spFileInfo);
// 
//         spFileInfo->SetFileBasePath (szLogBaseFilePath);
//         spFileInfo->SetFileName     (szFileName);
//         spFileInfo->SetExtensionName(szExtensionName);
//         spFileInfo->SetFileID       (spSmallFile->GetFileID());
//         spFileInfo->SetLargeFileFlag(FALSE);
// 
//         bRetCode = pFileInfoManager->AddFileInfo(spFileInfo);
         return TRUE;
    }

    BOOL KGsLogService::LoadConsoleLogIniFile()
    {
//#ifdef WIN32
//         BIniFile iniConsoleLog;
//         BOOL bRetCode = FALSE;
// 
//         CHAR szSectionName[256];
//         CHAR szWindowStringID[256];
//         SHORT sLeftTopCoordX = 0, sLeftTopCoordY = 0, sRightBottomCoordX = 0, sRightBottomCoordY = 0;
// 
//         bRetCode = iniConsoleLog.LoadFile(DEFAULT_CONSOLELOG_INIFILE);
//         BZ_CHECK_RETURN_BOOL(bRetCode);
// 
//         BOOL bIsOpenFlag = 0;
//         bRetCode = iniConsoleLog.GetInt("Main", "OpenFlag", 0, &bIsOpenFlag);
//         BZ_CHECK_RETURN_BOOL(bRetCode);
// 
//         if (!bIsOpenFlag)
//             return TRUE;
//         BZ_OpenLog(K_LOG_IF_OPEN_TYPE_CONSOLE);
// 
//         INT nWindowNum = 0;
//         bRetCode = iniConsoleLog.GetInt("Main", "SimulationWindowNum", 0, &nWindowNum);
//         BZ_CHECK_RETURN_BOOL(bRetCode);
// 
//         for (int i = 0; i < nWindowNum; i++)
//         {
//             BZ_ZeroMemory(szSectionName, 256);
//             ::sprintf(szSectionName, "SimulationWindow_%d", i);
// 
//             BZ_ZeroMemory(szWindowStringID, 256);
//             bRetCode = iniConsoleLog.GetString(szSectionName, "WindowStringID", "", szWindowStringID, 256);
//             BZ_CHECK_RETURN_BOOL(bRetCode);
// 
//             bRetCode = iniConsoleLog.GetShort(szSectionName, "LeftTopCoordX", 0, &sLeftTopCoordX);
//             BZ_CHECK_RETURN_BOOL(bRetCode);
// 
//             bRetCode = iniConsoleLog.GetShort(szSectionName, "LeftTopCoordY", 0, &sLeftTopCoordY);
//             BZ_CHECK_RETURN_BOOL(bRetCode);
// 
//             bRetCode = iniConsoleLog.GetShort(szSectionName, "RightBottomCoordX", 0, &sRightBottomCoordX);
//             BZ_CHECK_RETURN_BOOL(bRetCode);
// 
//             bRetCode = iniConsoleLog.GetShort(szSectionName, "RightBottomCoordY", 0, &sRightBottomCoordY);
//             BZ_CHECK_RETURN_BOOL(bRetCode);
// 
//             COORD leftTopCoord, rightBottomCoord;
//             leftTopCoord.X = sLeftTopCoordX; leftTopCoord.Y = sLeftTopCoordY;
//             rightBottomCoord.X = sRightBottomCoordX; rightBottomCoord.Y = sRightBottomCoordY;
// 
//             PConsoleSimulationWindow pConsoleSimulationWindow = ::new BConsoleSimulationWindow(g_ConsoleCS);
//             BZ_CHECK_RETURN_BOOL(NULL != pConsoleSimulationWindow);
// 
//             BSPConsoleSimulationWindow spConsoleSimulationWindow(pConsoleSimulationWindow);
//             spConsoleSimulationWindow->SetManagementArea(leftTopCoord, rightBottomCoord);
//             spConsoleSimulationWindow->SetWindowTitle(szWindowStringID);
//             spConsoleSimulationWindow->Paint();
//             g_RegisterConsoleSimulationWindow(szWindowStringID, spConsoleSimulationWindow);
//         }
// #endif
// 
//         return TRUE;
        return TRUE;
    }
}