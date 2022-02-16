//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// state_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Base State Classes
//--------------------------------------------------------------------------------------------------------

#ifndef __dragon_drop_m__actor_state__
#define __dragon_drop_m__actor_state__

#include "game.h"
//#include "observer.h"

class Subject;
class BaseActor;

// various states
enum e_actorStates
{
    ACTOR_IDLE,
    ACTOR_FALLING,
    ACTOR_JUMPING,
    ACTOR_MOVING,
    ACTOR_HOVER,
    ACTOR_HIT,
    ACTOR_DEFEND,
    ACTOR_CALLHELP,
    ACTOR_STUNNED,
    ACTOR_ATTACK,
    ACTOR_DYING,
    ACTOR_VICTORY,
    ACTOR_CONSTRICTED,
    ACTOR_TARGET
};

// state events
enum Event
{
    EVENT_FALLING,
    EVENT_JUMPING,
    EVENT_IS_IDLE,
    EVENT_TARGET_ACTIVE,
    EVENT_TARGET_LOST,
    EVENT_START_MOVING,
    EVENT_START_ATTACK,
    EVENT_PLAYER_START_ATTACK,
    EVENT_IS_HIT,
    EVENT_WAS_HURT,
    EVENT_ATTACK_STARTED,
    EVENT_ATTACK_FINISHED,
    EVENT_MOVEMENT_FINISHED,
    EVENT_ANIMATION_FINISHED,
    EVENT_STATE_TIMER_UP,
    EVENT_GAME_PAUSE
};

/**
 *  \brief Base state interface.
 *
 * An actor will have many states throughout its life. Depending on which state it is in, the actor will
 * behave differently. The different virtual functions of each derived ActorState objects will be used to 
 * call the actual functions of the actor. When each state loads it consume different data from the actor, 
 * The state uses the condition of that data to update the actor. A state enable a specific update or 
 * disable certain behaviors for the actor.
 */
class ActorState //: public Observer
{
public:
    //typedef CIwList<Condition*>::iterator _Iterator;
    
	ActorState(BaseActor* actor,
               unsigned long id = 0,
               CIwGameAnimTimeline* state_timeline = NULL,
               uint64 next_state_update = 0);
    
    virtual ~ActorState();
    
    virtual void Load();            //load the timeline; load certain condition to check against
	virtual void Close();           //set velocity to 0; set all conditions to false
	
	virtual void Update();
	virtual void Render(){}
    
    
    BaseActor*              GetActor() { return m_actor; }
    unsigned long           GetID() { return m_id; }
    void                    SetID(unsigned long id) { m_id = id; }
    void                    SetStateTimeline(CIwGameAnimTimeline* timeline);
    void                    SetStateTimeline(CIwGameAnimTimeline* timeline, bool load = false);
    CIwGameAnimTimeline*    GetStateTimeline();
    
    void                    SetNextStateUpdate(const uint64 next_update);
    bool                    IsTimeForStateUpdate() const;
    
    // default base state checks
    virtual bool CanAttack() const;
    virtual bool CanBeHurt() const;
    virtual bool CanMove() const;
    virtual bool IsAttackAnimationPlaying() const;
    
    // observer
    //void OnNotify(BaseActor* actor, Event event);
    
protected:
    BaseActor*              m_actor;                /*<< link to the actor that currently has this state. */
    CIwGameAnimTimeline*    m_stateTimeline;        /*<< the timeline animation to play while in this state. */
    CIwGameTimer            m_stateTimer;           /*<< a timer for the state. */
    uint64                  m_NextStateUpdate;      /*<< the time for the next update. */
    
private:
    unsigned long           m_id;
    
};

#endif /* defined(__dragon_drop_m__actor_state__) */
