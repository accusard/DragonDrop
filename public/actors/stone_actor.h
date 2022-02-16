//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// stone_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#if !defined(_STONE_ACTOR_H_)
#define _STONE_ACTOR_H_

#include "game_object_actor.h"

//--------------------------------------------------------------------------------------------------------
//  HeavyObjectActor class interface
//--------------------------------------------------------------------------------------------------------
class HeavyObjectActor : public GameObjectActor
{
public:
    bool Update(float dt);
    
    // collisions
    void ResolveCollisions();
    void NotifyCollisionWithBomb(CollidableActor* bomb_actor);
    
private:
    
};

//--------------------------------------------------------------------------------------------------------
//  StoneActor class interface
//--------------------------------------------------------------------------------------------------------
/**
 *  \brief A stone object
 */
class StoneActor : public HeavyObjectActor
{
    
public:
    // constructor
    StoneActor();
    
    bool    Update(float dt);
    bool    UpdateVisual();
    
    // collisions
    void    ResolveCollisions();
    void    NotifyCollisionWithExplosion(CollidableActor *actor);
    void    NotifyCollisionWithTrampoline(CollidableActor *actor);
    void    NotifyCollisionWithStone(CollidableActor* stone_actor);
    void    NotifyCollisionWithPlatform(CollidableActor* platform_actor);
    void    NotifyCollisionWithPlayer(CollidableActor* player);
    void    NotifyCollisionWithParticles(CollidableParticles* fire_breath);
    
private:
    
};

class StoneActorCreator : public IIwGameXomlClassCreator
{
public:
    StoneActorCreator()
    {
        setClassName("stoneactor");
    }
    
    IIwGameXomlResource* CreateInstance(IIwGameXomlResource *parent)
    {
        return new StoneActor();
    }
};

#endif
