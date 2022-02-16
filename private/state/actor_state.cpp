//
//  actor_state.cpp
//  dragon_drop_m
//
//  Created by Vanny Sou on 7/1/14.
//
//
#include "game_state.h"
#include "actor_state.h"
#include "obstacles.h"
#include "player_actor.h"

/********************************************************************************************************
 Actor State class implementations
 ********************************************************************************************************/

ActorState::ActorState(BaseActor* actor, unsigned long id, CIwGameAnimTimeline* state_timeline, uint64 next_state_update)
: m_actor(actor),  m_id(id), m_stateTimeline(state_timeline), m_NextStateUpdate(next_state_update)
{
    if(m_NextStateUpdate != 0)
        m_NextStateUpdate = m_stateTimer.GetCurrentTimeMs() + next_state_update;
    
}

ActorState::~ActorState()
{
    //m_actor->ClearMovement();
}

/**
 * \brief Loads data pertaining to the actor's state.
 *
 * This function loads the animation timeline of the actor when the state first starts up. It then
 * plays a sound appropriate to the state the actor is in. Finally, the derived object will make any
 * final initialization.
 */
void ActorState::Load()
{
    // DEBUGGING**
    if(m_actor->getType() == ENEMY)
    {
        CIwGameString state_name = m_actor->GetActorStateManager()->GetCurrentStateStr();
        
        CIwGameError::LogError("Loading State: ", state_name.c_str());
    }
    // **
    
    if(m_stateTimeline == NULL)
        return;
    
    // load the animation timeline
    CIwGameAnimTimeline* timeline = GetStateTimeline();
    GetActor()->setTimeline(timeline);
    timeline->restart();
}

void ActorState::Close()
{

}

void ActorState::Update()
{

}

void ActorState::SetStateTimeline(CIwGameAnimTimeline* timeline)
{
    m_stateTimeline = timeline;
}

void ActorState::SetStateTimeline(CIwGameAnimTimeline* timeline, bool load)
{
    m_stateTimeline = timeline;
    
    if(load)
        GetActor()->setTimeline(timeline);
}

CIwGameAnimTimeline* ActorState::GetStateTimeline()
{
    return m_stateTimeline;
}

/**
 * \brief Sets a duration for the next state update.
 */
void ActorState::SetNextStateUpdate(const uint64 next_update)
{
    if(next_update == 0)
    {
        return;
    }
    m_NextStateUpdate = m_stateTimer.GetCurrentTimeMs() + next_update;
}

/**
 * \brief Checks to see if the state should wait for an update.
 * \return true if the current time passes m_NextStateUpdate
 */
bool ActorState::IsTimeForStateUpdate() const
{
    if(m_stateTimer.GetCurrentTimeMs() > m_NextStateUpdate)
        return true;
    
    return false;
}

bool ActorState::CanAttack() const
{
    return true;
}

bool ActorState::CanBeHurt() const
{
    return true;
}

bool ActorState::CanMove() const
{
    return true;
}

bool ActorState::IsAttackAnimationPlaying() const
{
    return false;
}
