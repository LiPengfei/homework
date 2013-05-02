#ifndef __BLAZER_SABRE_STRING_H__
#define __BLAZER_SABRE_STRING_H__

#include "Public.h"
#include <string>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

    VOID   BZ_ConvertToHexString(
        IN    CONST CHAR * CONST cpSrc  ,
        IN    CONST size_t       nSrcLen,
        INOUT       CHAR *       pDst   ,
        IN    CONST size_t       nDstLen
    );

    BOOL   g_IsBlankCharacter(
        IN    CONST CHAR c
    );

    std::string g_TrimString(
        INOUT CONST std::string &s,
        IN    CONST CHAR    c
    );

    VOID   g_TrimString(
        INOUT CONST PCHAR pString,
        IN    CONST CHAR c
    );

    VOID   g_TrimStringBothEnds(
        INOUT CONST PCHAR pString,
        IN    CONST CHAR c
    );

    VOID   g_TrimStringBlankChar(
        INOUT CONST PCHAR pString
    );

    VOID   g_TrimStringBlankCharBothEnds(
        INOUT CONST PCHAR pString
    );

    VOID   g_PadWithBlanks(
        INOUT CONST PCHAR  pBuffer,
        IN    CONST size_t nBufferLen
    );

BZ_DECLARE_NAMESPACE_END

#endif