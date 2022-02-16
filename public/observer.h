//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// observer.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  The Observer class interface - notify an actor that an interesting event has occured
//--------------------------------------------------------------------------------------------------------

#if !defined(_OBSERVER_H_)
#define _OBSERVER_H_

#include "IwGame.h"
#include "game.h"
#include "actor_state.h"

class BaseActor;

/**
 * \brief This interface class is used to communicate between an object being observed and to notify it of
 * an event that has transpired
 *
 *  A class derived from this is notified of the event
 */
class Observer
{
    friend class Subject;

    
public:
    
    Observer();
    virtual ~Observer();
    
    // notify an actor of the event
    virtual void OnNotify(BaseActor* actor, Event event) = 0;
    
private:
    Observer* m_next;
    
};

/**
 * \brief This class is used to observe an object to see if anything of interest occur and if it
 * does will send notifications to the observer objects. Observers are objects in multiple location that
 * need to be aware of an event.
 */
class Subject
{
    
public:
    
    Subject() {}
    ~Subject() {}
    
    void AddObserver(Observer* observer);
    void RemoveObserver(Observer* observer);
    
protected:
    void Notify(BaseActor* actor, Event event);
    
private:
    Observer* m_head;
    //CIwList<Observer*> m_observerList;
};




#endif
