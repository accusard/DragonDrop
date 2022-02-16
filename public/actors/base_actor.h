//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// base_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// Declaration of base actor class to all other actors with movements
//--------------------------------------------------------------------------------------------------------

#if !defined(_BASE_ACTOR_H_)
#define _BASE_ACTOR_H_

#include "IwGame.h"
#include "game.h"
#include "state/actor_state.h"
#include "condition.h"
#include "button_icon.h"
#include "utility_actor.h"
#include "movement.h"
#include "observer.h"
#include "state_manager.h"
#include "game_timer.h"

//Default values. LoadPlayer.xml will override some of these values
#define	IMPACT_FORCE                  2000000000
#define FOREGROUND_SIZE               8

const float MOVING_SPEED            = 0.5f;     /**< a float that determines if a state is in a moving state */
const float JUMPING_HEIGHT          = 21.0f;
const float MAX_SPEED               = 8.0f;
const float JUMP_DAMPENING          = 0.9f;     // 2 or	8
const float BOUND_GROUND            = -10.0f;   // 100.0 or -10.0f
const float ACTOR_DEFAULT_FRICTION  = 0.9f;


class ActorState;
class SensorPropertyActor;
class Movement;

enum eSensorType
{
    WEAKNESS_SENSOR = 1000,
};

enum eDamageType
{
    DMG_NORMAL = 2000,
    DMG_IMPACT,
    
};

enum eShape
{
    S_NONE,
    S_BOX,
    S_CIRCLE,
    S_POLYGON,
};

enum e_PowerUpTypes
{
    SPRING, SWORD, BUBBLE, UMBRELLA
};

enum eDeath
{
    DEFAULT,
    IMPACT,
};

class PowerUpItems
{
private:
    unsigned int    m_id;
    bool            m_active;
    
public:
    PowerUpItems(unsigned int id) : m_id(id), m_active(true) {}
    
    bool    IsActive() const                {return m_active;}
    int     GetID() const                   {return m_id;}
    void    ToggleActive(bool b)            {m_active = b;}
};

struct ActorAnimation
{
    CIwGameAnimTimeline* Moving;
    CIwGameAnimTimeline* Jumping;
    CIwGameAnimTimeline* Idle;
    CIwGameAnimTimeline* Recover;
    CIwGameAnimTimeline* Death;
    CIwGameAnimTimeline* Attack;
    CIwGameAnimTimeline* Falling;
    CIwGameAnimTimeline* Hovering;
    CIwGameAnimTimeline* Looking;
    
    ActorAnimation() : Moving(NULL), Jumping(NULL), Idle(NULL), Recover(NULL), Death(NULL), Attack(NULL), Falling(NULL), Hovering(NULL), Looking(NULL)
    {}
};

/**
 * \brief The base class of all actors that interact with the environment as well as with other 
 *        BaseActor objects
 *
 * BaseActors' utilizes a series of states and movements. Their behaviors are different depending 
 * on their current state. Each function that calls for an action for or against the actor will 
 * make a state check to determine if the actions can be applied to the actor.
 */
class BaseActor : public CollidableActor
{
    using CIwGameActorImage::Init;
    
public:
    enum e_direction
    {
        FACING_RIGHT    =  1,
        FACING_LEFT     = -1,
        NONE            =  0,
    };
    
protected:
	//Actor Properties
    e_direction             Facing;                 //Orientation of actor
	e_direction             OriginalFacing;         //Orientation of actor when first spawned
	bool                    IsDying;                //Status of actor to flag before deletion
	bool                    Jumping;                //Actor jumping
	int                     HP;
    int                     m_origHP;               /*<< The original HP of this actor at spawn */
	ActorAnimation          Animate;                //Timelines for actor animations
    
    Movement*               m_movement;             /*<< The current movement of the actor */
    CIwList<Movement*>      m_oldMovements;         /*<< A list of movements that have been marked to be deleted at a later time */
	float                   m_speed;                /*<< The speed of the actor that will be applied to the actor's velocity */
    float                   m_maxSpeed;             /*<< The maximum speed of the actor */
    float                   m_speedMultiplier;      /*<< A modifier that will be applied to m_speed */
    bool                    m_accelerated;          /*<< This is true if the actor have reached its maximum speed */
    CIwFVec2                m_originalSpeed;        /*<< The original speed of actor at spawn time */
    float                   m_originalFriction;     /*<< The original friction of the actor that gets reapplied when the actor */
                                                    /*<< is traversing the ground */
    bool                    m_movingDmgOthers;      /*<< tracks whether or not this actor can damage others while it is moving */

    
	GameTimer               DeathTimer;             /*<< Time before actor respawn or set to be destroyed */
    CIwGameDummySprite*     HitBox;
    SensorPropertyActor*    BaseSensor;
    
    //CIwList<ActorState*>    m_states;
    //ActorState*             m_currentState;
    //ActorState*             m_nextActorState;
    PowerUpItems*           m_Item;
    float                   m_hoverValue;
    //unsigned long           m_prevStateID;        /*<< The id of the previous state of the actor. */
    bool                    m_canAttack;            /*<< The state of the actor if it can attack */
    bool                    m_canMove;              /*<< The state of the actor if it can move */
    bool                    m_canBeHurt;            /*<< The state of the actor if it can be hurt */

    StateManager*           m_actorStateManager;

public:
	//Constructors and destructors
	BaseActor();
	virtual ~BaseActor();
    
	//Derived virtual member functions from CIwGameActor and CIwGameActorImage
    bool    Update(float dt);
    bool	UpdateVisual();
    void	ResolveCollisions();
    void	OnTapped(int index, int x, int y, bool allow_bubble);
    bool    LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    
    //void            AddNewAttackState(CIwGameActor* target);
    StateManager*   GetActorStateManager() {return m_actorStateManager; }
    bool            Respawn();
    
    
    //Class-specific member functions
    bool            Init(const char* name);
    void            InitHitBox(bool ext_width, bool ext_height, float factor);
	void			setHP(int hp);					//set hitpoints of the actor
	int				getHP() const { return HP; }	//return HP of actor
	void			setMaxSpeed(float max);			//set MaxSpeed of the actor
	virtual void	UpdateMovement();				//virtual functions for inherited actors
	b2Vec2			GetCurrentVelocity() const;		//get the current velocity of the actor
	void			setVelocityX(float x);			//set the x velocity of the actor with Box2D physics enabled
	void			setVelocityY(float y);			//set the y velocity of the actor with Box2D physics enabled
	void			setVelocitybVec2(b2Vec2 vel);		//special setter to call when user needs to update ONLY the x or the y velocity of the actor
	void			setDying(bool dying = true);
    bool			isDying();
	bool			FlipActor();					//Flip the actor's image horizontally. NOTE: IwGameSprite::setImageTransform(CIw2DImageTransform transform)	{ ImageTransform = transform; UVsDirty = true;}
	void			ApplyImpactForce(float);				//
    GameTimer*      GetObjectTimer();
    
    void            ApplyDamage(const int dmg) {HP -= dmg;}
	bool			isJumping() const;
    void            SetJumping(bool jump = true){Jumping = jump;}
	virtual void	Jump(float, bool = true){}

	void			UpdateDeltaJump();
	void			KillActor(eDeath d, float f = 0, bool collide = false);
    bool            IsOffScreen();
	//void			SetDeathTimer(CIwGameTimer& time) { DeathTimer = time; }
    CIwGameActor*   HitTest(float x, float y);
    void            SetBaseSensor( SensorPropertyActor* sensorActor) { BaseSensor = sensorActor; }
    SensorPropertyActor*    GetBaseSensor() const { return BaseSensor; }
    void            ResizeActorBox2dShape(int width, int height);
    void            SetOriginalFriction(float friction) {m_originalFriction = friction;}
    float           GetOriginalFriction() const {return m_originalFriction; }
    
    void            SetActorFriction(float friction);
    ActorAnimation  GetAnimate() {return Animate; }
    bool            HasItem(e_PowerUpTypes item);
    void            ToggleItem(int item, bool load_item = true);
    void            SetHoverValue(float hover_value) {m_hoverValue = hover_value; setVelocityY(hover_value);}
    float           GetHoverValue() const {return m_hoverValue;}
    
    /**
     * \name Collision
     *
     * Functions relating to the collision of various game objects.
     */
    void    NotifyCollision(CIwGameActor* other);
    void    NotifyCollisionWithSpikedBall(CollidableActor* ball);
    
    /**
     * \name Movement
     *
     * Funcions relating to the movement of this actor.
     */
    virtual void            MoveLeft();
    virtual void            MoveRight();
    virtual void            MoveToTarget(CIwFVec2 pos, const float speed_mod);
    CIwGameAnimInstance*    CreateMoveToPosAnim(CIwFVec2& orig_pos, CIwFVec2& target_pos, float speed);
    float                   GetSpeed() const;
    float                   GetMaxSpeed() const { return m_maxSpeed; }
    void                    SetMaxSpeed(float spd) { m_maxSpeed = spd; }
    bool                    LimitSpeed(float &current_spd, const float max_spd);
    void                    SetAccelerated(const bool accelerated);
    void                    SetMovement(Movement* movement);
    void                    ClearMovement();
    void                    ClearOldMovements();
    virtual void            NotifyMovementFinished();
    Movement*               GetMovement() {return m_movement;}
    void                    SetFacing(e_direction facing);
    e_direction             GetFacing() const { return Facing; }
    void                    ResetFacing();
    void                    KnockBack(float vel_x);
    bool                    MovingDamageOthers() const;
    
    /**
     * \name Actions
     *
     * Funcions relating to the action of this actor.
     */
    virtual void            StartAttack() {}
    virtual void            StopAttack() {}
    virtual bool            UpdateAttack(CIwGameActor* actor);
    virtual void            PostAttack();
    void                    ClearAttack();
    virtual CIwGameActor*   AcquireTarget(const int type);
    virtual void            NotifyTargetAcquired();
    bool                    CanBeHurt() const;
    virtual void            NotifyDying();
    virtual void            ActionsOnNotify(Event event);
    
    /**
     * \name Sounds
     *
     */
    virtual const char* GetSoundAttack() {return "";}
    
    /**
     * \name State checks
     */
    bool IsAttackAnimationPlaying();
};


#endif
