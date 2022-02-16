//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// state_manager.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  StateManager Class
//--------------------------------------------------------------------------------------------------------

#ifndef __dragon_drop__state_manager__
#define __dragon_drop__state_manager__

#include "actor_state.h"
#include "observer.h"

class StateManager : public Observer
{
public:
    typedef CIwList<ActorState*>::iterator _Iterator;
    
    StateManager();
    ~StateManager();
    
    void            Update();
    void            AddState(ActorState* state, bool change);
    bool            ChangeState(unsigned long id, uint64 next_state_update = 0);
    void            RemoveState(ActorState* state);
    ActorState*     GetCurrentState();
    CIwGameString   GetCurrentStateStr();
    unsigned long   GetPrevState() {return m_prevStateID;}
    ActorState*     FindState(unsigned long id);
    void            ClearAttackStates();
    void            ClearOldStates();
    void            OnNotify(BaseActor* actor, Event event);
    void            SetQueueState(ActorState* state);
    
private:
    CIwList<ActorState*>    m_states;
    ActorState*             m_currentState;
    ActorState*             m_queueState;
    //unsigned long           m_queueState;              /*<< State queue. Will change to this once timer expire*/
    unsigned long           m_prevStateID;            /*<< The id of the previous state of the actor. */
};

#endif /* defined(__dragon_drop__state_manager__) */
