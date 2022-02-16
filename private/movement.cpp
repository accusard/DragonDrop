//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// movement.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Movement class implementation
//--------------------------------------------------------------------------------------------------------
//#include "observer.h"
#include "movement.h"
#include "base_actor.h"

/**
 * \brief This is the base class that controls the movement of an actor
 * \param actor The actor that this movement applies to
 * \param ignore_collisoin Assign to true if this movement should ignore any collisions made by Box2d
 */
Movement::Movement(BaseActor* actor, bool ignore_collision, const float speed_mod) :
                    m_actor(actor),
                    m_ignoreCollision(ignore_collision),
                    m_isFinished(false),
                    m_moveSpeed(0.0f),
                    m_moveSpeedModifer(speed_mod)
{}

/**
 * \brief The destructor
 */
Movement::~Movement() {}

/**
 * \brief Updates the actor that is attached to this movement
 * \return Returns true and continiously update as long as m_isFinished is false.
 */
bool Movement::Update()
{
    if(m_isFinished)
    {
        //m_moveSpeed = 0.0f;
        NotifyMovementFinished();
        return false;
    }
    //m_actor->UpdateMovement();
    
    return true;
}

/**
 * \brief Sets the t_facing of the actor and flip the actor's image as necessary.
 */
void Movement::Right()
{
    m_actor->SetFacing(BaseActor::FACING_RIGHT);
    m_actor->FlipActor();
}

/**
 * \brief Sets the t_facing of the actor and flip the actor's image as necessary.
 */
void Movement::Left()
{
    m_actor->SetFacing(BaseActor::FACING_LEFT);
    m_actor->FlipActor();
}

/**
 * \brief Sets the actor that this movement will apply to
 * \param The actor that this movement will apply to
 */
void Movement::SetActor(BaseActor* actor)
{
    m_actor = actor;
}

/**
 * \brief Sets the target actor's horizontal velocity through Box2d but keep its vertical velocity unchanged.
 */
void Movement::SetActorVelocityX(float vel_x, const int dir)
{
    if(dir == BaseActor::NONE)
        return;
    
    b2Vec2 vel = m_actor->getBox2dBody()->getBody()->GetLinearVelocity();
    
    vel_x *= dir;
    m_actor->setVelocity(vel_x, vel.y);
    
}

/**
 * \brief Sets this Movement as finished.
 */
void Movement::SetMovementFinished(const bool finished)
{
    m_isFinished = finished;
}

/**
 * \brief Notify this movement and the target actor that it has finished moving.
 */
void Movement::NotifyMovementFinished()
{
    if(m_actor != NULL)
        m_actor->NotifyMovementFinished();
}

