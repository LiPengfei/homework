// Test.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include "Public/Macro.h"
#include "Engine.h"
#include "Sabre.h"

using namespace BZ(sabre);

int _tmain(int argc, _TCHAR* argv[])
{
    char szBuffer[100];
    BZ_ZeroMemory(szBuffer, 100);

    BFile file;
    BZ_PROCESS_ERROR(file.Open("ReadMe1.txt", "r+") == TRUE);
    BZ_DEBUG_PRINT_BOOL(file.IsOpen());
    BZ_DEBUG_PRINT_INT(file.Size());
    BZ_DEBUG_PRINT_BOOL(file.Flush());
    BZ_DEBUG_PRINT_BOOL(file.IsEof());
    BZ_DEBUG_PRINT_BOOL(file.SeekEnd(0));
    BZ_DEBUG_PRINT_BOOL(file.IsEof());
    BZ_DEBUG_PRINT_INT(file.Read(szBuffer, 100, 1));
    BZ_DEBUG_PRINT_BOOL(file.IsEof());
    BZ_DEBUG_PRINT_BOOL(file.SeekBegin(0));
    //BZ_DEBUG_PRINT_BOOL(file.WriteLine("abc"));

//     while(file.ReadLine(szBuffer, 100)) 
//     {
//         printf("%s", szBuffer);
//         //file.Read(szBuffer, 100, 1);
//         BZ_ZeroMemory(szBuffer, 100);
//     }

    BZ_DEBUG_PRINT_INT(file.Tell());
    BZ_DEBUG_PRINT_INT(file.Write("abc abc", 7, 7));
    
    BZ_DEBUG_PRINT_BOOL(file.Close());

Exit0:
    return 0;
}

