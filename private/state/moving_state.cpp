//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// state_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
/********************************************************************************************************
 Moving
 ********************************************************************************************************/

#include "moving_state.h"
#include "falling_state.h"
#include "idle_state.h"

Moving::Moving(BaseActor* actor, CIwGameAnimTimeline* timeline) : ActorState(actor, ACTOR_MOVING, timeline) {}

void Moving::Load()
{
    ActorState::Load();

    m_actor->setTimeline(m_stateTimeline);
    if(m_stateTimeline != NULL)
        m_stateTimeline->play();
}

void Moving::Close()
{
    //m_subject.Notify(m_actor, EVENT_MOVEMENT_FINISHED);
}

void Moving::Update()
{
    ActorState::Update();
    
    if(m_actor->GetCurrentVelocity().x <= MOVING_SPEED/2 && m_actor->GetCurrentVelocity().x >= -MOVING_SPEED/2)
        m_actor->GetActorStateManager()->OnNotify(m_actor, EVENT_IS_IDLE);
    
    if(m_actor->GetCurrentVelocity().y > 0.1)
        m_actor->GetActorStateManager()->OnNotify(m_actor, EVENT_FALLING);
    
    if(m_actor->GetMovement() != NULL)
        m_actor->GetMovement()->Update();
}


