//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// state_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Idle state of the actor
//--------------------------------------------------------------------------------------------------------

#if !defined(_IDLE_STATE_H_)
#define _IDLE_STATE_H_

#include "base_actor.h"
#include "actor_state.h"

class Idle : public ActorState
{
public:
    Idle(BaseActor* actor, CIwGameAnimTimeline* idle_timeline, uint64 next_update = 0);
    
    void Load();
    void Close();
    void Update();
};

//--------------------------------------------------------------------------------------------------------
//  LookingForTarget state of the actor
//--------------------------------------------------------------------------------------------------------
class LookingForTarget : public ActorState
{
public:
    LookingForTarget(BaseActor* actor, CIwGameAnimTimeline* look_for_timeline, int target_type);
    
    void Update();
    
private:
    int m_targetType;
};

#endif
