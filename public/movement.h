//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// movement.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------

#if !defined(_MOVEMENT_H_)
#define _MOVEMENT_H_


#include "IwGeomFVec2.h"
#include "observer.h"

class BaseActor;
class Observer;


/**
 * \brief The base class of movement
 *
 * TODO: #I8923 use data member m_ignoreCollision to move the actor without using Box2d
 */
class Movement
{
    
public:
    
private:
    bool            m_ignoreCollision;  /**< True for movements that can move freely without collision */
    bool            m_isFinished;       /**< True if actor has arrived within the range of target position */
    
protected:
    BaseActor*      m_actor;            /**< The target actor that this movement applies to. */
    float           m_moveSpeed;        /**< The speed of this movement that will be applied to the actor's
                                                velocity*/
    float           m_moveSpeedModifer; /**< Addtional modifer to this actor's max_speed */

public:
    Movement(BaseActor* actor,
             bool ignore_collision, const float speed_mod);
    
    virtual ~Movement();
    
    void SetActor(BaseActor* actor);
    void SetActorVelocityX(float vel_x, const int dir);
    void SetMovementFinished(const bool finished = true);
    bool HasFinishedMoving() const {return m_isFinished;}
    void NotifyMovementFinished();
    
    virtual bool Update();
    virtual void Right();
    virtual void Left();
};

#endif
