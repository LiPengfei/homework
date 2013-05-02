#ifndef __BLAZER_SABRE_FRAME_H__
#define __BLAZER_SABRE_FRAME_H__

#include "Tool/Timer.h"

#define MAX_GAME_LOGIC_FPS 18

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

class BLogicFrameCtrl : private BUnCopyable
{
private:
    BTimer m_timer;
    DWORD  m_dwLogicFrameCounter;
    DWORD  m_dwGameLogicFPS;

public:
    BLogicFrameCtrl();
    ~BLogicFrameCtrl();

public:
    VOID  Start();
    BOOL  CanFrameCarryOn() CONST;
    DWORD GetFrameCounter() CONST;
    DWORD AddFrameCounter();
    DWORD DelFrameCounter();
    DWORD GetGameLogicFPS();
};

BZ_DECLARE_NAMESPACE_END

#endif

