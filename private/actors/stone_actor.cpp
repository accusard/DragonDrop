//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// stone_actor.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  HeavyObjectActor class interface
//--------------------------------------------------------------------------------------------------------

#include "stone_actor.h"
#include "bomb_actor.h"
#include "visual_effect.h"

bool HeavyObjectActor::Update(float dt)
{
    if( !GameObjectActor::Update(dt) )
        return false;
    
    if(	Box2dBody->getBody()->GetLinearVelocity().y >= 3.0f  )
        SetLanded(false);
    
    return true;
}

/**
 *  \brief This function goes through a list of collidables and notify the other actor it has collided with this
 */
void HeavyObjectActor::ResolveCollisions()
{
    CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();
    
	for(int t = 0; t < started.getSize(); t++)
	{
		CIwGameBox2dCollidable* collision = started.element_at(t);
		if(collision != NULL)
		{
			CollidableActor* actor = (CollidableActor*)collision->getUserData();
			if(actor != NULL)
                actor->NotifyCollisionWithHeavyObject(this);
        }
    }
}

/**
 *  \brief Notify this actor that it has collided with a bomb.
 *  \param bomb_actor The bomb that this actor collided with.
 */
void HeavyObjectActor::NotifyCollisionWithBomb(CollidableActor* bomb_actor)
{
    // to ensure the stone crushes and sizzle out the bomb from above
    int is_located = FindActorOrientationTo(bomb_actor, ActorOrientation::Y_AXIS);
    if(is_located == ActorOrientation::ABOVE)
    {
        CIwFVec2 vel = getVelocity();
        vel.y = 0.01;
        setVelocity(vel.x, vel.y);
         
    }
    
    GameObjectActor::NotifyCollision(bomb_actor);
}

//--------------------------------------------------------------------------------------------------------
// StoneActor class implementation
//--------------------------------------------------------------------------------------------------------

StoneActor::StoneActor() : HeavyObjectActor()
{
    isDestructible = true;
}

bool StoneActor::Update(float dt)
{
    if( !HeavyObjectActor::Update(dt) )
        return false;
    
    return true;
}

bool StoneActor::UpdateVisual()
{
    return CIwGameActorImage::UpdateVisual();
}

/**
 *  \brief This function goes through a list of collidables and notify the other actor it has collided with this
 */
void StoneActor::ResolveCollisions()
{
    CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();
    
	for(int t = 0; t < started.getSize(); t++)
	{
		CIwGameBox2dCollidable* collision = started.element_at(t);
		if(collision != NULL)
		{
			CollidableActor* actor = (CollidableActor*)collision->getUserData();
			if(actor != NULL)
                actor->NotifyCollisionWithStone(this);
        }
    }
}

/**
 *  \brief Notify this actor that it has collided with an explosion.
 *  \param explosion The explosion that this actor collided with.
 */
void StoneActor::NotifyCollisionWithExplosion(CollidableActor* explosion)
{
    GameObjectActor::NotifyCollision(explosion);
    Explode();
}

void StoneActor::NotifyCollisionWithTrampoline(CollidableActor *trampoline)
{
    GameObjectActor::NotifyCollision(trampoline);
    
    int is_located;
    
    is_located = FindActorOrientationTo(trampoline, ActorOrientation::Y_AXIS);
    
    if(is_located == ActorOrientation::ABOVE)
    {
        GameObjectActor*    gameobject  = (GameObjectActor*)trampoline;
        
        if(gameobject->IsBouncy())
        {
            gameobject->SetLanded(true);
            CIwFVec2 vel = this->getVelocity();
            vel.y = -10;
            this->setVelocity(vel.x, vel.y);
        }
    }
}
/**
 *  \brief Notify this actor that it has collided with a stone.
 *  \param stone_actor The stone that this actor collided with.
 */
void StoneActor::NotifyCollisionWithStone(CollidableActor* stone_actor)
{
    GameObjectActor::NotifyCollision(stone_actor);
}

/**
 *  \brief Notify this actor that it has collided with a platform.
 *  \param platform_actor The platform that this actor collided with.
 */
void StoneActor::NotifyCollisionWithPlatform(CollidableActor* platform_actor)
{
    GameObjectActor::NotifyCollision(platform_actor);
}

/**
 *  \brief Notify this actor that it has collided with the player
 *  \param player The player that this actor collided with.
 */
void StoneActor::NotifyCollisionWithPlayer(CollidableActor* player)
{
    int stone_is_located = FindActorOrientationTo(player, ActorOrientation::Y_AXIS);

    if(HasLanded() || stone_is_located == ActorOrientation::BELOW)
        return;
    
    VisualEffect effect;
    
    effect.SliceActor(this, 5, 5);
    
    setDestroyed(true);
}

/**
 *  \brief Notify this actor that it has collided with the a particle
 *  \param p The fire_breath that this actor collided with.
 */
void StoneActor::NotifyCollisionWithParticles(CollidableParticles* p)
{
    CIwGameError::LogError("Collided with Particles.");
    setDestroyed(true);
}
