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

#if !defined(_JUMPING_STATE_H_)
#define _JUMPING_STATE_H_

#include "base_actor.h"
#include "actor_state.h"

class Jumping : public ActorState
{
public:
    Jumping(BaseActor* actor, CIwGameAnimTimeline* idle_timeline, float j_height);
    
    void Load();
    void Close();
    void Update();
    
private:
    bool            m_jumpHeightReached;
    float           m_jumpHeight;
    
};

#endif /* defined(__dragon_drop_m__actor_state__) */
