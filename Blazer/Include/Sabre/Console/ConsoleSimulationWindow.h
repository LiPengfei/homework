#ifndef __BLAZER_SABRE_CONSOLESIMULATIONWINDOW_H__
#define __BLAZER_SABRE_CONSOLESIMULATIONWINDOW_H__

#include "ConsolePublic.h"
#include "Synchronize/ThreadMutex.h"
#include <string>
#include <list>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

/*------------------------------------------------------------------------------------------------------------*/
    /* Class       : BConsoleSimulationWindow                                                                     */
    /* Description :     Separate a logical area of a console window to be a simulation window. This "window" is  */
    /*               just on concept and in the area of "window", we can output the specified-color text to any   */
    /*               coordinate of it. This "window" can just be used in windows platform.                        */
    /*                                                                                                            */
    /*                 +-------------------------------------------------------------------+                      */
    /*                 |   Windows Console                                           _ ¡õ x |                      */
    /*                 +-------------------------------------------------------------------+                      */
    /*                 |                                                                   |                      */
    /*                 |    LeftTop-> +---------------------------------+                  |                      */
    /*                 |              |    Simulation Window Title      |                  |                      */
    /*                 |              +---------------------------------+                  |                      */
    /*                 |              |                                 |                  |                      */
    /*                 |              |         Simulation Area         |                  |                      */
    /*                 |              |                                 |                  |                      */
    /*                 |              +---------------------------------+ <-RightBottom    |                      */
    /*                 |                                                                   |                      */
    /*                 +-------------------------------------------------------------------+                      */
    /*                                                                                                            */
    /*    kcommon::KCriticalSection g_ConsoleCS;                                                                  */
    /*    kcommon::BConsoleSimulationWindow w(g_ConsoleCS);                                                       */
    /*    COORD leftTop = {10, 10}, rightBottom = {30, 30};                                                       */
    /*                                                                                                            */
    /*    w.SetManagementArea(leftTop, rightBottom);                                                              */
    /*    w.Write("1.Red Color   ", 0, 0,                                                                         */
    /*    g_MakeConsoleColor(K_CONSOLE_COLOR_DEFAULT, K_CONSOLE_COLOR_INTENSITY | K_CONSOLE_COLOR_RED)   );       */
    /*    w.Write("2.Yellow Color", 0, 1,                                                                         */
    /*    g_MakeConsoleColor(K_CONSOLE_COLOR_DEFAULT, K_CONSOLE_COLOR_INTENSITY | K_CONSOLE_COLOR_YELLOW));       */
    /*    w.Write("3.Blue Color  ", 0, 2,                                                                         */
    /*    g_MakeConsoleColor(K_CONSOLE_COLOR_DEFAULT, K_CONSOLE_COLOR_INTENSITY | K_CONSOLE_COLOR_BLUE)  );       */
    /*    w.Paint();                                                                                              */
    /*                                                                                                            */
    /*------------------------------------------------------------------------------------------------------------*/
#ifdef WIN32
    
typedef struct BRectangleSize
{
    USHORT m_usWidth;
    USHORT m_usHeight;

    BRectangleSize() : m_usWidth(0), m_usHeight(0)
    {
    }

    BRectangleSize(USHORT usWidth, USHORT usHeight) : m_usWidth(usWidth), m_usHeight(usHeight)
    {
    }
} *PRectangleSize;

typedef struct BColorSectionInfo
{
    DWORD m_dwStartPos;
    DWORD m_dwEndPos;
    WORD  m_wColor;

    BColorSectionInfo() : m_dwStartPos(0), m_dwEndPos(0), m_wColor(K_CONSOLE_COLOR_STYLE_DEFAULT)
    {
    }

    BColorSectionInfo(
        CONST DWORD dwStartPos,
        CONST DWORD dwEndPos,
        CONST WORD  wColor = K_CONSOLE_COLOR_STYLE_DEFAULT) : 
    m_dwStartPos(dwStartPos), m_dwEndPos(dwEndPos), m_wColor(wColor)
    {
    }

} *PColorSectionInfo;
#endif

#ifdef WIN32
typedef struct BConsoleSimulationWindowTitle : private BUnCopyable
{
    std::string m_strWindowTitleText;
    WORD        m_wWindowTitleColor;

    BConsoleSimulationWindowTitle() : m_strWindowTitleText(K_DEFAULT_CONSOLE_WINDOW_TITLE), 
        m_wWindowTitleColor(K_CONSOLE_COLOR_STYLE_DEFAULT)
    {
    }
} *PConsoleSimulationWindowTitle;
#endif

#ifdef WIN32
class BConsoleSimulationWindow : private BUnCopyable
{
private:
    typedef std::list<BColorSectionInfo> BColorSectionInfoList;

private:
    HANDLE                        m_hStdOutput;          // the console handle
    BThreadMutex &                m_cs;                  // the console critical section

    COORD                         m_leftTopCoord;         // the coord of left top of "window"
    COORD                         m_rightBottomCoord;     // the coord of right bottom of "window"
    BRectangleSize                m_managementSize;       // the size of management area

    BColorSectionInfoList         m_colorSectionList;     // the color section list
    PCHAR                         m_pConsoleBuffer;       // the console buffer
    DWORD                         m_dwConsoleBufferSize;  // the console buffer size

    DWORD                         m_dwCurCursorPos;       // the cur position of cursor
    DWORD                         m_dwCurCursorLine;      // the cur line of cursor

    DWORD                         m_dwWindowID;           // the id of the simulation window
    BConsoleSimulationWindowTitle m_windowTitle;          // the title of the simulation window

public:
    explicit BConsoleSimulationWindow(BThreadMutex &cs);
    BConsoleSimulationWindow(HANDLE hStdOutput, BThreadMutex &cs);
    ~BConsoleSimulationWindow();

public:
    VOID Paint();                                                 // paint the "window" in the console
    VOID Clear();                                                 // clear "window" content

    VOID WriteText(
        IN CONST CHAR * CONST cszText,
        IN CONST WORD         wColor = K_CONSOLE_COLOR_STYLE_DEFAULT,
        IN CONST BOOL         bAutoEnter = FALSE
        );

    VOID WriteLine(
        IN CONST CHAR * CONST cszText,
        IN CONST WORD         wColor = K_CONSOLE_COLOR_STYLE_DEFAULT,
        IN CONST BOOL         bAutoEnter = FALSE
        );

    BOOL SetManagementArea(
        IN CONST COORD &leftTopCoord,
        IN CONST COORD &rightBottomCoord
        );

    BRectangleSize GetManagementAreaSize() CONST;

    COORD  GetCurCursorPos() CONST;
    VOID   SetCurCursorPos(IN CONST USHORT x, IN CONST USHORT y);

    BOOL   SetWindowID(IN CONST DWORD dwWindowID);
    DWORD  GetWindowID() CONST;

    BOOL   SetWindowTitle(
        IN CONST std::string &strWindowTitleText,
        IN CONST WORD        wWindowTitleColor = K_CONSOLE_COLOR_STYLE_RED_ON_WHITE
        );

    std::string GetWindowTitle() CONST;

private:
    COORD Local2GlobalCoord(IN CONST COORD &coordLocal)  CONST;
    COORD Global2LocalCoord(IN CONST COORD &coordGlobal) CONST;

    BOOL UpdateColorSectionList(
        IN CONST DWORD dwStartPos,
        IN CONST DWORD dwEndPos,
        IN CONST WORD  wColor
        );

    VOID MergeColorSectionList();

    VOID Write(
        IN CONST CHAR * CONST cszText,
        IN CONST USHORT       x,
        IN CONST USHORT       y,
        IN CONST WORD         wColor = K_CONSOLE_COLOR_STYLE_DEFAULT,
        IN CONST BOOL         bAutoEnter = FALSE
        );
};

typedef BConsoleSimulationWindow *PConsoleSimulationWindow;
typedef BSharedPtr<BConsoleSimulationWindow> BSPConsoleSimulationWindow;
#endif

#ifdef WIN32
class BConsoleSimulationWindowManagement : private BUnCopyable
{
    DECLARE_SINGLETON_PATTERN(BConsoleSimulationWindowManagement);
private:
    typedef std::map<DWORD, BSPConsoleSimulationWindow> BConsoleSimulationWindowMap;

private:
    BConsoleSimulationWindowMap m_simulationWindowMap;

public:
    BConsoleSimulationWindowManagement();
    ~BConsoleSimulationWindowManagement();

public:
    BSPConsoleSimulationWindow GetConsoleSimulationWindow(IN CONST DWORD dwWindowID);

    BOOL AddConsoleSimulationWindow(
        IN CONST DWORD                       dwWindowID,
        IN CONST BSPConsoleSimulationWindow &spConsoleSimulationWindow
        );

    BOOL DelConsoleSimulationWindow(IN CONST DWORD dwWindowID);
};

//template class COMMON_API KSingleton<KConsoleSimulationWindowManagement>;
#endif

#ifdef WIN32
inline BOOL g_RegisterConsoleSimulationWindow(
    IN CONST CHAR * CONST                cszWindowID              ,
    IN BSPConsoleSimulationWindow &spConsoleSimulationWindow)
{
    BZ_CHECK_C_STRING_RETURN_BOOL(cszWindowID);
    BZ_CHECK_RETURN_BOOL(spConsoleSimulationWindow);

    DWORD dwWindowID = BZ_HashString2ID(cszWindowID);
    BOOL bRetCode = FALSE;

    bRetCode = spConsoleSimulationWindow->SetWindowID(dwWindowID);
    BZ_CHECK_RETURN_BOOL(bRetCode);

    BConsoleSimulationWindowManagement *pManagement = BZ_SINGLETON_GET_PTR(BConsoleSimulationWindowManagement);
    BZ_CHECK_RETURN_BOOL(pManagement);

    bRetCode = pManagement->AddConsoleSimulationWindow(dwWindowID, spConsoleSimulationWindow);
    BZ_CHECK_RETURN_BOOL(bRetCode);

    return TRUE;
}

inline BOOL g_UnRegisterConsoleSimulationWindow(IN CONST CHAR * CONST cszWindowID)
{
    BZ_CHECK_C_STRING_RETURN_BOOL(cszWindowID);

    DWORD dwWindowID = BZ_HashString2ID(cszWindowID);
    BOOL bRetCode = FALSE;

    BConsoleSimulationWindowManagement *pManagement = BZ_SINGLETON_GET_PTR(BConsoleSimulationWindowManagement);
    BZ_CHECK_RETURN_BOOL(pManagement);

    bRetCode = pManagement->DelConsoleSimulationWindow(dwWindowID);
    BZ_CHECK_RETURN_BOOL(bRetCode);

    return TRUE;
}

inline BSPConsoleSimulationWindow g_GetConsoleSimulationWindow(IN CONST DWORD dwWindowID)
{
    BSPConsoleSimulationWindow sp;

    BConsoleSimulationWindowManagement *pManagement = BZ_SINGLETON_GET_PTR(BConsoleSimulationWindowManagement);
    BZ_CHECK_RETURN_CODE(pManagement, sp);

    sp = pManagement->GetConsoleSimulationWindow(dwWindowID);
    return sp;
}

inline BSPConsoleSimulationWindow g_GetConsoleSimulationWindow(IN CONST CHAR * CONST cszWindowID)
{
    BSPConsoleSimulationWindow sp;
    BZ_CHECK_C_STRING_RETURN_CODE(cszWindowID, sp);

    DWORD dwWindowID = BZ_HashString2ID(cszWindowID);
    sp = g_GetConsoleSimulationWindow(dwWindowID);
    return sp;
}

inline VOID g_WriteConsoleSimulationWindowLine(
    IN CONST DWORD        dwWindowID                            ,
    IN CONST CHAR * CONST cszText                               ,
    IN CONST WORD         wColor = K_CONSOLE_COLOR_STYLE_DEFAULT)
{
    BZ_CHECK_C_STRING_RETURN_VOID(cszText);

    BSPConsoleSimulationWindow sp = g_GetConsoleSimulationWindow(dwWindowID);
    BZ_CHECK_RETURN_VOID(sp);

    sp->WriteLine(cszText, wColor, FALSE);
    sp->Paint();
}

inline VOID g_WriteConsoleSimulationWindowLine(
    IN CONST CHAR * CONST cszWindowID                           ,
    IN CONST CHAR * CONST cszText                               ,
    IN CONST WORD         wColor = K_CONSOLE_COLOR_STYLE_DEFAULT)
{
    BZ_CHECK_C_STRING_RETURN_VOID(cszText);
    BZ_CHECK_C_STRING_RETURN_VOID(cszWindowID);

    BSPConsoleSimulationWindow sp = g_GetConsoleSimulationWindow(cszWindowID);
    BZ_CHECK_RETURN_VOID(sp);

    sp->WriteLine(cszText, wColor, FALSE);
    sp->Paint();
}
#else
inline VOID g_WriteConsoleSimulationWindowLine(IN CONST CHAR * CONST cszText)
{
    BZ_CHECK_C_STRING_RETURN_VOID(cszText);
    printf("%s\n", cszText);
}
#endif

inline VOID g_SetConsoleSimulationWindowTitle(
    CONST CHAR * CONST cszWindowID                                           ,
    CONST std::string &strWindowTitleText                                    ,
    CONST WORD         wWindowTitleColor = K_CONSOLE_COLOR_STYLE_RED_ON_WHITE)
{
#ifdef WIN32
    BZ_CHECK_C_STRING_RETURN_VOID(cszWindowID);

    BSPConsoleSimulationWindow sp = g_GetConsoleSimulationWindow(cszWindowID);
    BZ_CHECK_RETURN_VOID(sp);

    sp->SetWindowTitle(strWindowTitleText, wWindowTitleColor);
    sp->Paint();
#endif
}

BZ_DECLARE_NAMESPACE_END

#endif