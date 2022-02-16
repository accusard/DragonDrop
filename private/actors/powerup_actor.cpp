//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// powerup_actor.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#include "powerup_actor.h"

PowerUpActor::PowerUpActor()
{
    //ActualClassType will be used in an iterator to identify all objects of this type
    setActualClassType("powerup");
    
    ActionBar = (ButtonIcon*)GAME->findScene("HUD")->findActor("ActionBar");
}

bool PowerUpActor::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
    if (!CollectableActor::LoadFromXoml(parent, load_children, node))
        return false;
    
    // convert value from XOML string to hash value
	unsigned int collection_value_hash = CIwGameString::CalculateHash("value");
	unsigned int object_type_hash = CIwGameString::CalculateHash("type");
    
	for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();
		if (name_hash == collection_value_hash)
		{
			int value = (*it)->GetValueAsInt();
			this->Value = value;
		}
		else
            if (name_hash == object_type_hash)
            {
                CIwGameString type = (*it)->GetValue();
                if(type == "sword")
                {
                    setType(SWORD);
                }
                if(type == "bubble")
                {
                    setType(BUBBLE);
                }
                if(type == "spring")
                {
                    setType(SPRING);
                }
                if(type == "umbrella")
                {
                    setType(UMBRELLA);
                }
            }
	}
	return true;
}

void PowerUpActor::ResolveCollisions()
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
                actor->NotifyCollisionWithPowerup(this);
        }
    }
}

void PowerUpActor::NotifyCollisionWithPlayer(CollidableActor* actor)
{
    setActive(false);
}
