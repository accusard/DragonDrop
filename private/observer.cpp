//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// ball_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  The Observer and Subject class implementations
//--------------------------------------------------------------------------------------------------------


#include "observer.h"
#include "movement.h"

Observer::Observer()
{}

Observer::~Observer() {}

/**
 * \brief Add an observer to the front of the linked list.
 * \param observer The observer to be added.
 */
void Subject::AddObserver(Observer *observer)
{
    //m_observerList.push_back(observer);
    
    observer->m_next = m_head;
    m_head = observer;
     
}

/**
 * \brief Remove an observer from the list.
 * \brief Observer The pointer to the observer being removed.
 */
void Subject::RemoveObserver(Observer *observer)
{
    
    if(m_head == observer)
    {
        m_head = observer->m_next;
        observer->m_next = NULL;
        return;
    }
    
    Observer* current = m_head;
    while(current != NULL)
    {
        if(current->m_next == observer)
        {
            m_head = observer->m_next;
            observer->m_next = NULL;
            return;
        }
        current = current->m_next;
    }
    
}

/**
 * \brief Go through the list of linked observer and notify any interested prospector of the event
 * that has transpired.
 *
 * The list will start with the recent observer that was added and move towards the older observers.
 *
 * \param actor A pointer to the CIwGameActor object.
 * \param event The event that has occured.
 */
void Subject::Notify(BaseActor *actor, Event event)
{
    /*
    for(_observerIt it = begin(); it != end(); ++it)
    {
        (*it)->OnNotify(actor,event);
    }
    */
    
    Observer* observer = m_head;
    while(observer != NULL)
    {
        observer->OnNotify(actor, event);
        observer = observer->m_next;
    }
     
}


