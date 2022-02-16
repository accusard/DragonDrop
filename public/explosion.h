//--------------------------------------------------------------------------------------------------------
// Dragon Drop
//	explosion.h
// Programmed by: 
//	Vanny Sou
//
//--------------------------------------------------------------------------------------------------------
#if !defined (_EXPLOSION_H_)
#define _EXPLOSION_H_

#include "IwGame.h"
#include "IwGameUtil.h"
#include "game.h"
#include "actors/static_actor.h"
#include "utility_actor.h"

class Explosion : public CollidableActor
{
    using CIwGameActorImage::Init;
    
private:
	float			Radius;			// radius of the explosion
	
	//member functions from base class
public:
	bool			Update(float dt);
    
    // collisoins
    void            ResolveCollisions();
	void			NotifyCollision(CIwGameActor* other);
    void            NotifyCollisionWithStone(CollidableActor *actor);
    void            NotifyCollisionWithBomb(CollidableActor *actor);

	//class specific functions
public:
	void			Init(float r);                          // initializes an explosion object
	void			SetRadius(float r);						// sets the size of the explosion radius
	float			GetRadius() const { return Radius; }	// get the radius
};

#endif