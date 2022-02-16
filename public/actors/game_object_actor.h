//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// game_object_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Base class for the GameObjectActor
//--------------------------------------------------------------------------------------------------------
#if !defined(_OBJECT_ACTOR_H_)
#define _OBJECT_ACTOR_H_

#include "IwGame.h"
#include "IwGameUtil.h"
#include "static_actor.h"
#include "base_actor.h"

class GameObjectActor : public CollidableActor, public BurnableActor
{
    using CIwGameActorImage::Init;
    
protected:
	bool			isDestructible;			// The destructible state of the object
	bool			isLightSource;			// Provide source of light
	bool			m_isBouncy;				// Used to set box2d physics restitution
	bool			isHeavy;				// Used to determine the gravity scale of the object
	int				AttackPower;			// The amount of damage this object actor does to other objects
	bool			Landed;					// Object collided with foreground platforms
    
    float           OriginalRestitution;    /**< Save object's original restitution */
    int             OriginalType;           /**< Save object's original type */
    bool            m_canHurt;              /**< This object can hurt the player or enemy */


public:
	GameObjectActor(bool destructible = false, int power = 0);
	~GameObjectActor();

    bool Init(IIwGameBrush *image, int width, int height, ObjectData& data);
    
	bool Update(float dt);
	bool UpdateVisual();
    bool LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode* node);
    
    // collisions
    void NotifyCollision(CIwGameActor* other);
    void NotifyCollisionWithBomb(CollidableActor* bomb_actor);
    void NotifyCollisionWithParticles(CollidableParticles* fire_breath);

	void SetDestructible(bool destructible = true) {isDestructible = destructible; }
	bool CanBeBlownUp() { return isDestructible; }
	void SetLightSource(bool light = true) { isLightSource = light; }
	bool LightSource()	{ return isLightSource; }
	void SetBouncy(float32 restitution, bool bouncy = true);
	virtual bool IsBouncy() { return m_isBouncy; }
	void SetHeavy(bool heavy = true);
	bool IsHeavy()	{return isHeavy; }
	void SetAttackPower(int ap) { AttackPower = ap; }
	int GetAttackPower() {return AttackPower;}

    virtual void Explode();
    
	void SetLanded(bool landed) {Landed = landed;}
	bool HasLanded() const { return Landed; }
    
    void            SetAsDroppedObject(bool isDropped = true);
    bool            IsDroppedObject();
    bool            CanHurtPlayer() const;
    virtual bool    IsAttackedByPlayer(BaseActor* player)const {return false;}
};

class GameObjectActorCreator : public IIwGameXomlClassCreator
{
public:
    GameObjectActorCreator()
    {
        setClassName("gameobjectactor");
    }
    
    IIwGameXomlResource* CreateInstance(IIwGameXomlResource *parent)
    {
        return new GameObjectActor();
    }
};

//TODO: L4891 Move PlatformActor class to its own file
class PlatformActor : public GameObjectActor
{
public:

    
    // collisions
    void ResolveCollisions();
    
private:
    
};
#endif
