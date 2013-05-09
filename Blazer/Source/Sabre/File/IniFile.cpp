#include "File/IniFile.h"
#include <string.h>
#include <fstream>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

#define TEMP_BUFFER_LEN 1024

typedef std::vector<std::string> VecString;

VecString BZ_StringSplit(const char* cpStr, char cFence)
{
    VecString   retValue;
    const char *cpTmp   = cpStr;
    while ('\0' != *cpStr)
    {
        if (cFence == *cpStr)
        {
            retValue.push_back(std::string(cpTmp, cpStr));
            cpTmp = cpStr;
            ++cpTmp;
        }
        ++cpStr;
    }

    retValue.push_back(std::string(cpTmp, cpStr));

    return retValue;
}

BOOL BIniFile::LoadFile(const char *const fileName)
{
    BZ_CHECK_C_STRING_RETURN_BOOL(fileName);
    
    std::ifstream fileIn(fileName);
    if (!fileIn)
    {
        assert(false);
        return FALSE;
    }

    char   buffer[LINE_BUFFER_SIZE];
    bool   getSeg  = false;
    size_t nBufLen = 0; 
    int    nAssPos = 0;
    while (!fileIn.eof())
    {
        fileIn.getline(buffer, LINE_BUFFER_SIZE);
        if ((!getSeg) && (!IsSegment(buffer)))
        {
            continue;
        }

        if (IsComment(buffer))
        {
            continue;
        }

        if (IsSegment(buffer))
        {
            BSegmentNode seg;
            getSeg              = true;
            nBufLen             = strlen(buffer);
            buffer[nBufLen - 1] = '\0';
            seg.m_bssSeg = BSimpleString(buffer + 1);

            m_segArray.push_back(seg);
            continue;
        }

        if (-1 != (nAssPos = IsKeyNode(buffer)))
        {
            BKeyNode key;
            buffer[nAssPos] = '\0';
            key.m_bssKey     = BSimpleString(buffer);
            key.m_bssValue   = BSimpleString(buffer + nAssPos + 1);
            m_segArray.back().AddKey(key);
            continue;
        }
    }

    fileIn.close();
    return TRUE;
}

BOOL BIniFile::SaveFile(const char *const fileName) const
{
    BZ_CHECK_C_STRING_RETURN_BOOL(fileName);

    std::ofstream fileOut(fileName);
    if (!fileOut)
    {
        assert(false);
        return FALSE;
    }
    // need add lipengfei 2013/03/21
    int nSegCount = m_segArray.size();
    int nKeyCount = 0;

    for (int i = 0; i != nSegCount; ++i)
    {
        nKeyCount = m_segArray[i].m_keyArray.size();
        fileOut << "[" << m_segArray[i].m_bssSeg.m_cpStr << "]" << std::endl;

        for (int j = 0; j != nKeyCount; ++j)
        {
            fileOut << m_segArray[i].m_keyArray[j].m_bssKey.m_cpStr
                << "=" << m_segArray[i].m_keyArray[j].m_bssValue.m_cpStr
                << std::endl;
        }
    }

    fileOut.close();
    return TRUE;
}

BOOL BIniFile::GetValue(const char *const cpcSegName, 
                        const char *const cpcKeyName,
                        OUT char * pValue) const
{
    BZ_CHECK_RETURN_BOOL(NULL != cpcSegName);
    BZ_CHECK_RETURN_BOOL(NULL != cpcKeyName);
    BZ_CHECK_RETURN_BOOL(NULL != pValue);

    int nIdx = FindSegment(cpcSegName);

    if (-1 == nIdx)
    {
        return FALSE;
    }

    for (int i = m_segArray[nIdx].m_keyArray.size() - 1; i != -1; --i)
    {
        if (cpcKeyName == m_segArray[nIdx].m_keyArray[i].m_bssKey)
        {
            ::strcpy(pValue, m_segArray[nIdx].m_keyArray[i].m_bssValue.m_cpStr);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL BIniFile::GetIntValue(const char *const cpcSegName, 
                        const char *const cpcKeyName,
                        OUT int &pValue) const
{
    BZ_CHECK_RETURN_BOOL(NULL != cpcSegName);
    BZ_CHECK_RETURN_BOOL(NULL != cpcKeyName);

    int nIdx = FindSegment(cpcSegName);

    if (-1 == nIdx)
    {
        return FALSE;
    }

    for (int i = m_segArray[nIdx].m_keyArray.size() - 1; i != -1; --i)
    {
        if (cpcKeyName == m_segArray[nIdx].m_keyArray[i].m_bssKey)
        {
            char cpTmp[TEMP_BUFFER_LEN];
            ::strcpy(cpTmp, m_segArray[nIdx].m_keyArray[i].m_bssValue.m_cpStr);
            pValue = atoi(cpTmp);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL BIniFile::SetValue(const char *const cpcSegName,
                        const char *const cpcKeyName,
                        const char *const cpcValue)
{
    BZ_CHECK_RETURN_BOOL(NULL != cpcSegName);
    BZ_CHECK_RETURN_BOOL(NULL != cpcKeyName);
    BZ_CHECK_RETURN_BOOL(NULL != cpcValue);

    for (int i = m_segArray.size() - 1; i != -1; --i)
    {
        if (cpcSegName == m_segArray[i].m_bssSeg)
        {
            for (int j = m_segArray[i].m_keyArray.size() - 1; j != -1; --j)
            {
                if (cpcKeyName == m_segArray[i].m_keyArray[j].m_bssKey)
                {
                    m_segArray[i].m_keyArray[j].m_bssValue 
                        = BSimpleString(cpcValue);
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

BOOL BIniFile::SetIntValue(const char *const cpcSegName, const char *const cpcKeyName, int cpcValue)
{
    BZ_CHECK_RETURN_BOOL(NULL != cpcSegName);
    BZ_CHECK_RETURN_BOOL(NULL != cpcKeyName);

    for (int i = m_segArray.size() - 1; i != -1; --i)
    {
        if (cpcSegName == m_segArray[i].m_bssSeg)
        {
            for (int j = m_segArray[i].m_keyArray.size() - 1; j != -1; --j)
            {
                if (cpcKeyName == m_segArray[i].m_keyArray[j].m_bssKey)
                {
                    char cpTmp[TEMP_BUFFER_LEN];
                    itoa(cpcValue, cpTmp, 10);
                    m_segArray[i].m_keyArray[j].m_bssValue 
                        = BSimpleString(cpTmp);
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

BOOL BIniFile::AddKey(const char *const cpcSegName,
                      char *const cpcKeyName,
                      const char *const cpcValue)
{
    BZ_CHECK_RETURN_BOOL(NULL != cpcSegName);
    BZ_CHECK_RETURN_BOOL(NULL != cpcKeyName);
    BZ_CHECK_RETURN_BOOL(NULL != cpcValue);

    int nIdx      = 0;
    BKeyNode key;
    key.m_bssKey   = BSimpleString(cpcKeyName);
    key.m_bssValue = BSimpleString(cpcValue);

    if (-1 != (nIdx = FindSegment(cpcSegName)))
    {
        m_segArray[nIdx].m_keyArray.push_back(key);
        return TRUE;
    }

    BSegmentNode seg;
    seg.m_bssSeg = BSimpleString(cpcSegName);
    seg.m_keyArray.push_back(key);
    m_segArray.push_back(seg);
    return TRUE;
}

BOOL BIniFile::GetSegment(const char *cpSegName, OUT BSegmentNode &segVal) const
{
    int nPos = FindSegment(cpSegName);
    if (-1 == nPos)
    {
        return FALSE;
    }
    
    segVal = m_segArray[nPos];

    return TRUE;
}

int  BIniFile::FindSegment(const char *const cpcSegName) const
{
    BZ_CHECK_RETURN_BOOL(NULL != cpcSegName);

    for (int i = m_segArray.size() - 1; i != -1; --i)
    {
        if (cpcSegName == m_segArray[i].m_bssSeg)
        {
            return i;
        }
    }

    return -1;
}

int  BIniFile::FindKey(const char *const cpcSegName,
                       const char *const cpcKeyName) const
{
    BZ_CHECK_RETURN_BOOL(NULL != cpcSegName);
    BZ_CHECK_RETURN_BOOL(NULL != cpcKeyName);

    int nIdx; 
    if (-1 == (nIdx = FindSegment(cpcSegName)))
    {
        return false;
    }

    for (int j = m_segArray[nIdx].m_keyArray.size() - 1; j != -1; --j)
    {
        if (cpcKeyName == m_segArray[nIdx].m_keyArray[j].m_bssKey)
        {
            return j;
        }
    }
    return -1;
}

bool BIniFile::IsSegment(const char *const cpcLine)
{
    if('\0' == cpcLine[0])
    {
        return false;
    }

    int nLen = ::strlen(cpcLine);
    return '[' == cpcLine[0] && ']' == cpcLine[nLen - 1];
}

int  BIniFile::IsKeyNode(const char *const cpcLine)
{
    if ('\0' == cpcLine[0])
    {
        return -1;
    }

    int nLen = ::strlen(cpcLine);
    int nPos  = 0;
    while(nPos != nLen)
    {
        if ('=' == cpcLine[nPos])
        {
            return 0 == nPos ? -1 : nPos;
        }
        ++nPos;
    }

    return -1;
}

bool BIniFile::IsComment(const char *const cpcLine)
{
    if ('\0' == cpcLine[0])
    {
        return false;
    }

    if (';' == cpcLine[0])
    {
        return true;
    }
    return false;
}

BZ_DECLARE_NAMESPACE_END