#include "Tool/Frame.h"

BZ_DECLARE_NAMESPACE_BEGIN(sabre)

BLogicFrameCtrl::BLogicFrameCtrl() : m_dwLogicFrameCounter(0), m_dwGameLogicFPS(0)
{
}

BLogicFrameCtrl::~BLogicFrameCtrl()
{
    m_timer.Stop();
}

VOID BLogicFrameCtrl::Start()
{
    m_timer.Start();
}

BOOL  BLogicFrameCtrl::CanFrameCarryOn() CONST
{
    if (m_dwLogicFrameCounter * 1000 <= m_timer.GetElapseInMilliSec() * MAX_GAME_LOGIC_FPS)
        return TRUE;
    return FALSE;
}

DWORD BLogicFrameCtrl::GetFrameCounter() CONST
{
    return m_dwLogicFrameCounter;
}

DWORD BLogicFrameCtrl::AddFrameCounter()
{
    return ++m_dwLogicFrameCounter;
}

DWORD BLogicFrameCtrl::DelFrameCounter()
{
    return --m_dwLogicFrameCounter;
}

DWORD BLogicFrameCtrl::GetGameLogicFPS()
{
    DWORD dwElapseInMilliSec = m_timer.GetElapseInMilliSec();

    if (dwElapseInMilliSec <= 0)
        return 0;

    m_dwGameLogicFPS = m_dwLogicFrameCounter * 1000 / dwElapseInMilliSec;
    return m_dwGameLogicFPS;
}

BZ_DECLARE_NAMESPACE_END