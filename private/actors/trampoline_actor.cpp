//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// trampoline_actor.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// TrampolineActor class implementations
//--------------------------------------------------------------------------------------------------------

#include "trampoline_actor.h"

TrampolineActor::TrampolineActor() : GameObjectActor()
{
    
}

TrampolineActor::~TrampolineActor()
{

}

bool TrampolineActor::Update(float dt)
{
    if(isDestroyed())
        return false;
    
	return GameObjectActor::Update(dt);
}

void TrampolineActor::ResolveCollisions()
{
    //a list of collidables
	CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();
    
	for(int t = 0; t < started.getSize(); t++)
	{
		CIwGameBox2dCollidable* collision = started.element_at(t);
		if(collision != NULL)
		{
			CollidableActor* actor = (CollidableActor*)collision->getUserData();
			if(actor != NULL)
                actor->NotifyCollisionWithTrampoline(this);
        }
    }
}

void TrampolineActor::NotifyCollision(CIwGameActor* other)
{
    GameObjectActor::NotifyCollision(other);
    
    SetBouncy(0.1f);
    
	if(m_isBouncy && Landed)
	{
		if(getAngle() > OriginalAngle + 5.0f || getAngle() < OriginalAngle - 5.0f)
		{
			Landed = false;
			SetBouncy(OriginalRestitution,false);
			setType(OriginalType);
		}
	}
}

/**
 *  \brief
 *  TODO: have not been implemented
 */
void TrampolineActor::NotifyCollisionWithBomb(CollidableActor* bomb_actor)
{
    
}

bool TrampolineActor::LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode* node)
{
    if(!CollidableActor::LoadFromXoml(parent, load_children, node))
        return false;
    
    return true;
}

// object effects
void TrampolineActor::Explode()
{
    
}
