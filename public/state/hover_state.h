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

#if !defined(_HOVER_STATE_H_)
#define _HOVER_STATE_H_

#include "base_actor.h"
#include "actor_state.h"
#include "obstacles.h"

class Hovering : public ActorState
{
public:
    Hovering(BaseActor* actor, CIwGameAnimTimeline* hover_timeline);
    
    void Load();
    void Close();
    void Update();
    
    void SetHoverValue(float hover_value);
    
private:
    float m_hoverValue;
};

#endif /* defined(__dragon_drop_m__actor_state__) */
