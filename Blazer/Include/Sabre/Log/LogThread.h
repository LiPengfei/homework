#ifndef __BLAZER_SABRE_LOGTHREAD_H__
#define __BLAZER_SABRE_LOGTHREAD_H__

#include "Public.h"
#include "Thread/Thread.h"
#include "File/File.h"
#include "Database/MysqlManager.h"
#include "Net/SocketStream.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BConsoleLogThread                                                             */
/*-------------------------------------------------------------------------------------------*/
class BConsoleLogThread : public BThread
{
private:
// modified by lipengfei 13/05/03
public:
    BConsoleLogThread();
    virtual ~BConsoleLogThread();

public:
    BOOL Init();

private:
    virtual UINT Run();
};

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BFileLogThread                                                                */
/*-------------------------------------------------------------------------------------------*/
class BFileLogThread : public BThread
{
private:
    // modified by lipengfei 13/05/03
    BSPFileManager m_fileManager;   // add by lipengfei 13/05/02

public:
    BFileLogThread();
    virtual ~BFileLogThread();

public:
    BOOL Init();

private:
    virtual UINT Run();
};

/***********************************************************************/
/* CLASS     : BNetLogThread                                           */
/***********************************************************************/
class BNetLogThread : public BThread
{
private:
    BSPSocketStreamManager m_sockstrManager;

public:
    BNetLogThread();
    virtual ~BNetLogThread();

public:
    virtual BOOL Init();

private:
    virtual UINT Run();
};

/************************************************************************/
/* class DbLogThread                                                    */
/************************************************************************/
class BDbLogThread : public BThread
{
private:
    BSPMysqlTableManager m_mysqlManager;

public:
    BDbLogThread();
    virtual ~BDbLogThread();

public:
    virtual BOOL Init();

private:
    virtual UINT Run();
};
BZ_DECLARE_NAMESPACE_END

#endif