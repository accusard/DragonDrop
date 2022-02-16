//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// trampoline_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// TrampolineActor interface
//--------------------------------------------------------------------------------------------------------

#if !defined(_TRAMPOLINE_ACTOR_H_)
#define _TRAMPOLINE_ACTOR_H_

#include "game_object_actor.h"

class TrampolineActor : public GameObjectActor
{
    
public:
	TrampolineActor();
	~TrampolineActor();
    
	bool Update(float dt);
    bool LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode* node);
    
    // collisions
    void ResolveCollisions();
    void NotifyCollision(CIwGameActor* other);
    void NotifyCollisionWithBomb(CollidableActor* bomb_actor);

    // object effects
    void Explode();
    
protected:
    
};

class TrampolineActorCreator : public IIwGameXomlClassCreator
{
public:
    TrampolineActorCreator()
    {
        setClassName("TrampolineActor");
    }
    
    IIwGameXomlResource* CreateInstance(IIwGameXomlResource *parent)
    {
        return new TrampolineActor();
    }
};

#endif
