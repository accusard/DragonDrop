//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// target_movement.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  TargetMovement implementation
//--------------------------------------------------------------------------------------------------------
#include "base_actor.h"
#include "target_movement.h"
/**
 *\brief Function will continously update until target actor arrive at or passes its target position.
 */
bool TargetMovement::Update()
{
    if(!Movement::Update())
        return false;
    
    // get current actor pos
    CIwFVec2 curr_actor_pos = m_actor->getPosition();
    
    // apply x-coordinate movements
    if(curr_actor_pos.x != this->m_targetPos.x)
    {
        //determine orientation
        ActorOrientation orient(m_actor);
        int actor_is_located = orient.FindOrientationFromX(m_targetPos.x);
        
        // get speed of actor and add any modification
        float max_speed = m_moveSpeedModifer;
        m_moveSpeed += m_actor->GetSpeed();
        
        m_actor->SetAccelerated(m_actor->LimitSpeed(m_moveSpeed, max_speed));
        
        if(m_actor->GetFacing() == BaseActor::NONE)
        {
            if(actor_is_located == ActorOrientation::RIGHT)
            {
                // actor is located to the right so go left
                Left();
            }
            else
            if(actor_is_located == ActorOrientation::LEFT)
            {
                // actor is located to the left so go right
                Right();
            }
        }
        
        SetActorVelocityX(m_moveSpeed, m_actor->GetFacing());
        
        // checks if actor has arrived at target position
        if(HasArrivedAtTargetPos())
            SetMovementFinished();
        }
    
    return true;
}

/**
 * \brief Checks to see if the current movement has arrived at the target position.
 * \return Returns true if the actor has arrived at the targeted position.
 */
bool TargetMovement::HasArrivedAtTargetPos()
{
    int facing = m_actor->GetFacing();
    bool has_arrived = false;
    
    switch (facing)
    {
        case BaseActor::FACING_LEFT:
            if(m_actor->getPosition().x <= m_targetPos.x)
                has_arrived = true;
            break;
            
        case BaseActor::FACING_RIGHT:
            if(m_actor->getPosition().x >= m_targetPos.x)
                has_arrived = true;
            break;
    }
    
    return has_arrived;

}

TargetMovement::TargetMovement(BaseActor* actor,
                               bool ignore_collision,
                               CIwFVec2& target_pos,
                               const float speed_mod)
                : Movement(actor, ignore_collision, speed_mod), m_targetPos(target_pos) {}

TargetMovement::~TargetMovement() {}

/**
 * \brief Assign an x value to the coordinates of m_targetPos
 */
void TargetMovement::SetTargetPosX(const float x)
{
    m_targetPos.x = x;
}

/**
 * \brief Assign a y value to the coordinates of m_targetPos
 */
void TargetMovement::SetTargetPosY(const float y)
{
    m_targetPos.y = y;
}
