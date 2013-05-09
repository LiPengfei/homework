#include "Console/ConsolePublic.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

BConsoleSetting::BConsoleSetting() : m_hStdOutput(INVALID_HANDLE_VALUE)
{
    m_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    while(NULL == m_hStdOutput || INVALID_HANDLE_VALUE == m_hStdOutput)
    {
        assert(FALSE);
        m_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    }
}

BConsoleSetting::~BConsoleSetting()
{
}

BOOL BConsoleSetting::SetConsoleProgramIcon(WORD wIconID)
{
    HWND hWnd   = ::GetConsoleWindow();
    HICON hIcon = ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(wIconID));

    BZ_CHECK_RETURN_BOOL(NULL != hWnd && NULL != hIcon);
    ::SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    return TRUE;
}

BOOL BConsoleSetting::SetConsoleTitle(CONST PCHAR cszTitle)
{
    BZ_CHECK_C_STRING_RETURN_BOOL(cszTitle);
    return (0 != ::SetConsoleTitleA(cszTitle));
}

BOOL BConsoleSetting::SetConsoleCursorPos(CONST COORD &coord)
{
    return (0 != ::SetConsoleCursorPosition(m_hStdOutput, coord));
}

BOOL BConsoleSetting::SetConsoleCursorPos(CONST SHORT x, CONST SHORT y)
{
    return SetConsoleCursorPos(BZ_MakeCoord(x, y));
}

BOOL BConsoleSetting::SetConsoleCursorVisible(BOOL bIsVisible)
{
    CONSOLE_CURSOR_INFO temp;
    if (::GetConsoleCursorInfo(m_hStdOutput, &temp))
    {
        temp.bVisible = bIsVisible;	
        return (0 != ::SetConsoleCursorInfo(m_hStdOutput, &temp));
    }

    return FALSE;
}

BOOL BConsoleSetting::SetConsoleTextColor(CONST WORD wColor)
{
    return (0 != ::SetConsoleTextAttribute(m_hStdOutput, wColor));
}

BZ_DECLARE_NAMESPACE_END