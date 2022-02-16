//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// collectable_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Actors for collectable pickups in the game
//--------------------------------------------------------------------------------------------------------

#if !defined(_COLLECT_ACTOR_H_)
#define _COLLECT_ACTOR_H_

#include "IwGame.h"
#include "game.h"
#include "player_actor.h"
#include "debug_actor_text.h"

//
//
//
//  Base collectable actor class
//
//
//

class CollectableActor : public CollidableActor
{
    using CIwGameActorImage::Init;
    
public:

protected:
    int                 m_identifier;       /**< An ID that is used by Game to identify whether this collectable have been collected.  */
    bool				m_holdToCollect;	/**< If true, this object will be talllied up with the total number collected at the end of the level */
	bool				Collected;			/**< The collected state of this item */
	int					Value;				// amount to collect and add
    HUDTextDisplay*     HUDText;            // a link to the HUD text display
    
public:
	CollectableActor();
    ~CollectableActor();

    void            Init(int _id);
	bool			Update(float dt);
	void			ResolveCollisions();
    void            NotifyCollisionWithCollectable(CollidableActor* collectable_actor);
	bool			LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode* node);
	
	//Specific member functions
	void			SetCollected(bool collected = true);
	bool			HasCollected() const { return Collected; }
    void            SetHoldToCollect(bool hold);
    bool            HoldToCollect() const;
	int				getValue() const { return Value; }
    int             GetID() const;
    void            SetID(int _id);
    int             Collect();
    
};

class CollectableActorCreator : public IIwGameXomlClassCreator
{
public:
	CollectableActorCreator()
	{
		setClassName("collectableactor");
	}

	IIwGameXomlResource* CreateInstance(IIwGameXomlResource *parent)
	{
		return new CollectableActor();
	}
};

//
//
//
//  MapActor class - collectable map fragments
//
//
//

class MapActor : public CollectableActor
{
private:
    
public:
    MapActor() : CollectableActor()
    {
        //setActualClassType("map");
        setType(MAP);
    }
    void NotifyCollisionWithCollectable(CollidableActor* collectable_actor);
    
};

class MapActorCreator : public IIwGameXomlClassCreator
{
public:
    MapActorCreator() { setClassName("mapactor"); }
    IIwGameXomlResource*    CreateInstance(IIwGameXomlResource* parent)
    {
        return new MapActor();
    }
};

//
//
//
//  OrbActor class - collectable map fragments
//
//
//

class OrbActor : public CollectableActor
{
private:
    
public:
    OrbActor() : CollectableActor()
    {
        //setActualClassType("map");
        setType(ORB);
    }
    void NotifyCollision(CIwGameActor* other);
    
};

class OrbActorCreator : public IIwGameXomlClassCreator
{
public:
    OrbActorCreator() { setClassName("orbactor"); }
    IIwGameXomlResource*    CreateInstance(IIwGameXomlResource* parent)
    {
        return new OrbActor();
    }
};

#endif
