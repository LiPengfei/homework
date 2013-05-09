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

CONST INT DEFAULT_MAX_CONSLE_LINE_LEN        = 80;
CONST INT DEFAULT_MAX_CONSLE_LINE_BUFFER_LEN = DEFAULT_MAX_CONSLE_LINE_LEN + 1;

CONST CHAR * CONST K_DEFAULT_CONSOLE_WINDOW_TITLE = "Console Simulation Window Title";

inline COORD BZ_MakeCoord(IN CONST SHORT x, IN CONST SHORT y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    return coord;
}

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