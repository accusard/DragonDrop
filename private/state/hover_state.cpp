//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// state_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Actor Hovering state
//--------------------------------------------------------------------------------------------------------

#include "hover_state.h"

Hovering::Hovering(BaseActor* actor, CIwGameAnimTimeline* timeline) : ActorState(actor, ACTOR_HOVER, timeline), m_hoverValue(0.0f) {}

void Hovering::Load()
{
    ActorState::Load();
    //m_hoverValue = GetActor()->GetHoverValue();
}

void Hovering::Close()
{
    ActorState::Close();
}

void Hovering::Update()
{
    ActorState::Update();
    
    CIwGameActor* contacted_actor = GetActor()->GetBaseSensor()->CheckForContact();
    
    if(contacted_actor != NULL && contacted_actor->getActualClassTypeHash() == CIwGameString("wind").getHash())
    {
        float w_speed   = static_cast<Wind*>(contacted_actor)->GetWindSpeed();
        float w_dir     = static_cast<Wind*>(contacted_actor)->GetWindDirection();
        
        float windVelocity = w_speed * w_dir;
        GetActor()->SetHoverValue(windVelocity);
        //GetActor()->setVelocityY(windVelocity);
        return;
        //m_isGrounded = false;
    }
    GetActor()->setVelocityY(UMBRELLA_FALL_VALUE);
}

void Hovering::SetHoverValue(float hover_value)
{
    m_hoverValue = hover_value;
}


