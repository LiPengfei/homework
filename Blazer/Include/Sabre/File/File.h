// File:   File.h
//
// Module: Sabre
// Filter: File
//
// Comment:
// 
// 
/*
 * 文件操作的几类API
 * fopen        c提供API      支持windows linux
 * ifstream     c++提供API    支持windows linux
 * CreateFile   win32提供API  支持windows 支持2G以上文件
 * open         linux提供API  支持linux
 * 
 * fopen 32位windows linux 打开文件大小限制2G 原因是游标类型long(有符号32位)
 * fopen64
 */
#ifndef __BLAZER_SABRE_FILE_H__
#define __BLAZER_SABRE_FILE_H__

#include "Public.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)


//class BIFile
//{
//public: 
//    virtual ~BIFile() = 0;
//
//public:
//    virtual BOOL Open(
//        const char* const cpcFile,
//        const char* const cpcMode// enum
//    ) = 0;
//
//    virtual BOOL Close() = 0;
//
//    virtual LONG Size() const = 0;
//
//    virtual BOOL SeekBegin(LONG lOffset) = 0;
//    virtual BOOL SeekEnd(LONG lOffset) = 0;
//    virtual BOOL SeekCurrent(LONG lOffset) = 0;
//
//    virtual LONG Tell() const = 0;
//
//    virtual LONG Read(char* const cpBuffer, LONG lBufferSize) = 0;
//};

//FOPEN_MAX 

// < 2G
class BFile /* : public BIFile*/
{
public: // constructor and destructor
    BFile() : m_hFile(NULL), m_bBinaryFile(FALSE)
    {
    }

    ~BFile()
    {
        Close();
    }

public: // member functions
    virtual BOOL Open(const char *const cpcFile, const char *const cpcMode);
    virtual BOOL Close();

    virtual BOOL SeekBegin(LONG lOffset);
    virtual BOOL SeekEnd(LONG lOffset);
    virtual BOOL SeekCurrent(LONG lOffset);

    virtual LONG Size() const;
    virtual LONG Tell() const;
    virtual BOOL Flush();

    virtual BOOL IsEof() const;
    virtual BOOL IsOpen() const;

    virtual LONG Read(char *const cpBuffer, LONG lBufferSize, LONG lReadSize);
    virtual LONG Write(const char *const cpcBuffer, LONG lBufferSize, LONG lWriteSize);

    virtual LONG ReadTextLine(OUT char *const cpStr, IN LONG lReadLen);
    virtual LONG WriteTextLine(const char* const cpcBuffer, LONG lBufferSize);

private: // member variables
    FILE *m_hFile;
    BOOL  m_bBinaryFile;
};

// >= 4G
class BLargeFile/* : public BIFile*/
{

};

BZ_DECLARE_NAMESPACE_END

#endif // __BLAZER_SABRE_FILE_H__
