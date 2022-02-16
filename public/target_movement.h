//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// target_movement.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------

#if !defined(_TARGET_MOVEMENT_H_)
#define _TARGET_MOVEMENT_H_

#include "movement.h"

/**
 * \brief A class that uses CIwFVec2 to move the actor to a target position.
 *
 * The class will utilize the target actor's Box2d to move it towards the target
 * position.
 */
class TargetMovement : public Movement
{
    using Movement::Right;
    using Movement::Left;
    
private:
    CIwFVec2        m_targetPos;            /**< The target x and y coordinates of this movement. */
    
public:
    TargetMovement(BaseActor* actor,
                   bool ignore_collision,
                   CIwFVec2& target_pos,
                   const float speed_mod);
    
    ~TargetMovement();
    
    bool Update();
    void Right(float vel_x);
    void Left(float vel_x);
    
    void SetTargetPosX(const float x);
    void SetTargetPosY(const float y);
    
    bool HasArrivedAtTargetPos();
};

#endif
