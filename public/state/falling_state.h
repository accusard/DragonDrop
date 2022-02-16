//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// state_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------

#if !defined(_FALLING_STATE_H_)
#define _FALLING_STATE_H_

#include "player_actor.h"
#include "base_actor.h"
#include "actor_state.h"

class Falling : public ActorState
{
public:
	Falling(BaseActor* actor, CIwGameAnimTimeline* idle_timeline);
    
    void Load();
    void Close();
    void Update();
	
    
protected:
    bool m_isGrounded;
    
};

#endif /* defined(__dragon_drop_m__actor_state__) */
