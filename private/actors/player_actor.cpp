//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK Copyright 2018-2022
//	Powered by Pocketeers Limited's IwGame Engine
//
// player_actor.cpp
//	Programmed by: Vanny Sou
// 
//--------------------------------------------------------------------------------------------------------

#include "actors/player_actor.h"
#include "actors/static_actor.h"
#include "game.h"
#include "scene.h"
#include "game_object_actor.h"
#include "game_state.h"
#include "stone_actor.h"
#include "powerup_actor.h"

#define MAX_TOUCHES 4

PlayerActor::PlayerActor() : Ability(0), ActionBarActive(false)
{
	// Load Animations
	Animate.Moving = IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->FindTimeline("player_run", Scene);
	Animate.Jumping = IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->FindTimeline("player_jump", Scene);
	Animate.Idle	= IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->FindTimeline("player_idle", Scene);
	Animate.Recover	= IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->FindTimeline("player_recover", Scene);
    Animate.Attack  = IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->FindTimeline("player_attack", Scene);
    Animate.Falling = IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->FindTimeline("player_fall", Scene);
    Animate.Hovering= IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->FindTimeline("player_hover", Scene);
    
    //create various states of actor
    m_actorStateManager->AddState(new Idle(this, Animate.Idle), true);
    m_actorStateManager->AddState(new class Jumping(this, Animate.Jumping, JUMPING_HEIGHT),false);
    m_actorStateManager->AddState(new class Falling(this, Animate.Falling),false);
    m_actorStateManager->AddState(new class Moving(this, Animate.Moving),true);
    m_actorStateManager->AddState(new class Hovering(this, Animate.Hovering),false);
    
	// Load up movement interface buttons
	RightButton = (ButtonIcon*)GAME->findScene("HUD")->findActor("UI_Right");
	LeftButton = (ButtonIcon*)GAME->findScene("HUD")->findActor("UI_Left");
	JumpButton = (ButtonIcon*)GAME->findScene("HUD")->findActor("UI_Jump");

	JumpButton->setTarget(this);
    
    //ToggleItem(UMBRELLA, false);
    
    if(GAME->GetUserOptions()->IsDebugging())
    {
        if(GAME->GetUserOptions()->HasUmbrella())
            ToggleItem(UMBRELLA, true);
    }
    
    m_resetOnLevelStart = true;
}

void PlayerActor::ResolveCollisions()
{
    CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();
    
	for(int t = 0; t < started.getSize(); t++)
	{
		CIwGameBox2dCollidable* collision = started.element_at(t);
		if(collision != NULL)
		{
			CollidableActor* actor = (CollidableActor*)collision->getUserData();
			if(actor != NULL)
                actor->NotifyCollisionWithPlayer(this);
        }
    }
    
    // TODO: #C0174 change ENEMY and SPIKED_BALL checks to CANBEHURTBYSWORD
    // check if player is attacking an enemy
    if(IsAttackAnimationPlaying())
    {
        for(CIwGameScene::_Iterator it = getScene()->begin(); it != getScene()->end(); ++it)
        {
            if( (*it)->getType() == ENEMY || SPIKED_BALL)
            {
                if(CheckCollisionRect(*it))
                {
                    CollidableActor* actor = static_cast<CollidableActor*>(*it);
                    actor->NotifyCollisionWithSword(this);
                }
            }
        }
    }
}

bool PlayerActor::Update(float dt)
{
	if(DeathTimer.HasTimedOut() || IsOffScreen())
		GAME->ResetLevel();
    
    if(GAME->GetStateID() != STATE_PAUSE)
        UpdateInput();
    
	return BaseActor::Update(dt);
}

void PlayerActor::UpdateInput()
{
    if(IsDying)
        return;
    
	//BaseActor::UpdateMovement();

	b2Body* body = Box2dBody->getBody();
	b2Vec2 vel = body->GetLinearVelocity();

    if(GAME->GetUserOptions()->GetTouchMoveOption())
        ReadInput(vel.x, vel.y);
    else
        ReadInputButtons(vel.x, vel.y);
    
    // check if player have reached max speed
    if(LimitSpeed(vel.x, GetMaxSpeed()))
        m_accelerated = true;
    else
        m_accelerated = false;
		
    //vel.x = Velocity.x;
    
    /*
    if(Timeline != Animate.Jumping)
    {
        //SetActorFriction(0.9f);
        if(Timeline != Animate.Attack)
        {
            Timeline = Animate.Moving;
            if(vel.x <= MOVING_SPEED/2 && vel.x >= -MOVING_SPEED/2)
            {
                Timeline = Animate.Idle;
                Timeline->restart();
            }
        }
    }
    */
    
	setVelocitybVec2(vel);
	
}



void PlayerActor::OnTapped(int index, int x, int y, bool allow_bubble)
{
    if(GAME->GetUserOptions()->GetTouchMoveOption())
        Jump(JUMPING_HEIGHT, true);
}

bool PlayerActor::LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode* node)
{
	if (!CIwGameActorImage::LoadFromXoml(parent, load_children, node))
		return false;

    SensorPropertyActor* base_sensor = (SensorPropertyActor*)Scene->findActor("BaseSensor");
    
	// Custom XOML attributes
	for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int attrib_name_hash = (*it)->getName().getHash();

		if(attrib_name_hash == CIwGameXomlNames::Type_Hash)
		{
			CIwGameString type = (*it)->GetValue();
			if(type == "PLAYER") 
			{
				setType(PLAYER);
			}
		}
		else
        if(attrib_name_hash == CIwGameString("basesensor").getHash())
        {
            CIwGameString attach_to;
            attach_to = (*it)->GetValue();
            
            SensorPropertyActor* attach_to_actor = (SensorPropertyActor*)getScene()->findActor(attach_to.c_str());
            
            SetBaseSensor(attach_to_actor);
            
            if(GAME->GetUserOptions()->ShowPSensor())
            {
                CIwGameBrushImage* image = (CIwGameBrushImage*)Scene->getResourceManager()->FindResource("Brush_Generic_Square", CIwGameXomlNames::Brush_Hash);
                
                attach_to_actor->Init(this,image);
            }
            else
                attach_to_actor->Init(this);
        }
	}
    
	InitHitBox(true, true, 1.5f);
    
	return true;
}

void PlayerActor::ReadInput(float &vx, float &vy)
{

    CIwGameTouch *touch = IW_GAME_INPUT->getTouch(0);

    //ChangeState(ACTOR_MOVING);
    // accelerate velocity right
    if(touch->dx > 0) 
    {
        MoveRight(vx, vy);
    }
    
    // accelerate velocity left
    if(touch->dx < 0) 
    {
        MoveLeft(vx, vy);
    }
}

void PlayerActor::ReadInputButtons(float &vx, float &vy)
{
    //ChangeState(ACTOR_MOVING);
    
    if(RightButton->isSelected())
    {
        MoveRight(vx, vy);
    }

    if(LeftButton->isSelected())
    {
        MoveLeft(vx, vy);
    }
}

void PlayerActor::CameraFollow(CIwGameActor* actor)
{
	CIwGameScene* scene = (GAME)->getCurrentScene();
	StaticActor* background = (StaticActor*)scene->findActor("Background");

	if(background != NULL)
	{
		CIwGameCamera* camera = scene->getCamera();
		//Record the actor's original spawn positions
		//CIwFVec2 player_fvec2			= getOriginalPosition();
		CIwFVec2 background_fvec2		= background->getPosition();
	
		//Record the camera's position
		CIwFVec2 cam_fvec2 = camera->getPosition();

		//determine camera's position based on actor's position
		float followcam_posx = actor->getPosition().x + 50;
		float followcam_posy = actor->getPosition().y - 250;

		//Determine slow background scroll
		background_fvec2 = (cam_fvec2 * (float)0.9);
		background_fvec2.x -= (float)300;
		//update the background's position
		background->setPosition(background_fvec2);
	
		//set the camera's position to follow the player
		camera->setPosition(followcam_posx, followcam_posy);
		
	}
}

bool PlayerActor::Respawn()
{
    if(!BaseActor::Respawn())
        return false;
    
    CIwFVec2 start_pos = static_cast<GameScene*>(Scene)->GetPlayerStartPos();
    
    this->setPosition(start_pos);
    GetBaseSensor()->setPosition(start_pos);
    GetBaseSensor()->SetContacted();
    GetBaseSensor()->setActive(true);
    
    return true;
}

/**
 *  \brief
 *  TODO: the dampen_jump does not do anything yet
 */
void PlayerActor::Jump(float jump_height, bool dampen_jump)
{
    const long id = m_actorStateManager->GetCurrentState()->GetID();
    
	if(id == ACTOR_JUMPING || id == ACTOR_FALLING || isDying())
		return;
    
    m_actorStateManager->OnNotify(this, EVENT_JUMPING);
}

void PlayerActor::MoveLeft(float &vx, float &vy)
{
    BaseActor::MoveLeft();
    vx -= m_speed;
}

void PlayerActor::MoveRight(float &vx, float &vy)
{
    BaseActor::MoveRight();
    vx += m_speed;
}

/**
 *  \brief Notify the body of the player that it has collided with a trampoline.
 *  \param trampoline_actor The trampoline that this actor collided with.
 */
void PlayerActor::NotifyCollisionWithTrampoline(CollidableActor* trampoline_actor)
{
    
}

/**
 *  \brief Notify this actor that it has collided with a stone.
 *  \param stone_actor The stone that this actor collided with.
 */
void PlayerActor::NotifyCollisionWithStone(CollidableActor* stone_actor)
{
    int player_is_located = FindActorOrientationTo(stone_actor, ActorOrientation::Y_AXIS);
    
    if( player_is_located == ActorOrientation::ABOVE || ((GameObjectActor*)stone_actor)->HasLanded() )
        return;
    
    setDying();
}

void PlayerActor::NotifyCollisionWithSpikedBall(CollidableActor* ball)
{
    BaseActor::NotifyCollisionWithSpikedBall(ball);
}

void PlayerActor::NotifyCollisionWithParticles(CollidableParticles* particles)
{
    if(particles->getType() == FIREBREATH)
    {
        CIwFVec2 particle_pos       = particles->getPosition();
        CIwFVec2 player_pos         = this->getPosition();

        StoneActor* stone = (StoneActor*)Scene->findClosestActor(Position.x, Position.y, DROP_STONE);
        if(stone != NULL)
        {
            int player_orientation = FindActorOrientationTo(stone, ActorOrientation::X_AXIS);
            
            if(player_pos.y < particle_pos.y || player_orientation == ActorOrientation::LEFT)
                return;
            
            KillActor(IMPACT, IMPACT_FORCE);
        }
    }
}

void PlayerActor::NotifyCollisionWithPowerup(PowerUpActor* power)
{
    e_PowerUpTypes power_up_type = (e_PowerUpTypes)power->getType();
    
    ToggleItem(power_up_type);
}

void PlayerActor::NotifyCollisionWithEnemy(CollidableActor* enemy)
{
    ActorOrientation player_orient(this);
    
    int player_is_located = player_orient.FindOrientation(enemy, ActorOrientation::Y_AXIS);
    
    if(player_is_located == ActorOrientation::ABOVE)
        return;
    
    KillActor(IMPACT, IMPACT_FORCE);
}

// TODO: #Z5112 encapsulate  a list of sound by returning the object.nameOfSound
const char* PlayerActor::GetSoundAttack()
{
    return "sword_swing";
}
