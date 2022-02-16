//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// base_actor.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// Implementation of base actor class to all other actors with movements
//--------------------------------------------------------------------------------------------------------

#include "game.h"
#include "base_actor.h"
#include "game_object_actor.h"
#include "sensor_actor.h"
#include "movement.h"
#include "target_movement.h"
#include "attack_state.h"
#include "options.h"
#include "game_state.h"
#include "idle_state.h"
#include "hit_state.h"
#include "moving_state.h"

BaseActor::BaseActor() : CollidableActor(), Facing(FACING_RIGHT), OriginalFacing(Facing), IsDying(false), Jumping(true), HP(1), m_origHP(HP), m_movement(NULL), m_speed(MOVING_SPEED), m_maxSpeed(MAX_SPEED), m_speedMultiplier(0.0f), m_accelerated(false), m_originalSpeed(0,0), m_originalFriction(0), m_movingDmgOthers(false), HitBox(NULL), BaseSensor(NULL), m_Item(NULL), m_hoverValue (0),  m_canAttack(true), m_canMove(true), m_canBeHurt(true), m_actorStateManager(new StateManager())
{
	setLayer(L_ACTORS);
    setActualClassType("baseactor");
    if(GAME->GetUserOptions()->IsDebugging())
        setDraggable(true);
}

BaseActor::~BaseActor()
{
    SAFE_DELETE(m_Item);
    SAFE_DELETE(m_actorStateManager);
    
    ClearMovement();
    ClearOldMovements();
}

bool BaseActor::Init(const char* image)
{
    if(image == NULL)
    {
        CIwGameActor::Reset();
        CIwGameDummySprite* sprite = new CIwGameDummySprite();
        sprite->Init();
        Visual = sprite;
        Scene->getSpriteManager()->addSprite(sprite);
        return true;
    }
    
    CIwGameBrushImage* brush = (CIwGameBrushImage*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource(image, CIwGameXomlNames::Brush_Hash);
    
    if(brush == NULL)
    {
        CIwGameError::LogError("Error:: Could not find brush to init Actor..", getName().c_str());
        return false;
    }
    
    int width   = brush->getSrcRect().w;
    int height  = brush->getSrcRect().h;
    
    CIwGameActorImage::Init(brush, width, height);
    
    //m_actorStateManager = new StateManager();
    
    return true;
}

bool BaseActor::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
	if (!CollidableActor::LoadFromXoml(parent, load_children, node))
		return false;
    
    //default values
    int     hp  = 1;
    
    //increase the size of the hitbox
    float hitbox_factor = 1.1f;
    
    // Custom XOML Tags
	for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
        unsigned int name_hash = (*it)->getName().getHash();
        if(name_hash == CIwGameXomlNames::Velocity_Hash)
        {
            m_originalSpeed.x = (*it)->GetValueAsFloat();
        }
        else
        if(name_hash == CIwGameString("facing").getHash())
        {
            CIwGameString value = (*it)->GetValue();
            //e_direction facing = NONE;
            
            if(value == "right")
                Facing = FACING_RIGHT;
            else
            if(value == "left")
                Facing = FACING_LEFT;
            
            OriginalFacing = Facing;
            FlipActor();
        }
        else
        if(name_hash == CIwGameString("hitboxfactor").getHash())
        {
            hitbox_factor = (*it)->GetValueAsFloat();
        }
        else
        if(name_hash == CIwGameString("speed").getHash())
        {
            m_speed = (*it)->GetValueAsFloat();
        }
        else
        if(name_hash == CIwGameString("maxspeed").getHash())
        {
            m_maxSpeed = (*it)->GetValueAsFloat();
        }
        else
        if(name_hash == CIwGameString("hp").getHash())
        {
            hp = (*it)->GetValueAsInt();
        }
    }
    
    //assign values
    setHP(hp);
    m_origHP = hp;
    InitHitBox(true, true, hitbox_factor);
    return true;
}

void BaseActor::InitHitBox(bool ext_width, bool ext_height, float factor)
{
    if(factor <= 1.0f )
        return;
    
    //Init the hitbox of the actor
    CIwGameDummySprite* sprite = new CIwGameDummySprite();
    CIwGameBrushImage* brush = (CIwGameBrushImage*)getVisual()->getBrush();
    
    float width     = (float)brush->getSrcRect().w;
    float height    = (float)brush->getSrcRect().h;
    
    if(ext_width)
        width *= factor;
    
    if(ext_height)
        height *= factor;
    
    sprite->Init();
    sprite->setDestSize(width, height); // currently, setDestSize takes ints' instead of floats' as arguments
    
    
    CIwGameSprite* parent_sprite = Visual->getParent();  // ??game crashes on deployed releases if we don't use this line, but runs perfectly fine on debug mode
    
    if(parent_sprite != NULL)
        parent_sprite->addChild(sprite);
    
    if(getLinkedTo() != NULL)
        getLinkedTo()->getVisual()->setLinkedTo(sprite);
    
    Scene->getSpriteManager()->addSprite(sprite);
    
    //((CIwGameScene*)getParent())->getSpriteManager()->addSprite(sprite);
    
    HitBox = sprite;
}

/**
 * TODO: #O3843 need to replace function UpdateMovement()
 */
bool BaseActor::Update(float dt)
{
    
    //if actor is not alive, set active to false
    if(DeathTimer.HasTimedOut() || IsOffScreen())
        setActive(false);
    
    // update the state of the actor
    if(m_actorStateManager != NULL)
        m_actorStateManager->Update();
    
    // TODO: #A2031 Move to debugging unit
    if(isDragging() && BaseSensor != NULL && GAME->GetUserOptions()->IsDebugging())
    {
    	BaseSensor->UpdatePosition();
    }
    
    ClearOldMovements();
    
	return CollidableActor::Update(dt);
}

/**
 * \brief The base function to acquire a target in which all derived classes should override.
 * \return NULL as default.
 */
CIwGameActor* BaseActor::AcquireTarget(const int type)
{
	return NULL;
}

/**
 * \brief Notify the actor that a target have been acquired.
 */
void BaseActor::NotifyTargetAcquired()
{
    
}

/**
 * \brief Checks if this actor can be hurt.
 *
 * The function will return true by default if this actor does not currently have a state.
 *
 * \return can_be_hurt if this actor can be hurt in its current state.
 */
bool BaseActor::CanBeHurt() const
{
    bool can_be_hurt = true;
    
    if(m_actorStateManager->GetCurrentState()!= NULL)
        return m_actorStateManager->GetCurrentState()->CanBeHurt();
    
    return can_be_hurt;
}

bool BaseActor::UpdateVisual()
{
	if(!CIwGameActorImage::UpdateVisual())
		return false;
	
    if(HitBox != NULL && HitBox->getPosition() != Visual->getPosition())
    {
        HitBox->setPosition(Visual->getPosition().x, Visual->getPosition().y);
    }
    
    /*
    if(GAME->GetStateID() != STATE_PAUSE)
    {
        //if(Timeline != PatrolTimeline)
        //  setTimeline(PatrolTimeline);
        
        if(Timeline != NULL)
            Timeline->play();
    }
	else
    if(GAME->GetStateID() == STATE_PAUSE)
    {
        if(Timeline != NULL)
            Timeline->pause();
        //setTimeline(NULL);
    }
    */
	return true;
}

void BaseActor::setVelocitybVec2(b2Vec2 vel)
{
    
	Velocity.x = vel.x;	Velocity.y = vel.y;
#if defined(IW_GAME_BOX2D)
	b2Body* body = (Box2dBody == NULL) ? NULL : Box2dBody->getBody();
	if (body != NULL)
	{
		body->SetLinearVelocity(vel);
	}
#endif	// IW_GAME_BOX2D
}

void BaseActor::ResolveCollisions()
{	
	//a list of collidables
	CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();

	for(int t = 0; t < started.getSize(); t++)
	{
		CIwGameBox2dCollidable* collision = started.element_at(t);
		if(collision != NULL)
		{
			CIwGameActor* actor = (CIwGameActor*)collision->getUserData();
			if(actor != NULL)
			{
				actor->NotifyCollision(this);
			}
		}
	}

}

/**
 * \brief Add a new Attack state to the current actor after clearing all previous Attack States
 * \param target The target of the Attack state.

void BaseActor::AddNewAttackState(CIwGameActor* target)
{
    ClearAttack();
    
    m_actorStateManager->OnNotify(this, EVENT_START_ATTACK);
}
 */
void BaseActor::NotifyCollision(CIwGameActor* other)
{
	CIwGameError::LogError("BaseActor::NotifyCollision()");
}

/**
 * \brief If an actor is below a SpikedBall object then apply damage and blink the actor
 */
void BaseActor::NotifyCollisionWithSpikedBall(CollidableActor *ball)
{
    if(!m_actorStateManager->GetCurrentState()->CanBeHurt())
        return;
    
    int actor = FindActorOrientationTo(ball, ActorOrientation::Y_AXIS);
    
    // if this actor is below the ball, then apply damage
    if(actor == ActorOrientation::BELOW)
    {
        m_actorStateManager->OnNotify(this, EVENT_IS_HIT);
    }
}

void BaseActor::UpdateMovement()
{
    /*
    if(GetCurrentVelocity().x <= MOVING_SPEED/2 && GetCurrentVelocity().x >= -MOVING_SPEED/2)
        ChangeState(ACTOR_IDLE);
    
    if(GetCurrentVelocity().x >= MOVING_SPEED || GetCurrentVelocity().x <= -MOVING_SPEED)
        ChangeState(ACTOR_MOVING);
    
    if(GetCurrentVelocity().y > 0.1)
        ChangeState(ACTOR_FALLING);
     */
}

void BaseActor::setHP(int hp)
{
	HP = hp;
}

void BaseActor::setMaxSpeed(float max)
{
	m_maxSpeed = max;
}

void BaseActor::SetAccelerated(const bool accelerated)
{
    m_accelerated = accelerated;
}

/**
 * \brief Notify the actor that it has finished moving.
 */
void BaseActor::NotifyMovementFinished()
{
    m_accelerated = false;
    ClearMovement();
    //SAFE_DELETE(m_movement);
}

void BaseActor::SetFacing(e_direction facing)
{
    Facing = facing;
    FlipActor();
}

b2Vec2 BaseActor::GetCurrentVelocity() const
{
	b2Body* body = Box2dBody->getBody();
	b2Vec2 vel = body->GetLinearVelocity();
	return vel;
}

void BaseActor::setVelocityX(float x)
{
#if defined(IW_GAME_BOX2D)
	b2Vec2 vel = GetCurrentVelocity();
	vel.x = x;
	setVelocitybVec2(vel);
#endif
}

void BaseActor::setVelocityY(float y)
{
#if defined(IW_GAME_BOX2D)
	b2Vec2 vel = GetCurrentVelocity();
	vel.y = y;
	setVelocitybVec2(vel);
#endif
}

bool BaseActor::FlipActor()
{
	
	int flip_y = ((CIwGameBitmapSprite*)Visual)->getImageTransform() & IW_2D_IMAGE_TRANSFORM_FLIP_Y;
	if (Facing == FACING_LEFT)
		((CIwGameBitmapSprite*)Visual)->setImageTransform((CIw2DImageTransform)(flip_y | IW_2D_IMAGE_TRANSFORM_FLIP_X));
	else
		((CIwGameBitmapSprite*)Visual)->setImageTransform((CIw2DImageTransform)flip_y);
	
	return true;
}

void BaseActor::ApplyImpactForce(float impact_force)
{
	int dir = 1;

	if(Velocity.x > 0)
		dir *= -1;

	float32 mass = Box2dBody->getBody()->GetMass();
	//Box2dBody->ApplyForceToCenter(mass*IMPACT_FORCE,mass*IMPACT_FORCE);
	Box2dBody->getBody()->SetFixedRotation(false);
	//Box2dBody->getBody()->SetGravityScale(1.0);
	Box2dBody->ApplyTorque(dir * impact_force);
}

bool BaseActor::isJumping() const
{
	return Jumping;
}

void BaseActor::OnTapped(int index, int x, int y, bool allow_bubble)
{
	CIwGameError::LogError("User tapped on: ", CIwGameString(getName()).c_str());
}

bool BaseActor::Respawn()
{
    if(!CollidableActor::Respawn())
        return false;

    StopAttack();
    
    //reset original speed
	Velocity.x = m_originalSpeed.x;
	Velocity.y = m_originalSpeed.y;
    
    setDying(false);
    ResetFacing();
    
    ClearMovement();
    setHP(m_origHP);
    
    return true;
}

void BaseActor::ResetFacing()
{
    SetFacing(OriginalFacing);
    if(Facing == FACING_RIGHT && Velocity.x < 0)
    {
        FlipActor();
        Velocity.x *= -1;
    }
}

/**
 * \brief Knock the actor back based on the actor's current facing.
 * \param vel_x the force of the knockback.
 */
void BaseActor::KnockBack(float vel_x)
{
    BaseActor::e_direction facing = GetFacing();
    
    // apply the opposite velocity if facing either left or right
    if((facing == BaseActor::FACING_LEFT && vel_x < 0) || (facing == BaseActor::FACING_RIGHT && vel_x > 0))
        vel_x *= -1;
    
    setVelocityX(vel_x);
    
}

/**
 * \brief Checks whether or not moving enable this actor to damage other objects.
 * \return true if an actor can cause damage while moving.
 */
bool BaseActor::MovingDamageOthers() const
{
    return m_movingDmgOthers;
}

void BaseActor::setDying(bool dying)
{
	IsDying = dying;

	if(dying)
    {
		DeathTimer.setDuration(3000);

        if(Animate.Death != NULL)
        {
            Timeline = Animate.Death;
            Timeline->restart();
        }
        else
        if(Animate.Idle != NULL)
        {
            Timeline = Animate.Idle;
            Timeline->restart();
        }
        
        setHP(0);
    }
    else
    {
        DeathTimer.Stop();
    }
    
}


GameTimer* BaseActor::GetObjectTimer()
{
    return &DeathTimer;
}


bool BaseActor::isDying()
{
    if(!IsDying && getHP() < 1)
        setDying(true);
    
    return IsDying;
}
void BaseActor::UpdateDeltaJump()
{
	//
}

void BaseActor::KillActor(eDeath d, float force, bool collide)
{
    if(IsDying)
        return;
    
	setCollidable(collide);
    if(GetBaseSensor() != NULL)
        BaseSensor->setActive(false);
    
    switch(d)
    {
        case IMPACT:
        {
            ApplyImpactForce(force);
            break;
        }
    }
	setDying();
}

bool BaseActor::IsOffScreen()
{
    if(GAME->GetUserOptions()->IsDebugging())
        return false;
    
    if(getPosition().y < Scene->getExtents().h * 0.45f)
        return false;
    
    CIwGameError::LogError("Death due to scene extents: ", CIwGameString(Scene->getExtents().h * 0.45f).c_str());
    setPosition(0,0);
    return true;
    
}

CIwGameActor* BaseActor::HitTest(float x, float y)
{
    if(HitBox == NULL || !Visual->isVisibleWithParents())
        return NULL;
    
    if(HitBox->HitTest(x,y))
    {
        //CIwGameError::LogError("DummySprite HitBox..", getName().c_str());
        return this;
    }

    return NULL;
}

void BaseActor::ResizeActorBox2dShape(int width, int height)
{
    CIwGameBox2dMaterial* material = NULL;
    if(Box2dBody != NULL)
        material = Box2dBody->getBodyMaterial();
    
    if(material == NULL)
    {
        CIwGameError::LogError("Calling BaseActor::ResizeActorBox2dShape..");
        CIwGameError::LogError("..actor does not have Box2dMaterial attached to it to resize their Box2dShape..");
        return;
    }
    
    //define new shape
    CIwGameGeomShapePolygon* polygon = new CIwGameGeomShapePolygon();
    CIwGameShape* shape = new CIwGameShape();
    
    //currently, polygon shape uses 4 vertices only
    polygon->Vertices = new CIwFVec2[4]();
    polygon->NumVertices = 4;
    
    //translate fixture from the passed value width and height
    polygon->Vertices[0].x = width / 2;
    polygon->Vertices[0].y = -1 * height / 2;
    
    polygon->Vertices[1].x = width /2;
    polygon->Vertices[1].y = height / 2;
    
    polygon->Vertices[2].x = -1 * width / 2;
    polygon->Vertices[2].y = height / 2;
    
    polygon->Vertices[3].x = -1 * width / 2;
    polygon->Vertices[3].y = -1 * height /2;
    
    shape->setShape(polygon);
    Box2dBody->setFixture(shape, material, 0, 0);
    
    SAFE_DELETE(shape);
}

void BaseActor::SetActorFriction(float friction)
{
    if(friction == getBox2dBody()->getFixture()->GetFriction())
        return;
    
    b2Contact* contact_list = getBox2dBody()->getWorld()->getWorld()->GetContactList();
    b2Fixture* actor_fixture = getBox2dBody()->getFixture();
    
    while(contact_list != NULL)
    {
        b2Fixture* _fixtureA = contact_list->GetFixtureA();
        b2Fixture* _fixtureB = contact_list->GetFixtureB();
        
        if(actor_fixture == _fixtureA || actor_fixture == _fixtureB)
        {
            contact_list->SetFriction(friction);
            break;
        }
        contact_list = contact_list->GetNext();
    }
    
    return;
}

void BaseActor::ToggleItem(int item, bool load_item)
{

    ButtonIcon* action_bar = (ButtonIcon*)GAME->findScene("HUD")->findActor("ActionBar");
    
    if(action_bar != NULL && load_item)
    {
        action_bar->LoadActionBar(item);
    }
    
    if(m_Item != NULL && m_Item->GetID() == item)
    {
        m_Item->ToggleActive(true);
        return;
    }
    
    if(m_Item != NULL)
    {
        delete m_Item;
        m_Item = NULL;
    }
    
    m_Item = new PowerUpItems(item);
    //action_bar->SetActionID(item);      //link the ButtonIcon to this item's id
}

bool BaseActor::HasItem(e_PowerUpTypes item)
{
    bool hasItem = false;
    
    if(m_Item == NULL)
        return hasItem;
    
    if(m_Item->GetID() == item && m_Item->IsActive())
        hasItem = true;
    
    return hasItem;
}


void BaseActor::MoveLeft()
{
    Facing = FACING_LEFT;
    FlipActor();
}
void BaseActor::MoveRight()
{
    Facing = FACING_RIGHT;
    FlipActor();
}

/**
 * \brief Move the actor to the target position.
 *
 * Creates a new TargetMovement object and assign it to the actor's current movement.
 *
 * \param pos A CIwFVec of the actor's targeted position.
 */
void BaseActor::MoveToTarget(CIwFVec2 pos, float speed_mod)
{
    if(!m_actorStateManager->GetCurrentState()->CanMove())
        return;
    
    //CIwGameAnimTimeline* move_timeline = Animate.Moving;
    bool ig_col = false;
    
    Movement* movement = new TargetMovement(this, ig_col, pos, speed_mod);
    SetMovement(movement);
    m_actorStateManager->OnNotify(this, EVENT_START_MOVING);
}

CIwGameAnimInstance* BaseActor::CreateMoveToPosAnim(CIwFVec2& orig_pos, CIwFVec2& target_pos, float speed)
{
    CIwGameAnim* anim = CIwGameAnimUtil::CreateVec2Anim("attack_anim", orig_pos, target_pos, speed, CIwGameAnimFrame::AT_Linear);
    getScene()->getResourceManager()->addResource(anim);
    
    //create the instance of animation
    CIwGameAnimInstance* anim_instance = new CIwGameAnimInstance();
    anim_instance->setAnimation(anim);
    anim_instance->setRepeatCount(1);
    
    return anim_instance;
}

/**
 * \brief Checks to see if this actor can attack in its current state.
 *
 * This function is usually called by its ActorState->Update() function.
 *
 * \return true if the current state of the actor allow it to attack.
 */
bool BaseActor::UpdateAttack(CIwGameActor* actor)
{
    bool can_attack = m_actorStateManager->GetCurrentState()->CanAttack();
    
    return (can_attack && actor != NULL);
}

/**
 * \brief Perform any functions after its attacks.
 *
 * Default is to change its state to idle.
 */
void BaseActor::PostAttack()
{
    m_actorStateManager->OnNotify(this, EVENT_ATTACK_FINISHED);
}

/**
 * \brief Clears the actor's attack states that contain the state id ACTOR_ATTACK.
 */
void BaseActor::ClearAttack()
{
    m_actorStateManager->ClearAttackStates();
}

/**
 * \brief Sets the movement of this actor.
 *
 * The new movement of the actor is set through the pointer and the old movement is placed in
 * m_oldMovements to be cleared later.
 *
 * \param The pointer to an instance of Movement
 * TODO: decide if ClearMovement() should be called in this function
 */
void BaseActor::SetMovement(Movement* movement)
{
    // place current movement in m_oldMovements list to be cleared later
    ClearMovement();
    
    m_movement = movement;
    
    if(movement != NULL)
        movement->SetActor(this);
}

/**
 * \brief Clear the current movement of the actor and add the movement to a list to be cleared 
 * later.
 */
void BaseActor::ClearMovement()
{
    if(m_movement != NULL)
    {
        m_movement->SetActor(NULL);
        m_oldMovements.push_back(m_movement);
        m_movement = NULL;
    }
}

/**
 * \brief Clear any old movements that have been added to the list
 */
void BaseActor::ClearOldMovements()
{
    typedef CIwList<Movement*>::iterator _It;
    
    for(_It it = m_oldMovements.begin(); it != m_oldMovements.end(); ++it)
    {
        delete *it;
    }
    m_oldMovements.clear();
}

float BaseActor::GetSpeed() const
{
    return m_speed;
}

/**
 *  \brief Checks the speed of the actor and if it goes over m_maxSpeed then limit it to m_maxspeed
 */
bool BaseActor::LimitSpeed(float &current_spd, const float max_spd)
{
    // limit top speed
    if(current_spd >= max_spd || current_spd <= -max_spd)
    {
        if(current_spd >= max_spd) current_spd = max_spd;
        else
            if(current_spd <= -(max_spd)) current_spd = -(max_spd);
        
        return true;
    }
    else
        return false;
}

void BaseActor::NotifyDying()
{
    
}

void BaseActor::ActionsOnNotify(Event event)
{

}

/**
 * \brief Check the state of the actor to determine if the attack animation is currently playing
 */
bool BaseActor::IsAttackAnimationPlaying()
{
    bool playing = GetActorStateManager()->GetCurrentState()->IsAttackAnimationPlaying();
    
    return playing;
}
