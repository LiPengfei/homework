#ifndef __BLAZER_SABRE_LOGSERVICE_H__
#define __BLAZER_SABRE_LOGSERVICE_H__

#include "File/File.h"
#include "Log/LogPublic.h"
#include "Log/LogManager.h"
#include "Tool/Application.h"
#include "DesignPattern/Singleton.h"
#include "File/IniFile.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

/************************************************************************/
/* class BLogger                                                        */
/************************************************************************/
class BLogger : public BService
{
public:
    DECLARE_SINGLETON_PATTERN(BLogger);
    
private:
    BLogManager  *m_pLogManager;
    BSPIniFile    m_spIniFile;

public:
    BLogger();
    virtual ~BLogger();

public:
    virtual BOOL Init();
    virtual BOOL UnInit();
    virtual BOOL Run();
    
    void WriteLog(BLogRecord *pRecord);

private:
    INT PrepareFileLogger();         // -1 error happened, 0 did not start but no error, 1 start;
    INT PrepareDbLogger();
    INT PrepareNetLogger();
    INT PrepareConsoleLogger();
    INT GetFlag(const char *cpName); // -1 error , value success;
};

BZ_DECLARE_NAMESPACE_END

#endif