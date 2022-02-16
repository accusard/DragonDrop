//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	powered by Pocketeers Limited's IwGame Engine
//
// player_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
// 
//--------------------------------------------------------------------------------------------------------

#if !defined(_ACTOR_H_)
#define _ACTOR_H_

#include "IwGame.h"
#include "base_actor.h"
#include "collectable_actor.h"
#include "gameobject.h"
#include "enemy_actor.h"
#include "toolbar.h"
#include "button_icon.h"
#include "jumping_state.h"
#include "falling_state.h"
#include "hover_state.h"

class PowerUpActor;

class PlayerActor : public BaseActor
{
    using BaseActor::MoveLeft;
    using BaseActor::MoveRight;
    
protected:
	// Properties of PlayerActor
	ButtonIcon*			RightButton;
	ButtonIcon*			LeftButton;
	ButtonIcon*			JumpButton;
    int                 Ability;
    bool                ActionBarActive;

public:
    
	PlayerActor();
    virtual ~PlayerActor(){};
    
	/// Derived functions from base class CIwGameActor
	bool		LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode* node);
	void		OnTapped(int index, int x, int y, bool allow_bubble);
	bool		Update(float dt);
	//bool		UpdateVisual();

    // collisions
    void        ResolveCollisions();
    void		NotifyCollisionWithPowerup(PowerUpActor* power);
    void        NotifyCollisionWithTrampoline(CollidableActor* trampoline_actor);
    void        NotifyCollisionWithStone(CollidableActor* stone_actor);
    void        NotifyCollisionWithSpikedBall(CollidableActor* ball);
    void        NotifyCollisionWithParticles(CollidableParticles* particles);
    void        NotifyCollisionWithEnemy(CollidableActor* enemy);
    
	// PlayerActor class specific member functions
	void			UpdateInput();
	void			MoveLeft(float &, float &);
	void			MoveRight(float &, float &);
	void			Jump(float, bool = true);
	//float			ReadArrow(float &s);
	void			CameraFollow(CIwGameActor* actor);
	ButtonIcon*		GetJumpButton() {return JumpButton;}

    void			ReadInput(float &x, float &y);
    void            ReadInputButtons(float &vx, float &vy);
    void            SetAbility(int type) { Ability = type; }
    bool            IsActionBarActive(){return ActionBarActive;}
    void            SetActionBar(bool active = true){ActionBarActive = active;}
    
    bool            Respawn();
	//GameObjects		GetInventory() const { return Inventory; }
	//ActorAnimation	PlayAnimation();
    
    const char* GetSoundAttack();
};

class PlayerActorCreator : public IIwGameXomlClassCreator
{
public:
	PlayerActorCreator()
	{
		setClassName("PlayerActor");
		
	}

	IIwGameXomlResource*	CreateInstance(IIwGameXomlResource *parent)
	{
		return new PlayerActor();
	}
};

#endif
