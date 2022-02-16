//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// boss_dragon.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  BossDragon class implementation
//--------------------------------------------------------------------------------------------------------

#include "boss_dragon.h"
#include "target_movement.h"
#include "visual_effect.h"

BossDragon::BossDragon()  : m_breathAttack(NULL) {}

BossDragon::~BossDragon() {}

bool BossDragon::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
    if(!EnemyActor::LoadFromXoml(parent, load_children, node))
        return false;
    
    Init();
    
    return true;
}

/**
 * \brief Initialize the actor and sets its fire breath image.
 *
 * Will find the closest anchor and set it as a point of attack. Use a BodyPart object to determine the 
 * offset position of the actor's mouth. It will also load State::Idle and State::Moving classes to the 
 * actor's list of states.
 *
 * TODO: #V3899 make sure Init doesn't get called twice, possibly through LoadXoml and base class
 * CIwGameActor.
 *
 */
void BossDragon::Init()
{
    CIwFVec2 mouth_offset(0,0);
    
    // local variables' assignments
    mouth_offset.x  = ((CIwGameBrushImage*)getVisual()->getBrush())->getSrcRect().w /2;
    mouth_offset.y  = 0.0f;
    
    // create actor's body parts
    BodyPart mouth(this, "mouth", mouth_offset.x, mouth_offset.y);
    
    // data members' assignments
    m_mouth             = mouth;
    
    // animation
    CIwGameAnimTimeline* idle_t = Animate.Idle;
    CIwGameAnimTimeline* move_t = Animate.Moving;
    
    // add the Idle and Moving states to this actor
    m_actorStateManager->AddState(new class LookingForTarget(this, idle_t, PLAYER), true);
    m_actorStateManager->AddState(new class Moving(this, move_t),false);
}

void BossDragon::Init(const char* img_name, float init_poa, BodyPart mouth)
{
    
}

/**
 * \brief Notifies this actor that a target has been acquired.
 *
 * Once it has acquired a target, it will start to move to its initial point of attack.
 */
void BossDragon::NotifyTargetAcquired()
{
    // move towards the point of attack
    CIwFVec2 point = CreatePointOfAttack();
    
    SetPointOfAttack(point.x, point.y);
    MoveToPointOfAttack();

}

/**
 * \brief Notify the actor that it has finished moving.
 *
 * Once it has finish moving, the actor will change its state to an Attack state.
 */
void BossDragon::NotifyMovementFinished()
{
    EnemyActor::NotifyMovementFinished();
    
    // decide if it should scan for target or charge at player
    CIwGameActor* target = AcquireTarget(PLAYER);
    
    if(target != NULL && target->isActive())
    {
        m_actorStateManager->OnNotify(this, EVENT_START_ATTACK);
    }
}

/**
 * \brief Sets the duration of the actor's main attack.
 * \param duration The duration of the attack in milliseconds.
 */
void BossDragon::SetBreathDuration(const uint64 duration)
{
    m_fireBreathTimer.setDuration(duration);
}

/**
 * \brief The actor's main attack.
 *
 * This function is called by the actor's UpdateAttack and will be called continously until its duration
 * runs out.
 *
 * \param num_particles The number of particles in this effect.
 * \param x The x-coordinate position.
 * \param y The y-coordinate position.
 * \param knockback The float value of the distance the dragon will get knocked back for each breath
 * \param sr The scale range of each particle effect
 * \param vr The velocity range of each particle effect
 * \param cv The color velocity which effect the fade time of each particle
 * \param delay The time delay between each particle spawn
 */
void BossDragon::BreatheFire(const int num_particles,
                             float x, float y,
                             const CIwFVec2& sr, const CIwFVec4& vr, const CIwFVec4& cv,
                             float delay)
{
    // create the fire-breathing visual effect and actor entity
    VisualEffect fire_breath;
    
    fire_breath.InitWithSmokeEffects(getScene(), num_particles, delay, RED_COLOR, cv, sr, vr, 600);
    fire_breath.GenerateRandomParticles("Brush_Generic_Circle", L_ACTORS, x, y);
    
    m_breathAttack = fire_breath.GetVisualParticles();
    m_breathAttack->setType(FIREBREATH);
    
    //fire_breath.Init(getScene(), num_particles);
    //fire_breath.GenerateCollidableParticles("Brush_Generic_Circle", L_ACTORS, x, y);
    

}

bool BossDragon::Update(float dt)
{
    if(!EnemyActor::Update(dt))
        return false;
    
    return true;
}

/**
 * \brief The initial stage of the attack called by its state->Load().
 *
 * This actor's StartAttack function will set its breath duration.
 */
void BossDragon::StartAttack()
{
    SetBreathDuration(5000);
    
    int p       = 300;                   // number of particles
    CIwFVec2 sr = CIwFVec2(1,2);         // the scale range of the breath
    CIwFVec4 vr = CIwFVec4(-15,2,0,15);  // the velocity range of the breath
    CIwFVec4 cv = CIwFVec4(0,0,0,-5);    // the color fade of the breath
    float d     = 5.0f;                  // the delays between each particles
    
    BreatheFire(p, m_mouth.GetPos().x, m_mouth.GetPos().y, sr, vr, cv, d);
}

void BossDragon::StopAttack()
{
    if(m_breathAttack != NULL)
    {
        m_breathAttack->setActive(false);
        m_breathAttack->setDestroyed(true);
    }
}
/**
 * \brief The attack implementation of this actor.
 *
 * As long as the state of the actor is allowed to attack, the function will be called multiple times.
 *
 * \param actor The pointer to the target of the attack.
 * \return true if the state of the actor allow it to attack and the pointer to the actor does not equal
 * to NULL.
 */
bool BossDragon::UpdateAttack(CIwGameActor* target)
{
    if(!EnemyActor::UpdateAttack(target))
        return false;
    
    m_breathAttack->setPosition(m_mouth.GetPos().x, m_mouth.GetPos().y);
    if(m_fireBreathTimer.HasTimedOut())
    {
        // change state
        //CIwGameError::LogError("Actor Idle");
        m_actorStateManager->OnNotify(this, EVENT_TARGET_LOST);
        return true;
    }
    
    // // knock back the actor for each breath
    static float kb    = 1.0f;
    
    KnockBack(kb);
    
    return true;
}

void BossDragon::NotifyCollisionWithStone(CollidableActor* stone)
{
    VisualEffect eff;
    
    CIwGameBrushImage* large_piece; // large piece
    CIwGameBrushImage* small_piece; // small piece
    const char* sound;
    
    large_piece = (CIwGameBrushImage*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->FindResource("Brush_Stone", CIwGameXomlNames::Brush_Hash);
    small_piece = (CIwGameBrushImage*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->FindResource("Brush_Stone_smp", CIwGameXomlNames::Brush_Hash);
    
    sound = "dynamite_explode";
    
    eff.ExplodeActor(stone, large_piece, small_piece, "sound");
    stone->setDestroyed(true);
}

void BossDragon::NotifyCollisionWithPlatform(CollidableActor* platform)
{
    VisualEffect eff;
    
    CIwGameBrushImage* large_piece; // large piece
    CIwGameBrushImage* small_piece; // small piece
    const char* sound;
    
    large_piece = (CIwGameBrushImage*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->FindResource("Brush_Platform", CIwGameXomlNames::Brush_Hash);
    small_piece = (CIwGameBrushImage*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->FindResource("Brush_Stone_smp", CIwGameXomlNames::Brush_Hash);
    
    sound = "dynamite_explode";
    
    eff.ExplodeActor(platform, large_piece, small_piece, "sound");
    platform->setDestroyed(true);
}

void BossDragon::NotifyCollisionWithSpikedBall(CollidableActor *ball)
{
    EnemyActor::NotifyCollisionWithSpikedBall(ball);
    ball->setActive(false);
}

bool BossDragon::MovingDamageOthers()
{
    return true;
}

void BossDragon::ActionsOnNotify(Event event)
{
    switch(event)
    {
        case EVENT_WAS_HURT:
        {
            
            //CIwGameActor* player = Scene->findActor(PLAYER);
            CIwFVec2 target_pos = CIwFVec2(-250,0);
            
            MoveToTarget(target_pos, 25.0f);
            
            break;
        }
    }
}

void BossDragon::NotifyDying()
{
    static_cast<GameScene*>(Scene)->EndOfBoss();
}

/**
 * \brief A point of attack is a position of type CIwFVec where the actor will
 *          move to before it begin its attack
 *
 *          This actor will find the nearest spiked ball location.
 */
CIwFVec2 BossDragon::CreatePointOfAttack()
{
    float   x       = getPosition().x;
    float   y       = getPosition().y;
    int     type    = SPIKED_BALL;
    
    CIwGameActor* spiked_ball = Scene->findClosestActor(x, y, type);
    return spiked_ball->getPosition();
}
