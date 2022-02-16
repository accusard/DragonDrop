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

#if !defined(_ATTACK_STATE_H_)
#define _ATTACK_STATE_H_

#include "base_actor.h"
#include "actor_state.h"

//--------------------------------------------------------------------------------------------------------
//  Actor Attack state
//--------------------------------------------------------------------------------------------------------

/**
 * \brief The attacking state of the actor.
 *
 * The actor will need to be in this state to be able to attack.
 */
class Attack : public ActorState
{
public:
    Attack(BaseActor* attacker, CIwGameActor* target, CIwGameAnimTimeline* timeline, uint64 next_update = 0);
    
    void Load();
    void Close();
    void Update();
    
    bool CanAttack() const;
    bool IsAttackAnimationPlaying() const;
    
private:
    CIwGameActor*       m_targetActor;      /*<< The target of the attack. */
    const char*         m_attackSound;      /*<< The sound to play when loading this state */
    
};

#endif
