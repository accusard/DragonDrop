//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// powerup_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------

#if !defined(_POWERUP_ACTOR_H_)
#define _POWERUP_ACTOR_H_

#include "collectable_actor.h"
#include "button_icon.h"

/**
 * \brief PowerUpActor class that can be activated through an action bar to change the player's behavior.
 */
class PowerUpActor : public CollectableActor
{
private:
    ButtonIcon*     ActionBar;
    
public:
    PowerUpActor();
    
    bool LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    
    // collisions
    void ResolveCollisions();
    void NotifyCollisionWithPlayer(CollidableActor* player);
    
    void ActivatePowerup();
    
};

class PowerUpActorCreator : public IIwGameXomlClassCreator
{
public:
    PowerUpActorCreator() { setClassName("powerupactor"); }
    
    IIwGameXomlResource*    CreateInstance(IIwGameXomlResource* parent)
    {
        return new PowerUpActor();
    }
};

#endif
