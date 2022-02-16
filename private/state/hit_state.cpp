//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// state_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Actor Hit state
//--------------------------------------------------------------------------------------------------------

#include "hit_state.h"
#include "idle_state.h"

Hit::Hit(BaseActor* actor, CIwGameAnimTimeline* timeline, uint64 next_update, int dmg) :
    ActorState(actor, ACTOR_HIT, timeline, next_update), m_dmg(dmg) {}

void Hit::Load()
{
    m_actor->ApplyDamage(m_dmg);
    
    if(m_actor->isDying())
    {
        m_actor->KillActor(IMPACT);
        m_actor->NotifyDying();
    }
    ActorState::Load();
}

void Hit::Update()
{
    ActorState::Update();
    
    if(!IsTimeForStateUpdate())
        return;
    
    m_actor->ActionsOnNotify(EVENT_WAS_HURT);
    //m_actor->NotifyHit();
    
    //m_actor->ChangeState(ACTOR_IDLE);
}

bool Hit::CanBeHurt() const
{
    return false;
}
