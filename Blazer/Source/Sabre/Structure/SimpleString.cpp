#include "Structure/SimpleString.h"

#include <string.h>

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

    BSimpleString::BSimpleString() : m_cpStr(NULL), m_nLength(0)
{
}

BSimpleString::BSimpleString(const char *cpStr)
{
    BZ_CHECK_RETURN_VOID_QUIET(NULL != cpStr);

    m_nLength = strlen(cpStr) + 1;
    m_cpStr  = new char[m_nLength];

    strcpy(m_cpStr, cpStr);
}

BSimpleString::BSimpleString(const char *cpStr, size_t nLength)
    : m_cpStr (NULL), m_nLength(0)
{
    BZ_CHECK_RETURN_VOID_QUIET(NULL != cpStr);
 
    m_nLength = nLength;
    m_cpStr  = new char[nLength];
    //can handle char array with \0 at middle
    memcpy(m_cpStr, cpStr, nLength);
}

BSimpleString::BSimpleString(const BSimpleString& rhs)  
    : m_cpStr (NULL), m_nLength(0)
{
    if (!rhs)
    {
        return;
    }

    m_nLength = rhs.m_nLength;
    m_cpStr  = new char[rhs.m_nLength];
    memcpy(m_cpStr, rhs.m_cpStr, m_nLength);
}

BSimpleString &BSimpleString::operator=(const BSimpleString &rhs)
{
    if (&rhs == this)
    {
        return *this;
    }

    m_nLength = rhs.m_nLength;

    BZ_SafelyDeleteArrayPtr(m_cpStr);

    if (!rhs)
    {
        m_cpStr  = NULL;
        return *this;
    }

    m_nLength = rhs.m_nLength;
    m_cpStr  = new char[rhs.m_nLength];

    memcpy(m_cpStr, rhs.m_cpStr, rhs.m_nLength);
    return *this;
}

BSimpleString::~BSimpleString()
{
    Clean();
}

void BSimpleString::Clean()
{
    if (m_cpStr)
    {
        BZ_SafelyDeleteArrayPtr(m_cpStr);
    }
    m_cpStr  = NULL;
    m_nLength = NULL;
}

//NULL 跟任何字符串(包括NULL)做任何比较，都返回false;
//NULL 跟任何东西判断不等，都返回true;
bool operator==(const BSimpleString &lhs, const BSimpleString &rhs)
{
    if (!lhs)
    {
        return false;
    }
    lhs.m_cpStr[lhs.m_nLength - 1] = 0;
    rhs.m_cpStr[rhs.m_nLength - 1] = 0;
    return 0 == strcmp(lhs.m_cpStr, rhs.m_cpStr);
}

bool operator< (const BSimpleString &lhs, const BSimpleString &rhs)
{
    if (!lhs)
    {
        return false;
    }

    lhs.m_cpStr[lhs.m_nLength - 1] = 0;
    rhs.m_cpStr[rhs.m_nLength - 1] = 0;
    return -1 == strcmp(lhs.m_cpStr, rhs.m_cpStr);
}

bool operator!=(const BSimpleString &lhs, const BSimpleString &rhs)
{
    if (!lhs)
    {
        return true;
    }
    lhs.m_cpStr[lhs.m_nLength - 1] = 0;
    rhs.m_cpStr[rhs.m_nLength - 1] = 0;
    return 0 != strcmp(lhs.m_cpStr, rhs.m_cpStr);
}

bool operator!(const BSimpleString &rhs)
{
    return !rhs.m_cpStr;
}

BZ_DECLARE_NAMESPACE_END
