#ifndef __BLAZER_SABRE_STRING_H__
#define __BLAZER_SABRE_STRING_H__

#include "Public.h"
#include <string>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

    void   BZ_ConvertToHexString(
        IN    CONST CHAR * CONST cpSrc  ,
        IN    CONST size_t       nSrcLen,
        INOUT       CHAR *       pDst   ,
        IN    CONST size_t       nDstLen
    );

    BOOL   BZ_IsBlankCharacter(
        IN    CONST CHAR c
    );

    std::string BZ_TrimString(
        INOUT CONST std::string &s,
        IN    CONST CHAR    c
    );

    void   BZ_TrimString(
        INOUT CONST PCHAR pString,
        IN    CONST CHAR c
    );

    void   BZ_TrimStringBothEnds(
        INOUT CONST PCHAR pString,
        IN    CONST CHAR c
    );

    void   BZ_TrimStringBlankChar(
        INOUT CONST PCHAR pString
    );

    void   BZ_TrimStringBlankCharBothEnds(
        INOUT CONST PCHAR pString
    );

    void   BZ_PadWithBlanks(
        INOUT CONST PCHAR  pBuffer,
        IN    CONST size_t nBufferLen
    );

BZ_DECLARE_NAMESPACE_END

#endif