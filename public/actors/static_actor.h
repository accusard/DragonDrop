// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// static_actor.h
//	Programmed by: Vanny Sou Copyright 2018-2022
//
//--------------------------------------------------------------------------------------------------------

#if !defined(_STATIC_ACTOR_H_)
#define _STATIC_ACTOR_H_

#include "IwGame.h"
#include "game.h"
#include "scene.h"
#include "utility_actor.h"

/**
 *  \brief A class for static fixtures and backgrounds
 */
class StaticActor : public CollidableActor
{
//member functions from base class
public:
	bool			Update(float dt);
    bool			UpdateVisual();
	//void			ResolveCollisions();
    void			NotifyCollision(CIwGameActor* other);
	bool			LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode* node);

protected:
    bool                            ResizeFixture;
    bool                            MaintainSizeDepth;

public:
	StaticActor();
    ~StaticActor() {}

	void					OnBeginTouch(int index, int x, int y, bool allow_bubble);
    
    void                    ResizeActorBox2dShape(int width, int height);   // function provide visual aid to the Box2d Collision shape
    void                    ShowVisualCollision();                               // function provide visual aid to the Box2d Collision shape
    bool                    IsOffScreen();
    bool                    GetMaintainSizeDepth() const;
};

class StaticActorCreator : public IIwGameXomlClassCreator
{
public:
	StaticActorCreator()
	{
		setClassName("staticactor");
	}

	IIwGameXomlResource* CreateInstance(IIwGameXomlResource *parent)
	{
		return new StaticActor();
	}
};
#endif
