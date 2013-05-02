#include "Log/LogService.h"
#include "Structure/String.h"
#include "Console/ConsoleSimulationWindow.h"
#include "ClientAgency.h"

#pragma warning(disable:4996)

namespace kserver_gs
{
    KClientAgency::KClientAgency()
    {
    }

    KClientAgency::~KClientAgency()
    {
    }

    BOOL KClientAgency::OnClientConnected(BSPAsyncSocketStream &spAsyncSocketStream)
    {
        CHAR szConsoleLogRecord[80];
        BZ_ZeroMemory(szConsoleLogRecord, 80);

        CONST PCHAR cszLogString = "[INFO] Accept a new connection from: ip - %s, port - %d.";
        sprintf(szConsoleLogRecord, cszLogString, spAsyncSocketStream->GetRemoteIp().c_str(), spAsyncSocketStream->GetRemotePort());
/*        BZ_WriteConsoleLog("ConsoleLogRuntimeInfo", szConsoleLogRecord, K_CONSOLE_COLOR_YELLOW, TRUE);*/
        printf(szConsoleLogRecord);
        printf("\n");
        BZ_WriteFileLogByFileName("All", szConsoleLogRecord, TRUE);
        return TRUE;
    }

    BOOL KClientAgency::OnRecvdClientData(BSPAsyncSocketStream &spAsyncSocketStream, BSPDynamicBuffer &spBuffer)
    {
        CHAR szConsoleLogRecord[80];
        BZ_ZeroMemory(szConsoleLogRecord, 80);

        CONST PCHAR cszLogString = "[INFO] Receive a client data from: ip - %s, port - %d.";
        sprintf(szConsoleLogRecord, cszLogString, spAsyncSocketStream->GetRemoteIp().c_str(), spAsyncSocketStream->GetRemotePort());
        printf(szConsoleLogRecord);
        printf("\n");
//        BZ_WriteConsoleLog("ConsoleLogRuntimeInfo", szConsoleLogRecord, K_CONSOLE_COLOR_GREEN, TRUE);
        BZ_WriteFileLogByFileName("All", szConsoleLogRecord, TRUE);

        CONST PCHAR cszCutOffLine = "----------------------------[THE DATA FROM CLIENT]------------------------------\n";
        printf(cszCutOffLine);
//        BZ_WriteConsoleLog("ConsoleLogRuntimeInfo", cszCutOffLine, K_CONSOLE_COLOR_RED);
        BZ_WriteFileLogByFileName("All", cszCutOffLine, FALSE);

        BZ_ConvertToHexString((PCHAR)spBuffer->GetDataPtr(), spBuffer->GetUsedSize(), szConsoleLogRecord, 80);
        printf(szConsoleLogRecord);
        printf("\n");
//        BZ_WriteConsoleLog("ConsoleLogRuntimeInfo", szConsoleLogRecord, K_CONSOLE_COLOR_GREEN);
        BZ_WriteFileLogByFileName("All", szConsoleLogRecord, FALSE);
        //changed by lipengfei 2013/04/25
        printf(cszCutOffLine);
//        BZ_WriteConsoleLog("ConsoleLogRuntimeInfo", cszCutOffLine, K_CONSOLE_COLOR_RED);
        BZ_WriteFileLogByFileName("All", cszCutOffLine, FALSE);
        return TRUE;
    }

    BOOL KClientAgency::OnCloseConnection(BSPAsyncSocketStream &spAsyncSocketStream)
    {
        CHAR szConsoleLogRecord[80];
        BZ_ZeroMemory(szConsoleLogRecord, 80);

        CONST PCHAR cszLogString = "[INFO] Close a established connection from: ip - %s, port - %d.";
        sprintf(szConsoleLogRecord, cszLogString, spAsyncSocketStream->GetRemoteIp().c_str(), spAsyncSocketStream->GetRemotePort());
/*        BZ_WriteConsoleLog("ConsoleLogRuntimeInfo", szConsoleLogRecord, K_CONSOLE_COLOR_YELLOW, TRUE);*/
        printf(szConsoleLogRecord);
        printf("\n");
        BZ_WriteFileLogByFileName("All", szConsoleLogRecord, TRUE);

        spAsyncSocketStream->Close();
        return TRUE;
    }
}