//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// state_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
/********************************************************************************************************
 Jump
 ********************************************************************************************************/

#include "jumping_state.h"
#include "falling_state.h"

Jumping::Jumping(BaseActor* actor,
                 CIwGameAnimTimeline* jump_timeline,
                 float j_height) :
            ActorState(actor, ACTOR_JUMPING, jump_timeline), m_jumpHeightReached(false), m_jumpHeight(j_height)
{}

void Jumping::Load()
{
    //load the animation
    ActorState::Load();
    
    m_jumpHeightReached = false;
    
    //state specific
    IW_GAME_AUDIO->PlaySound("jump");
    
    float j_height = (m_jumpHeight ) * -1;
    GetActor()->setVelocityY(j_height);
}

void Jumping::Close()
{
    ActorState::Close();
}

void Jumping::Update()
{
    ActorState::Update();
    
    if(m_jumpHeightReached)
        GetActor()->GetActorStateManager()->OnNotify(m_actor, EVENT_FALLING);
    
    b2Vec2 vel = GetActor()->GetCurrentVelocity();
    
    if(vel.y > 0.1)
        m_jumpHeightReached = true;
}
