#ifndef __BLAZER_SABRE_SIMPLESTRING_H__
#define __BLAZER_SABRE_SIMPLESTRING_H__

#include "Public.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BSimpleString
{
public:
    char   *m_cpStr;
    size_t  m_nLength;

public:
    BSimpleString();

    //for you passing into char string with \0 terminal 
    BSimpleString(const char *cpStr);

    //you can pass a str with \0 at middle without lose data
    BSimpleString(const char *cpStr, size_t m_nLength);
    virtual ~BSimpleString();

public:
    BSimpleString(const BSimpleString &rhs);
    BSimpleString &operator=(const BSimpleString &rhs);

public:
    void Clean();
public:
    friend bool operator==(const BSimpleString &lhs, const BSimpleString &rhs);
    friend bool operator< (const BSimpleString &lhs, const BSimpleString &rhs);
    friend bool operator!=(const BSimpleString &lhs, const BSimpleString &rhs);
    friend bool operator! (const BSimpleString &rhs);
};

BZ_DECLARE_NAMESPACE_END

#endif
