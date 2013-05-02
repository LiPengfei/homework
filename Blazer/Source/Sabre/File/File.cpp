#include "File/File.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

BOOL BFile::Open(const char *const cpcFile, const char *const cpcMode)
{
    BZ_CHECK_C_STRING_RETURN_BOOL(cpcFile);
    BZ_CHECK_C_STRING_RETURN_BOOL(cpcMode);
    BZ_CHECK_RETURN_BOOL(NULL == m_hFile);          // open a new file, m_hFile must be NULL

    m_hFile = ::fopen(cpcFile, cpcMode);
    BZ_CHECK_RETURN_BOOL(m_hFile);

    if (::strchr(cpcMode, 'b'))
        m_bBinaryFile = TRUE;
    else
        m_bBinaryFile = FALSE;

    return TRUE;
}

BOOL BFile::Close()
{
    int nRetCode = 0;
    if (m_hFile)
    {
        nRetCode = ::fclose(m_hFile);
        BZ_CHECK_RETURN_BOOL(0 == nRetCode);
        m_hFile = NULL;
    }

    return TRUE;
}

LONG BFile::Size() const
{
    int nRetCode = 0;
    LONG lResult = -1;
    LONG lOriginPos = -1;

    BZ_CHECK_RETURN_CODE(m_hFile, -1);

    lOriginPos = ::ftell(m_hFile);
    BZ_CHECK_RETURN_CODE(-1 != lOriginPos, -1);

    nRetCode = ::fseek(m_hFile, 0, SEEK_END);
    BZ_CHECK_RETURN_CODE(0 == nRetCode, -1);

    lResult = ::ftell(m_hFile);
    BZ_CHECK_RETURN_CODE(-1 != lResult, -1);

    nRetCode = ::fseek(m_hFile, lOriginPos, SEEK_SET);
    BZ_CHECK_RETURN_CODE(0 == nRetCode, -1);

    return lResult;
}

BOOL BFile::SeekBegin(LONG lOffset)
{
    int nRetCode = 0;

    BZ_CHECK_RETURN_BOOL(m_hFile);
    BZ_CHECK_RETURN_BOOL(lOffset >= 0);
    BZ_CHECK_RETURN_BOOL(lOffset <= Size());

    nRetCode = ::fseek(m_hFile, lOffset, SEEK_SET);
    BZ_CHECK_RETURN_BOOL(0 == nRetCode);

    return TRUE;
}

BOOL BFile::SeekEnd(LONG lOffset)
{
    int nRetCode = 0;

    BZ_CHECK_RETURN_BOOL(m_hFile);
    BZ_CHECK_RETURN_BOOL(lOffset >= 0);
    BZ_CHECK_RETURN_BOOL(lOffset <= Size());

    nRetCode = ::fseek(m_hFile, -lOffset, SEEK_END);
    BZ_CHECK_RETURN_BOOL(0 == nRetCode);

    return TRUE;
}

BOOL BFile::SeekCurrent(LONG lOffset)
{
    int nRetCode = 0;

    BZ_CHECK_RETURN_BOOL(m_hFile);

    LONG lSize = Size();
    BZ_CHECK_RETURN_BOOL(-1 != lSize);

    LONG lPos = Tell();
    BZ_CHECK_RETURN_BOOL(-1 != lPos);
    BZ_CHECK_RETURN_BOOL(lOffset <= lSize - lPos);

    nRetCode = ::fseek(m_hFile, lOffset, SEEK_CUR);
    BZ_CHECK_RETURN_BOOL(0 == nRetCode);

    return TRUE;
}

LONG BFile::Tell() const
{
    LONG lResult = -1;
    BZ_CHECK_RETURN_CODE(m_hFile, -1);

    lResult = ::ftell(m_hFile);
    BZ_CHECK_RETURN_CODE(-1 != lResult, -1);

    return lResult;
}

LONG BFile::Read(char* const cpBuffer, LONG lBufferSize, LONG lReadSize)
{
    LONG lResult = -1;

    BZ_CHECK_RETURN_CODE(m_hFile, -1);
    BZ_CHECK_RETURN_CODE(lBufferSize > 0, -1);
    BZ_CHECK_RETURN_CODE(lReadSize > 0, -1);
    BZ_CHECK_RETURN_CODE(lReadSize <= lBufferSize, -1);

    // ::fread parameter's order problem [3/9/2013 °×ÔÅ]
    // lResult = ::fread(cpBuffer, lReadSize, 1, m_hFile);
    lResult = ::fread(cpBuffer, 1, lReadSize, m_hFile);
    BZ_CHECK_RETURN_CODE(lResult <= lReadSize, -1);
    BZ_CHECK_RETURN_CODE(::ferror(m_hFile) == 0, -1);

    return lResult;
}

LONG BFile::Write(const char* const cpcBuffer, LONG lBufferSize, LONG lWriteSize)
{
    LONG lResult = -1;

    BZ_CHECK_RETURN_CODE(m_hFile, -1);
    BZ_CHECK_RETURN_CODE(lBufferSize > 0, -1);
    BZ_CHECK_RETURN_CODE(lWriteSize > 0, -1);
    BZ_CHECK_RETURN_CODE(lWriteSize <= lBufferSize, -1);

    // ::fwrite parameter's order problem [3/9/2013 °×ÔÅ]
    // lResult = ::fwrite(cpcBuffer, lWriteSize, 1, m_hFile);
    lResult = ::fwrite(cpcBuffer, 1, lWriteSize, m_hFile);
    BZ_CHECK_RETURN_CODE(lResult != lWriteSize, -1);

    return lResult;
}

BOOL BFile::IsOpen() const
{
    return m_hFile != NULL;
}

BOOL BFile::Flush()
{
    BZ_CHECK_RETURN_BOOL(m_hFile);
    int nRetCode = ::fflush(m_hFile);
    BZ_CHECK_RETURN_BOOL(0 == nRetCode);

    return TRUE;
}

BOOL BFile::IsEof() const
{
    BZ_CHECK_RETURN_BOOL(m_hFile);
    return ::feof(m_hFile) != 0;
}

LONG BFile::ReadTextLine(OUT char *const cpStr, IN LONG lReadLen)
{
    LONG lResult = -1;
    char *pStr = NULL;

    BZ_CHECK_RETURN_CODE(m_hFile, -1);
    BZ_CHECK_RETURN_CODE(!m_bBinaryFile, -1);
    BZ_CHECK_RETURN_CODE(cpStr, -1);
    BZ_CHECK_RETURN_CODE_QUIET(0 != lReadLen, 0);
    BZ_CHECK_RETURN_CODE(lReadLen > 0, -1);

    // gets¶ÁÈ¡»»ÐÐ·û²¢½«Æä¶ªÆú£¬fgets°Ñ»»ÐÐ·û´æ×Ö·û´®Àï¡£ [3/6/2013 T.Q.Y.]
    pStr = ::fgets(cpStr, lReadLen, m_hFile);
    BZ_CHECK_RETURN_CODE(::ferror(m_hFile) != 0, -1);
    BZ_CHECK_RETURN_CODE_QUIET(pStr, 0);

    lResult = ::strnlen(pStr, lReadLen);
    char *pNewLine = &cpStr[lResult - 1];
    if ('\n' == *pNewLine)
    {
        *pNewLine = '\0';
        --lResult;
    }

    return lResult;
}

LONG BFile::WriteTextLine(const char *const cpcStr, LONG lWriteLen)
{
    int nRetCode = 0;

    BZ_CHECK_RETURN_CODE(m_hFile, -1);
    BZ_CHECK_RETURN_CODE(!m_bBinaryFile, -1);
    BZ_CHECK_RETURN_CODE(cpcStr, -1);
    BZ_CHECK_RETURN_CODE_QUIET(cpcStr[0], 0);
    BZ_CHECK_RETURN_CODE_QUIET(0 != lWriteLen, 0);
    BZ_CHECK_RETURN_CODE(lWriteLen > 0, -1);

    LONG lStrLen = static_cast<LONG>(::strnlen(cpcStr, lWriteLen));
    BZ_CHECK_RETURN_CODE(lStrLen >= lWriteLen, -1);

    nRetCode = ::fwrite(cpcStr, 1, lWriteLen, m_hFile);
    BZ_CHECK_RETURN_CODE(-1 != nRetCode, -1);

    ::fputc('\n', m_hFile);
    BZ_CHECK_RETURN_CODE(::ferror(m_hFile) != 0, -1);

    return lWriteLen;
}

BZ_DECLARE_NAMESPACE_END
