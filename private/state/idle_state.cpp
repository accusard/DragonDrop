//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// state_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Actor's Idle state implementations
//--------------------------------------------------------------------------------------------------------
#include "idle_state.h"
#include "moving_state.h"
#include "falling_state.h"

Idle::Idle(BaseActor* actor,
           CIwGameAnimTimeline* idle_timeline,
           uint64 next_update) :
            ActorState(actor, ACTOR_IDLE, idle_timeline, next_update)
{}

void Idle::Load()
{
    ActorState::Load();

    // only certain conditions will allow state change from Scene's StartEvent
    //SetStateCondition(CAN_HIT);
    //SetStateCondition(LOW_LIFE);
    //SetStateCondition(STUNNED);
}

void Idle::Close()
{
    ActorState::Close();
}

/**
 *
 */
void Idle::Update()
{
    ActorState::Update();
    
    if(m_actor->GetCurrentVelocity().y > 0.1)
        m_actor->GetActorStateManager()->OnNotify(m_actor, EVENT_FALLING);
    
    if(m_actor->GetCurrentVelocity().x >= MOVING_SPEED || m_actor->GetCurrentVelocity().x <= -MOVING_SPEED)
    {
        m_actor->GetActorStateManager()->OnNotify(m_actor, EVENT_START_MOVING);
    }
}

//--------------------------------------------------------------------------------------------------------
//  Actor's LookingForTarget state implementations
//--------------------------------------------------------------------------------------------------------
LookingForTarget::LookingForTarget(BaseActor* actor,
                                   CIwGameAnimTimeline* look_for_timeline,
                                   int target_type) :
                                    ActorState(actor, ACTOR_TARGET, look_for_timeline), m_targetType(target_type)

{
    
}

void LookingForTarget::Update()
{
    ActorState::Update();
    
    CIwGameActor* target = m_actor->AcquireTarget(m_targetType);

    if(IsTimeForStateUpdate() && target != NULL && target->isActive())
    {
        m_actor->NotifyTargetAcquired();
        // what should it do once it find target
        //m_actor->AddState(new Idle(m_actor, idle_timeline, 1000), true);
    }
}
