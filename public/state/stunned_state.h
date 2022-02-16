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

#if !defined(_STUNNED_STATE_H_)
#define _STUNNED_STATE_H_

#include "base_actor.h"
#include "actor_state.h"

//--------------------------------------------------------------------------------------------------------
//  Actor Stunned state
//--------------------------------------------------------------------------------------------------------

class Stunned : public ActorState
{
public:
    Stunned(BaseActor* actor, CIwGameAnimTimeline* hover_timeline);
    
    void Load();
    void Close();
    void Update();
    
    
private:
    
};

#endif /* defined(__dragon_drop_m__actor_state__) */
