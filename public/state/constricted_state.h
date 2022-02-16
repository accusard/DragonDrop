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

#if !defined(_CONSTRICTED_STATE_H_)
#define _CONSTRICTED_STATE_H_

#include "base_actor.h"
#include "actor_state.h"

//--------------------------------------------------------------------------------------------------------
//  Actor Constricted state
//--------------------------------------------------------------------------------------------------------

class Constricted : public ActorState
{
public:
    Constricted(BaseActor* actor, CIwGameAnimTimeline* hover_timeline);
    
    void Load();
    void Close();
    void Update();
    
    void SetConstrictor(CIwGameActor* actor) {Constrictor = actor;}
    
    
private:
    CIwGameActor*       Constrictor;
    
};

#endif /* defined(__dragon_drop_m__actor_state__) */
