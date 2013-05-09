#include "Log/LogService.h"
#include "Net/SocketAgency.h"

#include "DesignPattern/Singleton.h"
using namespace BZ(sabre);

int main()
{
    BLogger *pLogger = BZ_SINGLETON_GET_PTR(BLogger);
    pLogger->Init();
    pLogger->Run();

    BSocketAgency *p (new BSocketAgency);
    p->Init();
    p->Start();

    assert(false);
}