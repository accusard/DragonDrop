//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// enemy_actor.h
//	Programmed by: Vanny Sou Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// Implementation of EnemyActor class with basic AI
//--------------------------------------------------------------------------------------------------------

#if !defined(_ENEMY_ACTOR_H_)
#define _ENEMY_ACTOR_H_

#include "IwGame.h"

#include "base_actor.h"
#include "idle_state.h"
#include "defend_state.h"
#include "attack_state.h"
#include "hit_state.h"
#include "moving_state.h"
#include "options.h"
#include "sensor_actor.h"

class EnemyActor : public BaseActor
{
    using BaseActor::Jump;

public:
	EnemyActor();
	virtual ~EnemyActor(){}
    
	//Derived virtual member functions from CIwGameActor
    bool            LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
	void            OnTapped(int index, int x, int y, bool allow_bubble);
    bool            Update(float dt);
    
    /**
     * \name Collisions
     */
    void            ResolveCollisions();
    void            NotifyCollisionWithSword(CIwGameActor* sword);
    
    bool            TargetInRange(CIwGameActor* target);
    CIwGameActor*   AcquireTarget(const int type);
    
    /**
     * \name Actions
     *
     * Funcions relating to the action of this actor.
     */
	void                Jump();
	bool                IsFlying() const { return Flying; }
    void                SetPointOfAttack(float point_x, float point_y);
    virtual CIwFVec2    CreatePointOfAttack();
    virtual void        MoveToPointOfAttack();
    
private:
    CIwFVec2    m_pointOfAttack;  /**< The point of x in which this actor will start its
                                                 next attack */
    CIwRect     m_activeRange;      /*<< The actor's minimum and maximum range to determine its
                                             active status. (x,y,w,h) = (min_x,min_y,max_x,max_y) */
    bool        Flying;             /*<< Determines if this actor is flying or not */
    float       m_attackSpeed;
    float       m_attackRange;
};

class EnemyActorCreator : public IIwGameXomlClassCreator
{
public:
	EnemyActorCreator() { setClassName("enemyactor"); }
	IIwGameXomlResource*	CreateInstance(IIwGameXomlResource *parent) { return new EnemyActor(); }
};



//--------------------------------------------------------------------------------------------------------
//  Appendage class that is linked to another actor
//--------------------------------------------------------------------------------------------------------

class Appendage : public CollidableActor
{
private:
    //link to member?
    
public:
    Appendage() : CollidableActor()
    {
        //setLayer(L_ACTORS);
        if(GAME->GetUserOptions()->IsDebugging())
            setDraggable(true);
    }
    
    //bool LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    //void NotifyCollision(CIwGameActor *other);
    
};

class AppendageCreator : public IIwGameXomlClassCreator
{
public:
    AppendageCreator() {setClassName("appendage");}
    IIwGameXomlResource*    CreateInstance(IIwGameXomlResource * parent) {return new Appendage();}
    
};

//--------------------------------------------------------------------------------------------------------
//  Crab enemy with multiple states
//--------------------------------------------------------------------------------------------------------
/*
class Crab : public EnemyActor
{
    using EnemyActor::MoveLeft;
    using EnemyActor::MoveRight;
    
public:
    Crab() : EnemyActor(), m_vulnerable(NULL), m_leftClaw(NULL), m_orignalClawPos(0,0), m_targetPosition(0,0)
    {
        CIwGameScene* scene = GAME->findScene("MainScene");
        
        CIwGameAnimTimeline* bossCrabMove = (CIwGameAnimTimeline*)scene->getTimelinesManager()->findTimeline("bossCrab_move");
        
        //add the number of states this enemy should have
        AddState(new class Idle(this, NULL), true);
        AddState(new class Defend(this,NULL),false);
        //AddState(new class Hit(this,NULL),false);
        AddState(new class Moving(this, bossCrabMove),false);
        
        //duration to stay in the state when actor is hit
        //SetHitDuration(1000);
    }
    ~Crab()
    {
        for (_Iterator it = m_PlatformList.begin(); it != m_PlatformList.end(); ++it) delete *it;
    }
    
    typedef CIwList<CIwGameActor*>::iterator _Iterator;
    _Iterator       begin() {return m_PlatformList.begin(); }
    _Iterator       end() {return m_PlatformList.end(); }
    
    bool            Update(float dt);
    bool            LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    bool            UpdateAttack(CIwGameActor* actor);
    void            PostAttack();
    
    void            AddPlatform(CIwGameActor* platform);
    void            DestroyPlatform();
    void            ExtendClaw(CIwFVec2 to_pos);
    void            RetractClaw();
    CIwGameActor*   TargetNextPlatform() const;
    
    void            MoveLeft(float velx);
    void            MoveRight(float velx);
    
private:
    CIwList<CIwGameActor*>      m_PlatformList;     //List of platform to destroy (may not need)
    SensorActor*                m_vulnerable;       //Actor's vulnerable location
    Appendage*                  m_leftClaw;         //an extended part of an actor
    CIwFVec2                    m_orignalClawPos;   //'its location
    CIwFVec2                    m_targetPosition;    //target position to attack
    
};

class CrabCreator : public IIwGameXomlClassCreator
{
public:
    CrabCreator() {setClassName("crab");}
    IIwGameXomlResource*    CreateInstance(IIwGameXomlResource * parent) {return new Crab();}
    
};
 */
#endif
