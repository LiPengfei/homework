#include "Structure/String.h"
#include "SmartPointer/SharedPtr.h"

#pragma warning(disable:4996)

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

typedef std::string STRING;

void   BZ_ConvertToHexString(
    IN    CONST CHAR * CONST cpSrc  ,
    IN    CONST size_t       nSrcLen,
    INOUT       CHAR *       pDst   ,
    IN    CONST size_t       nDstLen)
{
    BZ_CHECK_RETURN_VOID(NULL != cpSrc && nSrcLen > 0);
    BZ_CHECK_RETURN_VOID(NULL != pDst  && nDstLen > nSrcLen * 2);
    BZ_ZeroMemory(pDst, nDstLen);

    CHAR pTemp[3];
    for (size_t i = 0; i < nSrcLen; i++)
    {
        BZ_ZeroMemory(pTemp, 3);
        sprintf(pTemp, "%02X", cpSrc[i]);
        strcat(pDst, pTemp);
    }

    pDst[nDstLen - 1] = '\0';
}

BOOL   BZ_IsBlankCharacter(IN CONST CHAR c)
{
    if (c == ' ' || c == '\t')
        return TRUE;
    return FALSE;
}

STRING BZ_TrimString(INOUT CONST STRING &s, IN CONST CHAR c)
{
    STRING temp = s;
    if (temp.empty())  return temp;

    STRING::size_type i = 0, j = 0, len = temp.size();
    while (i < len)
    {
        if ((j = temp.find_first_of(c, i)) == STRING::npos) break;
        temp.erase(j, 1);
        i = j;
    }

    return temp;
}

void   BZ_TrimString(INOUT CONST PCHAR pString, IN CONST CHAR c)
{
    BZ_CHECK_C_STRING_RETURN_VOID(pString);

    INT nStringLen = ::strlen(pString);
    PCHAR pTemp = (PCHAR)::malloc(nStringLen);
    BZ_ZeroMemory(pTemp, nStringLen);

    for (INT i = 0; i < nStringLen; i++)
    {
        if (pString[i] != c)
            pTemp[i] = pString[i];
    }

    ::memcpy(pString, pTemp, nStringLen);
    BZ_SafelyDeletePtr(pTemp);
}

void   BZ_TrimStringBothEnds(INOUT CONST PCHAR pString, IN CONST CHAR c)
{
    BZ_CHECK_C_STRING_RETURN_VOID(pString);

    INT nStringLen = ::strlen(pString);
    INT nHead = 0, nTail = nStringLen - 1;

    for (INT i = 0; i < nStringLen; i++)
    {
        if (pString[nHead] == c)
            nHead++;

        if (pString[nTail] == c)
            nTail--;

        if (nHead > nTail)
            break;
    }

    if (nHead > nTail || nHead == nStringLen || nTail < 0)
        BZ_ZeroMemory(pString, nStringLen);

    ::memmove(pString, pString + nHead, nTail - nHead + 1);
}

void   BZ_TrimStringBlankChar(INOUT CONST PCHAR pString)
{
    BZ_CHECK_C_STRING_RETURN_VOID(pString);

    INT nStringLen = ::strlen(pString);
    PCHAR pTemp = (PCHAR)::malloc(nStringLen);
    BZ_ZeroMemory(pTemp, nStringLen);

    for (INT i = 0; i < nStringLen; i++)
    {
        if ( !BZ_IsBlankCharacter(pString[i]) )
            pTemp[i] = pString[i];
    }

    ::memcpy(pString, pTemp, nStringLen);
    BZ_SafelyDeletePtr(pTemp);
}

void   BZ_TrimStringBlankCharBothEnds(INOUT CONST PCHAR pString)
{
    BZ_CHECK_C_STRING_RETURN_VOID_QUIET(pString);

    INT nStringLen = ::strlen(pString);
    INT nHead = 0, nTail = nStringLen - 1;

    for (INT i = 0; i < nStringLen; i++)
    {
        if ( BZ_IsBlankCharacter(pString[nHead]) )
            nHead++;

        if ( BZ_IsBlankCharacter(pString[nTail]) )
            nTail--;

        if (nHead > nTail)
            break;
    }

    if (nHead > nTail || nHead == nStringLen || nTail < 0)
        BZ_ZeroMemory(pString, nStringLen);

    nStringLen = nTail - nHead + 1;
    ::memmove(pString, pString + nHead, nStringLen);
    pString[nStringLen] = '\0';
}

void   BZ_PadWithBlanks(INOUT CONST PCHAR pBuffer, IN CONST size_t nBufferLen)
{
    BZ_CHECK_RETURN_VOID(NULL != pBuffer && nBufferLen >= 0);

    size_t i = 0;
    for (; i < nBufferLen - 1; i++)
    {
        if (pBuffer[i] == '\0')
            break;
    }

    for (; i < nBufferLen - 1; i++)
    {
        pBuffer[i] = ' ';
    }

    pBuffer[nBufferLen - 1] = '\0';
}

BZ_DECLARE_NAMESPACE_END
