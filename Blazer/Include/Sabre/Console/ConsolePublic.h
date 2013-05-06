#ifndef __BLAZER_SABRE_CONSOLEPUBLIC_H__
#define __BLAZER_SABRE_CONSOLEPUBLIC_H__

#include "Public.h"
#include "SmartPointer/SharedPtr.h"
#include "Structure/UniversalManager.h"
#include "DesignPattern/Singleton.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

/*---------------------------------------------------------------------------------*/
/* Color constants.                                                                */
/*---------------------------------------------------------------------------------*/
CONST BYTE K_CONSOLE_COLOR_DEFAULT   = 0x00;                                                                // default
CONST BYTE K_CONSOLE_COLOR_BLUE      = 0x01;                                                                // blue
CONST BYTE K_CONSOLE_COLOR_GREEN     = 0x02;                                                                // green
CONST BYTE K_CONSOLE_COLOR_RED       = 0x04;                                                                // red
CONST BYTE K_CONSOLE_COLOR_INTENSITY = 0x08;                                                                // intensity
CONST BYTE K_CONSOLE_COLOR_YELLOW    = K_CONSOLE_COLOR_RED  | K_CONSOLE_COLOR_GREEN;                        // yellow 
CONST BYTE K_CONSOLE_COLOR_PURPLE    = K_CONSOLE_COLOR_RED  | K_CONSOLE_COLOR_BLUE;                         // purple
CONST BYTE K_CONSOLE_COLOR_CYAN      = K_CONSOLE_COLOR_BLUE | K_CONSOLE_COLOR_GREEN;                        // cyan
CONST BYTE K_CONSOLE_COLOR_WHITE     = K_CONSOLE_COLOR_RED  | K_CONSOLE_COLOR_GREEN | K_CONSOLE_COLOR_BLUE; // white

CONST INT DEFAULT_MAX_CONSLE_LINE_LEN        = 80;
CONST INT DEFAULT_MAX_CONSLE_LINE_BUFFER_LEN = DEFAULT_MAX_CONSLE_LINE_LEN + 1;

CONST CHAR * CONST K_DEFAULT_CONSOLE_WINDOW_TITLE = "Console Simulation Window Title";

inline CONST BYTE BZ_MakeConsoleIntensityColor(IN CONST BYTE bColor)
{
    return (bColor | K_CONSOLE_COLOR_INTENSITY);
}

inline CONST WORD BZ_MakeConsoleBGColor(IN CONST BYTE bColor)
{
    return (bColor << 4);
}

inline CONST WORD BZ_MakeConsoleFGColor(IN CONST BYTE bColor)
{
    return static_cast<CONST WORD>(bColor);
}

inline CONST WORD BZ_MakeConsoleColor(IN CONST BYTE bBGColor, IN CONST BYTE bFGColor)
{
    return BZ_MakeConsoleBGColor(bBGColor) | bFGColor;
}

inline CONST WORD BZ_MakeConsoleColor(IN CONST WORD wBGColor, IN CONST WORD wFGColor)
{
    return wBGColor | wFGColor;
}

inline COORD BZ_MakeCoord(IN CONST SHORT x, IN CONST SHORT y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    return coord;
}

#define K_CONSOLE_COLOR_STYLE_DEFAULT \
    BZ_MakeConsoleColor( \
    K_CONSOLE_COLOR_DEFAULT, \
    K_CONSOLE_COLOR_WHITE \
    )

#define K_CONSOLE_COLOR_STYLE_RED_ON_WHITE \
    BZ_MakeConsoleColor( \
    (BYTE)(K_CONSOLE_COLOR_INTENSITY | K_CONSOLE_COLOR_RED | K_CONSOLE_COLOR_GREEN | K_CONSOLE_COLOR_BLUE), \
    (BYTE)(K_CONSOLE_COLOR_INTENSITY | K_CONSOLE_COLOR_RED) \
    )

class BConsoleSetting : private BUnCopyable
{
    DECLARE_SINGLETON_PATTERN(BConsoleSetting);
private:
    HANDLE  m_hStdOutput;                           // the console handle

public:
    BConsoleSetting();
    ~BConsoleSetting();

public:
    BOOL SetConsoleProgramIcon(WORD wIconID);
    BOOL SetConsoleTitle(CONST PCHAR cszTitle);

    BOOL SetConsoleCursorPos(CONST COORD &coord);
    BOOL SetConsoleCursorPos(CONST SHORT x, CONST SHORT y);
    BOOL SetConsoleCursorVisible(BOOL bIsVisible);

    BOOL SetConsoleTextColor(CONST WORD wColor);
    BOOL SetConsoleTextColor(CONST BYTE bBGColor, CONST BYTE bFGColor);
    BOOL SetConsoleTextColor(CONST WORD wBGColor, CONST WORD wFGColor);
};

//template class COMMON_API KSingleton<KConsoleSetting>;

enum BNumberForm
{
    BIN, DEC, OCT, HEX,
};

template <class T>
inline void BZ_PrintNumber(T &n, BNumberForm form)
{
    switch(form)
    {
    case BIN:
        {
            char szBinary[32];
            ::itoa(n, szBinary, 2);
            printf("%s", szBinary);
        }
        break;
    case DEC:
        printf("%d", n);
        break;
    case OCT:
        printf("%o", n);
        break;
    case HEX:
        printf("%X", n);
    default:
        assert(FALSE);
    }
}

template <class T>
inline void BZ_PrintNumArrayInHex(T *pArray, size_t nArrayLen)
{
    assert(NULL != pArray && nArrayLen > 0);
    for (size_t i = 0; i < nArrayLen; i++)
        printf("%X", pArray[i]);
}

BZ_DECLARE_NAMESPACE_END

#endif