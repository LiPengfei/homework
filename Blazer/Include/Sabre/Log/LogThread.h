#ifndef __BLAZER_SABRE_LOGTHREAD_H__
#define __BLAZER_SABRE_LOGTHREAD_H__

#include "Public.h"
#include "Thread/Thread.h"
#include "Log/LogFormater.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BConsoleLogThread                                                             */
/*-------------------------------------------------------------------------------------------*/
class BConsoleLogThread : public BThread
{
private:
    BSPLogFormater m_spLogFormater;

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
    BSPLogFormater m_spLogFormater;

public:
    BFileLogThread();
    virtual ~BFileLogThread();

public:
    BOOL Init();

private:
    virtual UINT Run();
};

/*-------------------------------------------------------------------------------------------*/
/* CLASS     : BNetLogThread                                                                 */
/*-------------------------------------------------------------------------------------------*/
class BNetLogThread : public BThread
{
public:
    BNetLogThread();
    virtual ~BNetLogThread();

private:
    virtual UINT Run();
};

BZ_DECLARE_NAMESPACE_END

#endif