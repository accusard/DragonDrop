//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// game_timer.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// BombActor class implementation
//--------------------------------------------------------------------------------------------------------

#ifndef __dragon_drop__GameTimer__
#define __dragon_drop__GameTimer__

#include "IwGameUtil.h"

class GameTimer : public CIwGameTimer
{
public:
    GameTimer();
    
    void Pause();
    void Resume();
    
    uint64 GetTimeSinceLastPause() const;
    
private:
    uint64 m_timeSinceLastPause;
    uint64 m_timeRemaining;
};

#endif /* defined(__dragon_drop__GameTimer__) */
