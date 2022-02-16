//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// state_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
/********************************************************************************************************
 Falling
 ********************************************************************************************************/

#include "falling_state.h"
#include "moving_state.h"

Falling::Falling(BaseActor* actor, CIwGameAnimTimeline* fall_timeline) : ActorState(actor, ACTOR_FALLING, fall_timeline), m_isGrounded(false)
{
    //SetID(ACTOR_FALLING);
}

void Falling::Load()
{
    ActorState::Load();
    if(GetActor()->HasItem(UMBRELLA))
    {
        CIwGameAnimTimeline* timeline = GetActor()->GetAnimate().Hovering;
        SetStateTimeline(timeline, true);
        //GetActor()->setTimeline(timeline);
    }
    
    m_isGrounded = false;
}

void Falling::Close()
{
    ActorState::Close();
}

void Falling::Update()
{
    ActorState::Update();
    
    if(GetActor()->getType() != PLAYER)
        return;
    
    if(GetActor()->HasItem(UMBRELLA) && ((PlayerActor*)GetActor())->IsActionBarActive())// && selected)
        GetActor()->setVelocityY(UMBRELLA_FALL_VALUE);
    
    m_isGrounded = GetActor()->GetBaseSensor()->HasContacted();
    
    
    CIwGameActor* contacted_actor = GetActor()->GetBaseSensor()->CheckForContact();
    
    if(contacted_actor != NULL && contacted_actor->getActualClassTypeHash() == CIwGameString("wind").getHash())
    {
        float w_speed   = static_cast<Wind*>(contacted_actor)->GetWindSpeed();
        float w_dir     = static_cast<Wind*>(contacted_actor)->GetWindDirection();
        
        float windVelocity = w_speed * w_dir;
        
        GetActor()->SetHoverValue(windVelocity);
        //GetActor()->setVelocityY(windVelocity);
        m_isGrounded = false;
        return;
    }
    
    if(m_isGrounded && (contacted_actor != NULL) &&
       (contacted_actor->getActualClassTypeHash() != CIwGameString("wind").getHash()))
    {
        GetActor()->SetActorFriction(ACTOR_DEFAULT_FRICTION);
        GetActor()->GetActorStateManager()->OnNotify(GetActor(), EVENT_START_MOVING);
    }
    
}


