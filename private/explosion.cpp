//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// explosion.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// Game Object class that players can interact with
//--------------------------------------------------------------------------------------------------------

#include "explosion.h"
#include "game_object_actor.h"
#include "sensor_actor.h"

void Explosion::Init(float radius)
{
	SetRadius(radius);

    //create the struct
	CIwGameGeomShapeCircle*		circle = new CIwGameGeomShapeCircle();
    
    //create new shape class to hold the struct
	CIwGameShape*				shape = new CIwGameShape();
    
	CIwGameBox2dMaterial*		material = (CIwGameBox2dMaterial*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource("Kinematic_Mat", CIwGameXomlNames::Box2dMaterial_Hash);
	CIwGameBrushImage*			brushimage = (CIwGameBrushImage*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource("Brush_Explosion", CIwGameXomlNames::Brush_Hash);
	
    //define the shape of the struct
	circle->Radius = Radius;
    
    //set class with new shape
	shape->setShape(circle);

	if(material == NULL)
		CIwGameError::LogError("Could not find material");
	if(brushimage == NULL)
		CIwGameError::LogError("Could not find image");
	
	if(brushimage != NULL)
		CIwGameActorImage::Init(brushimage,(int)Radius*2,(int)Radius*2);

	//Box2d properties
	Angle = -Angle;
	Box2dBody = new CIwGameBox2dBody();
	Box2dBody->setBodyShape(shape);
	Box2dBody->InitBody(Scene->getBox2dWorld(), shape, material, &Position, Angle, 0, 0);
	VelocityDamping.x = 0;
	VelocityDamping.y = 0;
	AngularVelocityDamping = 0;
	setVelocityDamping(VelocityDamping.x, VelocityDamping.y);
	setAngularVelocityDamping(AngularVelocityDamping);
	Box2dBody->setCollisionFlags(1,1,1);
	Box2dBody->setUserData(this);

	// Actor attributes
	setName("Explosion");
	setDraggable(false);
	setType(EXPLOSION);
	setLayer(L_ACTORS);
	setCollidable(true);

	SAFE_DELETE(shape);
	//SAFE_DELETE(circle);
}

void Explosion::SetRadius(float r)
{
	Radius = r;
}

void Explosion::ResolveCollisions()
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
                actor->NotifyCollisionWithExplosion(this);
        }
    }
}

void Explosion::NotifyCollision(CIwGameActor* other)
{
    setDestroyed(true);

	if(other->getType() == PLAYER || other->getType() == ENEMY)		
	{
        if(((BaseActor*)other)->GetBaseSensor() != NULL)
            ((BaseActor*)other)->GetBaseSensor()->setActive(false);
        
		((BaseActor*)other)->ApplyImpactForce(IMPACT_FORCE);
		((BaseActor*)other)->setDying();
        
        return;
	}

}

void Explosion::NotifyCollisionWithStone(CollidableActor *actor)
{
    Explosion::NotifyCollision(actor);
}

void Explosion::NotifyCollisionWithBomb(CollidableActor *actor)
{
    Explosion::NotifyCollision(actor);
}

bool Explosion::Update(float dt)
{
    if(!CollidableActor::Update(dt))
        return false;
    
    return true;
}
