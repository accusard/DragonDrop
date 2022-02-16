//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// ball_actor.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// SpikedBall class that players can interact with
//--------------------------------------------------------------------------------------------------------

#include "ball_actor.h"

bool SpikedBallActor::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
    return GameObjectActor::LoadFromXoml(parent, load_children, node);
}

void SpikedBallActor::ResolveCollisions()
{
    if(Box2dBody != NULL)
    {
        CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();
        
        for(int t = 0; t < started.getSize(); t++)
        {
            CIwGameBox2dCollidable* collision = started.element_at(t);
            if(collision != NULL)
            {
                CollidableActor* actor = (CollidableActor*)collision->getUserData();
                if(actor != NULL)
                    actor->NotifyCollisionWithSpikedBall(this);
            }
        }
    }
}

void SpikedBallActor::NotifyCollisionWithSword(CIwGameActor *sword)
{
    // let Box2d gravity take over if it has not already
    if(Box2dBody == NULL)
    {
        Box2dPhysicsUti b2dutil;
        b2dutil.AssignBox2dPhysics(*this, "Placeholder_Shape", "Heavy", false);
        return;
    }
}

bool SpikedBallActor::Respawn()
{
    RemoveBody();
    return GameObjectActor::Respawn();
}
