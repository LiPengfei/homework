#include "Structure/Buffer.h"
#include "Structure/String.h"
#include "Console/ConsolePublic.h"
#include "Console/ConsoleSimulationWindow.h"
#include "Synchronize/ThreadMutex.h"
#include "Synchronize/ThreadSynchronize.h"
BZ_DECLARE_NAMESPACE_BEGIN(sabre)

typedef std::string STRING;

/*------------------------------------------------------------------------------------------------------------*/
/* Class       : BConsoleSimulationWindow                                                                     */
/* Description :     Separate a logical area of a console window to be a simulation window. This "window" is  */
/*               just on concept and in the area of "window", we can output the specified-color text to any   */
/*               coordinate of it.                                                                            */
/*                                                                                                            */
/*                 +-------------------------------------------------------------------+                      */
/*                 |   Windows Console                                           _ ¡õ x |                      */
/*                 +-------------------------------------------------------------------+                      */
/*                 |                                                                   |                      */
/*                 |    LeftTop-> +---------------------------------+                  |                      */
/*                 |              |                                 |                  |                      */
/*                 |              |         Simulation Area         |                  |                      */
/*                 |              |                                 |                  |                      */
/*                 |              +---------------------------------+ <-RightBottom    |                      */
/*                 |                                                                   |                      */
/*                 +-------------------------------------------------------------------+                      */
/*------------------------------------------------------------------------------------------------------------*/
#ifdef WIN32
    BConsoleSimulationWindow::BConsoleSimulationWindow(BThreadMutex &cs) : m_hStdOutput(INVALID_HANDLE_VALUE), m_cs(cs),
    m_pConsoleBuffer(NULL), m_dwCurCursorPos(0), m_dwCurCursorLine(0), m_dwWindowID(0)
{
    m_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    while(NULL == m_hStdOutput || INVALID_HANDLE_VALUE == m_hStdOutput)
    {
        assert(FALSE);
        m_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    }

    Clear();
}

BConsoleSimulationWindow::BConsoleSimulationWindow(HANDLE hStdOutput, BThreadMutex &cs) : m_hStdOutput(hStdOutput), m_cs(cs),
    m_pConsoleBuffer(NULL), m_dwCurCursorPos(0), m_dwCurCursorLine(0), m_dwWindowID(0)
{
    while(NULL == m_hStdOutput || INVALID_HANDLE_VALUE == m_hStdOutput)
    {
        assert(FALSE);
        m_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    }

    Clear();
}

BConsoleSimulationWindow::~BConsoleSimulationWindow()
{
    BZ_SafelyDeletePtr(m_pConsoleBuffer);
}

/*------------------------------------------------------------------------------------------------------------*/
/* Description : The Algorithm of BConsoleSimulationWindow::Paint                                             */
/*               The console buffer(m_pConsoleBuffer) is as big as the management size. Traverse all rows and */
/*               all columns, set the text color by the color section list(m_colorSectionList), and write the */
/*               text to the console.h                                                                        */
/*------------------------------------------------------------------------------------------------------------*/
void BConsoleSimulationWindow::Paint()
{
    BGuard<BThreadMutex> guard(m_cs);                               // the critical section guard
    BOOL bRetCode = FALSE;                                              // the return code
    DWORD dwHasWritten = 0;                                             // the bytes of console buffer that has been written.
    BColorSectionInfoList::iterator iter = m_colorSectionList.begin();  // the iterator of color section list

    bRetCode = ::SetConsoleTextAttribute(m_hStdOutput, iter->m_wColor); // set text color
    BZ_CHECK_RETURN_VOID(bRetCode);

    for (USHORT y = 0; y < m_managementSize.m_usHeight; y++)
    { // traverse all rows
        USHORT x = 0;
        while (x < m_managementSize.m_usWidth)
        { // traverse all columns
            COORD coordLocal;
            coordLocal.X = x;
            coordLocal.Y = y;
            bRetCode = ::SetConsoleCursorPosition(m_hStdOutput, Local2GlobalCoord(coordLocal));
            BZ_CHECK_RETURN_VOID(bRetCode);

            DWORD dwCharsToWrite = 0;                   // the num of chars should be written to console
            DWORD dwNumberOfCharsWritten = 0;           // the num of chars has been written to console
            DWORD dwMaxRowPos = (y + 1) * m_managementSize.m_usWidth - 1;
            if (dwMaxRowPos <= iter->m_dwEndPos)
            { // column finishes but color section not finishes
                dwCharsToWrite = m_managementSize.m_usWidth - x;
                bRetCode = ::WriteConsoleA(m_hStdOutput, m_pConsoleBuffer + dwHasWritten, dwCharsToWrite, &dwNumberOfCharsWritten, NULL);
                assert(bRetCode);
            }
            else
            { // column not finishes but color section finishes
                dwCharsToWrite = iter->m_dwEndPos - dwHasWritten + 1;
                bRetCode = ::WriteConsoleA(m_hStdOutput, m_pConsoleBuffer + dwHasWritten, dwCharsToWrite, &dwNumberOfCharsWritten, NULL);
                assert(bRetCode);

                iter++; // next color section
                bRetCode = ::SetConsoleTextAttribute(m_hStdOutput, iter->m_wColor);
                BZ_CHECK_RETURN_VOID(bRetCode);
            }

            x += static_cast<USHORT>(dwCharsToWrite);
            dwHasWritten += dwCharsToWrite;
        }
    }

    COORD coordLocal;
    coordLocal.X = static_cast<SHORT>(m_dwCurCursorPos % m_managementSize.m_usWidth);
    coordLocal.Y = static_cast<USHORT>(m_dwCurCursorPos / m_managementSize.m_usWidth);
    bRetCode = ::SetConsoleCursorPosition(m_hStdOutput, Local2GlobalCoord(coordLocal));
    BZ_CHECK_RETURN_VOID(bRetCode);
}

void BConsoleSimulationWindow::Clear()
{
    BZ_ZeroMemory(m_pConsoleBuffer, m_dwConsoleBufferSize);
    m_colorSectionList.clear();
    m_colorSectionList.push_back(BColorSectionInfo(0, m_dwConsoleBufferSize - 1, K_CONSOLE_COLOR_STYLE_DEFAULT));
}

void BConsoleSimulationWindow::WriteText(
    IN CONST CHAR * CONST cszText   ,
    IN CONST WORD         wColor    ,
    IN CONST BOOL         bAutoEnter)
{
    BZ_CHECK_C_STRING_RETURN_VOID(cszText);
    BZ_CHECK_RETURN_VOID(m_dwCurCursorPos >= 0 && m_dwCurCursorPos < m_dwConsoleBufferSize);
    BZ_CHECK_RETURN_VOID(m_managementSize.m_usWidth > 0 && m_managementSize.m_usHeight > 1);

    USHORT x = 0, y = 0;
    x = static_cast<USHORT>(m_dwCurCursorPos % m_managementSize.m_usWidth);
    y = static_cast<USHORT>(m_dwCurCursorPos / m_managementSize.m_usWidth);

    if (x >= m_managementSize.m_usWidth || x < 0)
        x = 0;

    if (y >= m_managementSize.m_usHeight || y <= 0)
        y = 1;

    Write(cszText, x, y, wColor, bAutoEnter);
}

void BConsoleSimulationWindow::WriteLine(
    IN CONST CHAR * CONST cszText   ,
    IN CONST WORD         wColor    ,
    IN CONST BOOL         bAutoEnter)
{
    BZ_CHECK_C_STRING_RETURN_VOID(cszText);
    BZ_CHECK_RETURN_VOID(m_dwCurCursorPos >= 0 && m_dwCurCursorPos <= m_dwConsoleBufferSize);
    BZ_CHECK_RETURN_VOID(m_managementSize.m_usWidth > 0 && m_managementSize.m_usHeight > 1);

    CHAR szLine[DEFAULT_MAX_CONSLE_LINE_BUFFER_LEN];
    USHORT x = 0, y = static_cast<USHORT>(m_dwCurCursorLine + 1);

    if (y >= m_managementSize.m_usHeight || y <= 0)
        y = 1;

    BZ_ZeroMemory(szLine, DEFAULT_MAX_CONSLE_LINE_BUFFER_LEN);
    ::memcpy(szLine, cszText, DEFAULT_MAX_CONSLE_LINE_BUFFER_LEN);

    BZ_PadWithBlanks(szLine, DEFAULT_MAX_CONSLE_LINE_BUFFER_LEN);
    Write(szLine, x, y, wColor, bAutoEnter);
}

BOOL BConsoleSimulationWindow::SetManagementArea(
    IN CONST COORD &leftTopCoord    ,
    IN CONST COORD &rightBottomCoord)
{
    BZ_CHECK_RETURN_BOOL(leftTopCoord.X >= 0 && rightBottomCoord.X >= 0);
    BZ_CHECK_RETURN_BOOL(leftTopCoord.X < rightBottomCoord.X && leftTopCoord.Y < rightBottomCoord.Y);

    CONSOLE_SCREEN_BUFFER_INFO ci;
    ::GetConsoleScreenBufferInfo(m_hStdOutput, &ci);

    BZ_CHECK_RETURN_BOOL(rightBottomCoord.X <= ci.dwSize.X && rightBottomCoord.Y <= ci.dwSize.Y);

    m_leftTopCoord     = leftTopCoord;
    m_rightBottomCoord = rightBottomCoord;

    m_managementSize.m_usWidth  = m_rightBottomCoord.X - m_leftTopCoord.X + 1;
    m_managementSize.m_usHeight = m_rightBottomCoord.Y - m_leftTopCoord.Y + 1;
    m_dwConsoleBufferSize = m_managementSize.m_usWidth * m_managementSize.m_usHeight;

    BZ_SafelyDeletePtr(m_pConsoleBuffer);
    m_pConsoleBuffer = ::new char[m_dwConsoleBufferSize];

    Clear();
    return TRUE;
}

BRectangleSize BConsoleSimulationWindow::GetManagementAreaSize() CONST
{
    return m_managementSize;
}

COORD BConsoleSimulationWindow::GetCurCursorPos() CONST
{
    assert(m_managementSize.m_usWidth > 0 && m_managementSize.m_usHeight > 0);

    COORD coord;
    coord.X = static_cast<USHORT>(m_dwCurCursorPos / m_managementSize.m_usWidth);
    coord.Y = static_cast<USHORT>(m_dwCurCursorPos % m_managementSize.m_usWidth);
    return coord;
}

void  BConsoleSimulationWindow::SetCurCursorPos(IN CONST USHORT x, IN CONST USHORT y)
{
    BZ_CHECK_RETURN_VOID(x < m_managementSize.m_usWidth && y < m_managementSize.m_usHeight);
    m_dwCurCursorPos = y * m_managementSize.m_usWidth + y;
}

BOOL  BConsoleSimulationWindow::SetWindowID(IN CONST DWORD dwWindowID)
{
    assert(0 != dwWindowID);
    m_dwWindowID = dwWindowID;
    return TRUE;
}

DWORD BConsoleSimulationWindow::GetWindowID() CONST
{
    return m_dwWindowID;
}

BOOL  BConsoleSimulationWindow::SetWindowTitle(
    IN CONST STRING &strWindowTitleText,
    IN CONST WORD    wWindowTitleColor )
{
    size_t nWindowTitleTextLen = strWindowTitleText.size();
    BZ_CHECK_RETURN_BOOL(nWindowTitleTextLen < m_managementSize.m_usWidth);
    BZ_CHECK_RETURN_BOOL(m_dwCurCursorPos >= 0 && m_dwCurCursorPos < m_dwConsoleBufferSize);
    BZ_CHECK_RETURN_BOOL(m_managementSize.m_usWidth > 0 && m_managementSize.m_usHeight > 1);

    m_windowTitle.m_strWindowTitleText = strWindowTitleText;
    m_windowTitle.m_wWindowTitleColor  = wWindowTitleColor;

    CHAR szConsoleLogRecord[DEFAULT_MAX_CONSLE_LINE_BUFFER_LEN];
    BZ_ZeroMemory(szConsoleLogRecord, DEFAULT_MAX_CONSLE_LINE_BUFFER_LEN);

    ::memcpy(szConsoleLogRecord, strWindowTitleText.c_str(), nWindowTitleTextLen);
    BZ_PadWithBlanks(szConsoleLogRecord, DEFAULT_MAX_CONSLE_LINE_BUFFER_LEN);

    Write(szConsoleLogRecord, 0, 0, wWindowTitleColor, FALSE);
    SetCurCursorPos(0, 1);

    return TRUE;
}

STRING BConsoleSimulationWindow::GetWindowTitle() CONST
{
    return m_windowTitle.m_strWindowTitleText;
}

COORD BConsoleSimulationWindow::Local2GlobalCoord(IN CONST COORD &coordLocal)  CONST
{
    COORD coordGlobal;
    coordGlobal.X = coordLocal.X + m_leftTopCoord.X;
    coordGlobal.Y = coordLocal.Y + m_leftTopCoord.Y;
    return coordGlobal;
}

COORD BConsoleSimulationWindow::Global2LocalCoord(IN CONST COORD &coordGlobal) CONST
{
    assert(coordGlobal.X >= m_leftTopCoord.X);
    assert(coordGlobal.Y >= m_leftTopCoord.Y);

    COORD coordLocal;
    coordLocal.X = coordGlobal.X - m_leftTopCoord.X;
    coordLocal.Y = coordGlobal.Y - m_leftTopCoord.Y;
    return coordLocal;
}

BOOL BConsoleSimulationWindow::UpdateColorSectionList(
    IN CONST DWORD dwStartPos,
    IN CONST DWORD dwEndPos  ,
    IN CONST WORD  wColor    )
{
    BZ_CHECK_RETURN_BOOL(dwStartPos <= dwEndPos && dwStartPos >=0 && dwEndPos < m_dwConsoleBufferSize);
    BColorSectionInfo info(dwStartPos, dwEndPos, wColor);

    BOOL bStartFound = FALSE;
    for (BColorSectionInfoList::iterator iter = m_colorSectionList.begin(); iter != m_colorSectionList.end();)
    {
        if (dwStartPos >= iter->m_dwStartPos && dwStartPos <= iter->m_dwEndPos)
        { // start pos overlaps
            bStartFound = TRUE;

            if (dwStartPos == iter->m_dwStartPos)
                m_colorSectionList.insert(iter, info);
            else
            {
                BColorSectionInfo temp(iter->m_dwStartPos, dwStartPos - 1, iter->m_wColor);
                m_colorSectionList.insert(iter, temp);
                m_colorSectionList.insert(iter, info);
            }
        }

        if (bStartFound)
        {
            if (dwEndPos >= iter->m_dwStartPos && dwEndPos < iter->m_dwEndPos)
            {
                iter->m_dwStartPos = dwEndPos + 1;
                break;
            }
            else if (dwEndPos >= iter->m_dwEndPos)
            {
                iter = m_colorSectionList.erase(iter);
                continue;
            }
        }

        iter++;
    }

    MergeColorSectionList();
    return TRUE;
}

void BConsoleSimulationWindow::MergeColorSectionList()
{
    BColorSectionInfoList::iterator iter = m_colorSectionList.begin();

    if (iter == m_colorSectionList.end())
        return;

    BColorSectionInfoList::iterator iterPrev = iter++;
    for (; iter != m_colorSectionList.end(); iter++)
    {
        if (iterPrev->m_wColor == iter->m_wColor)
        {
            iterPrev->m_dwEndPos = iter->m_dwEndPos;
            m_colorSectionList.erase(iter);
            iter = iterPrev;
        }
        else
            iterPrev = iter;
    }
}

void BConsoleSimulationWindow::Write(
    IN CONST CHAR * CONST cszText   ,
    IN CONST USHORT       x         ,
    IN CONST USHORT       y         ,
    IN CONST WORD         wColor    ,
    IN CONST BOOL         bAutoEnter)
{
    BZ_CHECK_C_STRING_RETURN_VOID(cszText);
    BZ_CHECK_RETURN_VOID(x < m_managementSize.m_usWidth && y < m_managementSize.m_usHeight);

    DWORD dwOffset   = y * m_managementSize.m_usWidth + x;
    DWORD dwStrLen   = static_cast<DWORD>(::strlen(cszText));
    DWORD dwTextSize = bAutoEnter ? dwStrLen : BZ_Min((DWORD)(m_managementSize.m_usWidth - x), dwStrLen);

    if (dwOffset + dwTextSize > m_dwConsoleBufferSize)
        dwTextSize = m_dwConsoleBufferSize - dwOffset;

    ::memcpy(m_pConsoleBuffer + dwOffset, cszText, dwTextSize);

    m_dwCurCursorPos  = dwOffset + dwTextSize;
    m_dwCurCursorLine = y;

    UpdateColorSectionList(dwOffset, dwOffset + dwTextSize - 1, wColor);
}
#endif

#ifdef WIN32
BConsoleSimulationWindowManagement::BConsoleSimulationWindowManagement()
{
}

BConsoleSimulationWindowManagement::~BConsoleSimulationWindowManagement()
{
}

BSPConsoleSimulationWindow BConsoleSimulationWindowManagement::GetConsoleSimulationWindow(IN CONST DWORD dwWindowID)
{
    BConsoleSimulationWindowMap::iterator iter = m_simulationWindowMap.find(dwWindowID);
    if (m_simulationWindowMap.end() != iter)
        return iter->second;
    return BSPConsoleSimulationWindow();
}

BOOL BConsoleSimulationWindowManagement::AddConsoleSimulationWindow(
    IN CONST DWORD                       dwWindowID               ,
    IN CONST BSPConsoleSimulationWindow &spConsoleSimulationWindow)
{
    BZ_CHECK_RETURN_BOOL(spConsoleSimulationWindow);
    m_simulationWindowMap[dwWindowID] = spConsoleSimulationWindow;
    return TRUE;
}

BOOL BConsoleSimulationWindowManagement::DelConsoleSimulationWindow(IN CONST DWORD dwWindowID)
{
    m_simulationWindowMap.erase(dwWindowID);
    return TRUE;
}
#endif

BZ_DECLARE_NAMESPACE_END