// File:   File.h
//
// Module: Sabre
// Filter: File
//
// Comment:
// 
// 
/*
 * �ļ������ļ���API
 * fopen        c�ṩAPI      ֧��windows linux
 * ifstream     c++�ṩAPI    ֧��windows linux
 * CreateFile   win32�ṩAPI  ֧��windows ֧��2G�����ļ�
 * open         linux�ṩAPI  ֧��linux
 * 
 * fopen 32λwindows linux ���ļ���С����2G ԭ�����α�����long(�з���32λ)
 * fopen64
 */
#ifndef __BLAZER_SABRE_FILE_H__
#define __BLAZER_SABRE_FILE_H__

#include "Public.h"
#include "SmartPointer/SharedPtr.h"
#include "Structure/UniversalManager.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

//FOPEN_MAX 

// < 2G
class BFile
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

typedef BSharedPtr<BFile>                       BSPFile;
typedef BSharedPtr<BUniversalManager<BSPFile> > BSPFileManager;
typedef BUniversalManager<BSPFile>              BFileManager;

// >= 4G
class BLargeFile/* : public BIFile*/
{

};

BZ_DECLARE_NAMESPACE_END

#endif // __BLAZER_SABRE_FILE_H__
