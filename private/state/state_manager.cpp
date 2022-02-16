//
//  state_manager.cpp
//  dragon_drop
//
//  Created by Vanny Sou on 4/27/15.
//
//

#include "state_manager.h"
#include "base_actor.h"
#include "attack_state.h"
#include "hit_state.h"

StateManager::StateManager() : m_currentState(NULL), m_queueState(NULL), m_prevStateID(-1) {}

StateManager::~StateManager()
{
    if(m_currentState != NULL)
        m_currentState->Close();
    
    ClearOldStates();
}

// TODO: #D1204 may not need the bool change flag
void StateManager::AddState(ActorState* state, bool change)
{
    m_states.push_back(state);
    
    if(change == false)
        return;
    
    if(m_currentState != NULL)
    {
        // store previous actor state
        m_prevStateID = m_currentState->GetID();
        m_currentState->Close();
        //m_oldStates.push_back(m_currentState);
    }
    
    m_currentState = m_states.back();
    m_currentState->Load();
}

void StateManager::Update()
{
    if(m_currentState != NULL)
        m_currentState->Update();
    
    /*
    if(m_currentState->IsTimeForStateUpdate() && m_queueState != -1)
    {
        ChangeState(m_queueState);
        m_queueState = - 1;
    }
     */
}
/**
 * \brief Change the state of the actor.
 *
 * The function will go through a list of states that the actor currently have and changes to the specified
 * state. A duration can be set in the parameter to have the state pause before going into its update loop.
 *
 * \param change_to_id The id of the state that this actor should change to.
 * \param next_state_update Set a duration for the next state update for the state that we are changing to.
 * \return true if the change to state exist in the list; false if it does not exist
 */
bool StateManager::ChangeState(unsigned long change_to_id, uint64 next_state_update)
{
    unsigned int current_id = m_currentState->GetID();
    
    if(current_id == change_to_id)
        return true;
    
    // go through actor states and find the state to change to
    for(_Iterator it = m_states.begin(); it != m_states.end(); ++it)
    {
        if((*it) != NULL && (*it)->GetID() == change_to_id)
        {
            /*
            // ready for state change?
            if(!m_currentState->IsTimeForStateUpdate())
            {
                m_queueState = change_to_id;
                return false;
            }
            */
            // store previous actor state
            m_prevStateID = m_currentState->GetID();
            
            if(m_currentState != NULL)
                m_currentState->Close();
            
            m_currentState = *it;
            
            m_currentState->SetNextStateUpdate(next_state_update);
            
            m_currentState->Load();
            
            return true;
        }
    }
    return false;
}

void StateManager::ClearOldStates()
{
    
    for(_Iterator it = m_states.begin(); it != m_states.end(); ++it)
    {
        delete *it;
    }
    m_states.clear();
}

/**
 * \brief Access to the current state of the actor.
 * \return m_currentState A pointer to the current state of the actor.
 */
ActorState* StateManager::GetCurrentState()
{
    return m_currentState;
}

/**
 * \brief Remove a specific state of the actor.
 * \param state The pointer to the state to be removed and deleted.
 */
void StateManager::RemoveState(ActorState* state)
{
    if(state == NULL)
        return;
    
    for(_Iterator it = m_states.begin(); it != m_states.end(); ++it)
    {
        if((*it) == state)
        {
            m_states.erase(it);
            delete *it;
            return;
        }
    }
}

CIwGameString StateManager::GetCurrentStateStr()
{
    CIwGameString string;
    int _id = GetCurrentState()->GetID();
    
    switch(_id)
    {
        case ACTOR_IDLE:
        {
            string = "idle";
            break;
        }
        case ACTOR_FALLING:
        {
            string = "falling";
            break;
        }
        case ACTOR_JUMPING:
        {
            string = "jumping";
            break;
        }
        case ACTOR_MOVING:
        {
            string = "moving";
            break;
        }
        case ACTOR_HOVER:
        {
            string = "hovering";
            break;
        }
        case ACTOR_HIT:
        {
            string = "hit";
            break;
        }
        case ACTOR_DEFEND:
        {
            string = "defend";
            break;
        }
        case ACTOR_CALLHELP:
        {
            string = "callhelp";
            break;
        }
        case ACTOR_STUNNED:
        {
            string = "stunned";
            break;
        }
        case ACTOR_ATTACK:
        {
            string = "attack";
            break;
        }
        case ACTOR_DYING:
        {
            string = "dying";
            break;
        }
        case ACTOR_VICTORY:
        {
            string = "victory";
            break;
        }
        case ACTOR_TARGET:
        {
            string = "targeting";
            break;
        }
        default:
        {
            string = _id;
            break;
        }
    }
    
    return string;
}

/**
 * \brief Find the state of the actor as specified by the id
 * \param id The id of the state to find.
 * \return Returns the pointer to the state of the actor or NULL if the state specified does not exist.
 */
ActorState* StateManager::FindState(unsigned long id)
{
    unsigned int _id = id;
    
    for(_Iterator it = m_states.begin(); it != m_states.end(); ++it)
    {
        if((*it)->GetID() == _id);
        {
            return *it;
        }
    }
    return NULL;
}

void StateManager::ClearAttackStates()
{
    for(_Iterator it = m_states.begin(); it != m_states.end(); ++it)
    {
        if( (*it) != NULL && (*it)->GetID() == ACTOR_ATTACK )
        {
            delete *it;
            *it = NULL;
        }
    }
}

/**
 * \brief Notify the actor of the specified event and change state accordingly.
 * \param actor the target of this notification
 * \param event The occuring event state
 */
void StateManager::OnNotify(BaseActor* actor, Event event)
{
    switch(event)
    {
            // TODO: #S4866 pass target of attack as an argument instead of hard-code
        case EVENT_START_ATTACK:
        {
            ClearAttackStates();
            CIwGameActor* target = NULL;
            CIwGameAnimTimeline* attack_timeline = actor->GetAnimate().Attack;
            
            if(actor->getType() == ENEMY)
                target = actor->getScene()->findActor(PLAYER);
            
            AddState(new class Attack(actor, target, attack_timeline, 1000), true);
            break;
        }
            // TODO: #N3894 does not load moving animation yet
        case EVENT_START_MOVING:
        {
            ChangeState(ACTOR_MOVING);
            break;
        }
            
        case EVENT_ATTACK_FINISHED:
        {
            if (!ChangeState(ACTOR_TARGET) )
            {
                ChangeState(ACTOR_IDLE);
            }
            
            break;
        }
            
        case EVENT_TARGET_LOST:
        {
            ChangeState(ACTOR_TARGET, 1000);
            break;
        }
            
        case EVENT_FALLING:
        {
            ChangeState(ACTOR_FALLING);
            break;
        }
            
        case EVENT_IS_IDLE:
        {
            ChangeState(ACTOR_IDLE);
            break;
        }
            
        case EVENT_JUMPING:
        {
            ChangeState(ACTOR_JUMPING);
            break;
        }
            
        case EVENT_IS_HIT:
        {
            if(!ChangeState(ACTOR_HIT))
            {
                // locate the hurt timeline and set it to apply to this actor
                CIwGameAnimTimeline* hurt_timeline = (CIwGameAnimTimeline*)IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->findTimeline("blink_red");
                
                actor->setTimeline(hurt_timeline);
                hurt_timeline->restart();
                
                ActorState* hit_state = new Hit(actor, hurt_timeline, 1000);
                
                AddState(hit_state, true);
            }
            break;
        }
            
        case EVENT_MOVEMENT_FINISHED:
        {
            break;
        }
            
        case EVENT_ANIMATION_FINISHED:
        {
            break;
        }
            
        case EVENT_STATE_TIMER_UP:
        {
            break;
        }
    }
    
    // notify the actor and act on it
    actor->ActionsOnNotify(event);
}

void StateManager::SetQueueState(ActorState* state)
{
    m_queueState = state;
}