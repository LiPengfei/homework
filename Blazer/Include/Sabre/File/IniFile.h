#ifndef __BLASER_SABRE_INIFILE_H__
#define __BLASER_SABRE_INIFILE_H__

#include <string>
#include <vector>
#include "Structure/SimpleString.h"
#include "Public.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

std::vector<std::string> BZ_StringSplit(const char* cpStr, char cFence);

struct BKeyNode
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

    BSimpleString GetKey()
    {
        return m_bssKey;
    }

    BSimpleString GetValue()
    {
        return m_bssValue;
    }
};

struct BSegmentNode
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

    int  GetSize()
    {
        return m_keyArray.size();
    }

    void Clear()
    {
        m_keyArray.clear();
    }

    BKeyNode operator[](size_t nIdx)
    {
        return m_keyArray[nIdx];
    }
};

class BIniFile : public BUnCopyable
{
public:
    typedef std::vector<BSegmentNode> BSegArray;

    enum 
    {
        LINE_BUFFER_SIZE = 1024
    };

public:
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
    BOOL GetSegment(const char *cpSegName,
        OUT BSegmentNode &segVal) const;

    void Clear()
    {
        m_segArray.clear();
    }

public:
    int FindSegment(const char *const cpcSegName) const;
    int FindKey(const char *const cpcSegName,
        const char *const cpcKeyName) const;

protected:
    static bool IsSegment(const char *const cpcLine);
    static int  IsKeyNode(const char *const cpcLine);
    static bool IsComment(const char *const cpcLine);
};

BZ_DECLARE_NAMESPACE_END

#endif