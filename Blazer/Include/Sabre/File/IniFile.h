#ifndef _K_COMMON_INIFILE_H_
#define _K_COMMON_INIFILE_H_

#include <assert.h>
#include <string.h>
#include <vector>
#include "Structure/SimpleString.h"
#include "Public.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BKeyNode
{
public:
    BSimpleString m_bssKey;
    BSimpleString m_bssValue;
public:
    BKeyNode()
    { }

    BKeyNode(const char *const cpcKey, const char *const cpcValue)
        : m_bssKey(cpcKey), m_bssValue(cpcValue)
    { }

};

class BSegmentNode
{
private:
    typedef std::vector<BKeyNode> BKeyArray;

public:
    BSimpleString m_bssSeg;
    BKeyArray     m_keyArray;

public:
    BSegmentNode()
    { }

    BSegmentNode(const BSimpleString &bssSeg, const BKeyArray &keyArray)
        : m_bssSeg(bssSeg), m_keyArray(keyArray)
    { }

public:
    void AddKey(const BKeyNode &keyNode)
    {
        m_keyArray.push_back(keyNode);
    }

    void RemoveTail()
    {
        m_keyArray.pop_back();
    }

    void Clear()
    {
        m_keyArray.clear();
    }
};

class BIniFile : public BUnCopyable
{
#ifdef _DEBUG
public:
#else
private:
#endif
    typedef std::vector<BSegmentNode> BSegArray;

    enum 
    {
        LINE_BUFFER_SIZE = 1024
    };

#ifdef _DEBUG
public:
#else
private:
#endif
    char      m_cComment;
    BSegArray m_segArray;

public:
    BIniFile(char cComment = ';')
        : m_cComment(cComment)
    { }

    ~BIniFile()
    {
        Clear();
    }

public:
    BOOL LoadFile(const char *const fileName);
    BOOL SaveFile(const char *const fileName) const;
    BOOL GetValue(const char *const cpcSegName, 
        const char *const pcpKeyName,
        OUT char *pValue) const;
    BOOL GetIntValue(const char *const cpcSegName, 
        const char *const pcpKeyName,
        OUT int &pValue) const;
    BOOL SetValue(const char *const cpcSegName,
        const char *const cpcKeyName,
        const char *const cpcValue);
    BOOL SetIntValue(const char *const cpcSegName,
        const char *const cpcKeyName,
        int cpcValue);
    BOOL AddKey(const char *const cpcSegName,
        char *const cpcKeyName,
        const char *const cpcValue);

    void Clear()
    {
        m_segArray.clear();
    }

protected:
    int  FindSegment(const char *const cpcSegName) const;
    BOOL FindKey(const char *const cpcSegName,
        const char *const cpcKeyName) const;

protected:
    static bool IsSegment(const char *const cpcLine);
    static int  IsKeyNode(const char *const cpcLine);
    static bool IsComment(const char *const cpcLine);
};

BZ_DECLARE_NAMESPACE_END

#endif