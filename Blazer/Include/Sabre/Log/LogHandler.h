#ifndef __BLAZER_SABRE_LOGHANDLER_H__
#define __BLAZER_SABRE_LOGHANDLER_H__

#include "Public.h"
#include "SmartPointer/SharedPtr.h"
#include "Log/LogThread.h"
#include "Structure/UniversalManager.h"
#include "File/File.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BLogManager;
typedef CONST BLogManager * CONST CPCLogManager;

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BLogHandler                                                                   */
/*-------------------------------------------------------------------------------------------*/
class BLogHandler : private BUnCopyable
{
private:
    DWORD m_dwLogHandlerID;

public:
    BLogHandler() : m_dwLogHandlerID(0)
    {
    }

    virtual ~BLogHandler()
    {
    }

public:
    BOOL SetLogHandlerID(DWORD dwLogHandlerID)
    {
        m_dwLogHandlerID = dwLogHandlerID;
        return TRUE;
    }

    DWORD GetLogHandlerID()
    {
        return m_dwLogHandlerID;
    }

    virtual BOOL Init() = 0;
    virtual BOOL UnInit() = 0;
    virtual BOOL Dispatch(IN CPCLogManager cpcLogManager = NULL) = 0;
};

typedef       BLogHandler *       PLogHandler;
typedef       BLogHandler * CONST CPLogHandler;       // equal to "CONST PLogHandler" type
typedef CONST BLogHandler *       PCLogHandler;
typedef CONST BLogHandler * CONST CPCLogHandler;

typedef BSharedPtr<BLogHandler> BSPLogHandler;

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BConsoleLogHandler                                                            */
/*-------------------------------------------------------------------------------------------*/
class BConsoleLogHandler : public BLogHandler
{
private:
    BConsoleLogThread m_workThread;
 
public:
    BConsoleLogHandler();
    virtual ~BConsoleLogHandler();

public:
    virtual BOOL Init();
    virtual BOOL UnInit();
    virtual BOOL Dispatch(IN CPCLogManager cpcLogManager = NULL);
};

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BFileLogHandler                                                               */
/*-------------------------------------------------------------------------------------------*/

class BFileLogHandler : public BLogHandler
{
public:

private:
    BFileLogThread m_workThread;

public:
    BFileLogHandler();
    virtual ~BFileLogHandler();

public:
    virtual BOOL Init();
    virtual BOOL UnInit();
    virtual BOOL Dispatch(IN CPCLogManager cpcLogManager = NULL);
};

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BNetLogHandler                                                                */
/*-------------------------------------------------------------------------------------------*/
class BNetLogHandler : public BLogHandler
{
private:
    BNetLogThread m_workThread;

public:
    BNetLogHandler();
    virtual ~BNetLogHandler();

public:
    virtual BOOL Init();
    virtual BOOL UnInit();
    virtual BOOL Dispatch(IN CPCLogManager cpcLogManager = NULL);
};

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BDbLogHandler                                                               */
/*-------------------------------------------------------------------------------------------*/
class BDbLogHandler : public BLogHandler
{
private:
    BDbLogThread m_workThread;

public:
    BDbLogHandler();
    virtual ~BDbLogHandler();

public:
    virtual BOOL Init();
    virtual BOOL UnInit();
    virtual BOOL Dispatch(IN CPCLogManager cpcLogManager = NULL);
};

BZ_DECLARE_NAMESPACE_END

#endif