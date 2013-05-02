#ifndef __BLAZER_SABRE_APPLICATION_H__
#define __BLAZER_SABRE_APPLICATION_H__

#include "Public.h"
#include "SmartPointer/SharedPtr.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BService : private BUnCopyable
{
public:
    BService() {}
    virtual ~BService() {}

public:
    virtual BOOL Init()   = 0;
    virtual BOOL UnInit() = 0;
    virtual BOOL Run()    = 0;
};

typedef BSharedPtr<BService> BSPService;

class BAgency : private BUnCopyable
{
public:
    BAgency() {}
    virtual ~BAgency() {}

public:
    virtual BOOL Init()   = 0;
    virtual BOOL UnInit() = 0;
    virtual BOOL Run()    = 0;
};

typedef BSharedPtr<BAgency> BSPAgency;

class BApplication : private BUnCopyable
{
public:
    BApplication() {}
    virtual ~BApplication() {}

public:
    virtual BOOL Init()   = 0;
    virtual BOOL UnInit() = 0;
    virtual BOOL Run()    = 0;
};

typedef BSharedPtr<BApplication> BSPApplication;

BZ_DECLARE_NAMESPACE_END

#endif