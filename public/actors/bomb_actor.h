//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// game_object_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  BombActor class interface
//--------------------------------------------------------------------------------------------------------

#if !defined(_BOMB_ACTOR_H_)
#define _BOMB_ACTOR_H_

#include "game_object_actor.h"
#include "game_timer.h"

/**
 *  \brief A bomb that can detonate and destroy other objects around it after a certain tick
 */
class BombActor : public GameObjectActor
{
    
public:
    // constructor
    BombActor();
    
    void    SetCountdownTimer(uint64 time);
    void    Explode();
    void    SizzleOut(int num_particles, const char* brush_name,
                      float x, float y, float delay);
    
    GameTimer* GetObjectTimer();
    
    bool    Update(float dt);
    
    // collisions
    void    ResolveCollisions();
    void    NotifyCollision(CIwGameActor* other);
    void    NotifyCollisionWithStone(CollidableActor* other);
    
private:
    //uint64          m_lastCountdown;    /**<  */
    GameTimer       m_countdownTimer;   /**< A timer used for the bomb's tick countdown */
    float           m_explosionRadius;  /**< The radius of the explosion */
    
};

class BombActorCreator : public IIwGameXomlClassCreator
{
public:
    BombActorCreator()
    {
        setClassName("bombactor");
    }
    
    IIwGameXomlResource* CreateInstance(IIwGameXomlResource *parent)
    {
        return new BombActor();
    }
};

#endif
