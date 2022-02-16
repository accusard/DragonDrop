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

#if !defined(_CALL_FOR_HELP_STATE_H_)
#define _CALL_FOR_HELP_STATE_H_

#include "base_actor.h"
#include "actor_state.h"

//--------------------------------------------------------------------------------------------------------
//  Actor CallHelp state
//--------------------------------------------------------------------------------------------------------

class CallHelp : public ActorState
{
public:
    CallHelp(BaseActor* actor, CIwGameAnimTimeline* hover_timeline);
    
    void Load();
    void Close();
    void Update();
    
    
private:
    
};

#endif /* defined(__dragon_drop_m__actor_state__) */
