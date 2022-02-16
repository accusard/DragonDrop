//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// ball_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  BallActor class interface
//--------------------------------------------------------------------------------------------------------

#if !defined (_BALL_ACTOR_H_)
#define _BALL_ACTOR_H_

#include "stone_actor.h"
#include "utility_actor.h"
#include "game_object_actor.h"

/**
 * \brief
 */
class SpikedBallActor : public GameObjectActor
{
public:
    bool LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    void ResolveCollisions();
    
    bool Respawn();
    void NotifyCollisionWithSword(CIwGameActor *sword);
    
};


class SpikedBallActorCreator : public IIwGameXomlClassCreator
{
public:
    SpikedBallActorCreator()
    {
        setClassName("spikedballactor");
    }
    
    IIwGameXomlResource* CreateInstance(IIwGameXomlResource * parent)
    {
        return new SpikedBallActor();
    }
};
#endif
