//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// state_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Actor Attack state
//--------------------------------------------------------------------------------------------------------

#include "attack_state.h"

/**
 * \brief The constructor.
 *
 * \param attacker The attacker, usually the actor that is calling this constructor.
 * \param target The target of the attacker recieving this attack (can be NULL if no continous attack).
 * \param timeline The animation timeline for this attack.
 * \param next_state_update The time before the next state update.
 */
Attack::Attack(BaseActor* attacker, CIwGameActor* target, CIwGameAnimTimeline* timeline, uint64 next_state_update) :
    ActorState(attacker, ACTOR_ATTACK, timeline, next_state_update), m_targetActor(target), m_attackSound("")
{}

/**
 * \brief Loads the attack sequence of the actor attached to this state.
 *
 * This function loads the animation timeline of the actor when the state first starts up. It then
 * plays a sound appropriate to the state the actor is in. Finally, it will make any final initialization.
 */
void Attack::Load()
{
    // load the timeline
    ActorState::Load();
    
    // play the sound
    m_attackSound = m_actor->GetSoundAttack();
    IW_GAME_AUDIO->PlaySound(m_attackSound);
    
    // determine and start the attack
    m_actor->StartAttack();
}

void Attack::Close()
{
    ActorState::Close();
    //m_actor->PostAttack();
}

void Attack::Update()
{
    ActorState::Update();
    
    if(!IsTimeForStateUpdate() && m_stateTimeline->isPlaying())
        return;
    
    // update attack only if m_targetActor exist
    if(m_actor->UpdateAttack(m_targetActor))
        return;

    // notify finished
    m_actor->GetActorStateManager()->OnNotify(m_actor, EVENT_ATTACK_FINISHED);
}

/**
 * \brief Checks to see if the actor can attack in this state
 * \return true if the actor is allowed to attack
 */
bool Attack::CanAttack() const
{
    return true;
}

bool Attack::IsAttackAnimationPlaying() const
{
    if(m_stateTimeline != NULL && m_stateTimeline->isPlaying())
        return true;
    
    return false;
}
