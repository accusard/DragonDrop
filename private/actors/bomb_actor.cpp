//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// bomb_actor.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// BombActor class implementation
//--------------------------------------------------------------------------------------------------------

#include "bomb_actor.h"
#include "visual_effect.h"
#include "game_state.h"

BombActor::BombActor() : GameObjectActor(), m_explosionRadius(100.0f)
{
    
}

void BombActor::ResolveCollisions()
{
    CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();
    
	for(int t = 0; t < started.getSize(); t++)
	{
		CIwGameBox2dCollidable* collision = started.element_at(t);
		if(collision != NULL)
		{
			CollidableActor* actor = (CollidableActor*)collision->getUserData();
			if(actor != NULL)
                actor->NotifyCollisionWithBomb(this);
        }
    }
}

/**
 * \brief Sets the countdown timer for the bomb.
 *
 *
 */
void BombActor::SetCountdownTimer(uint64 time)
{
    if(Timeline != NULL)
        Timeline->play();

    setTimeline(IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->findTimeline("Detonate"));

	m_countdownTimer.setDuration(time);
}

/**
 * \brief Get the timer of the object.
 */
GameTimer* BombActor::GetObjectTimer()
{
    if(m_countdownTimer.HasTimedOut())
        return NULL;
    
    return &m_countdownTimer;
}

bool BombActor::Update(float dt)
{
    if( !GameObjectActor::Update(dt) )
        return false;
    
    if(m_countdownTimer.HasTimedOut())
    {
        //Explode actor
        Explode();
    }
	
    return true;
}

void BombActor::Explode()
{
    float explosion_radius = m_explosionRadius;
    
    Explosion* explosion = new Explosion();
	explosion->setScene(Scene);
	explosion->setPosition(Position);
	explosion->Init(explosion_radius);
	Scene->addActor(explosion);
    
    CIwGameActorParticles* p = ((GameScene*)Scene)->ExplodeActor(this, 15);
    setDestroyed(true);
}

/**
 * \brief This function produces simple smoke-like effect that disipates into the air.
 *
 * \param num_particles The number of particles for each cycle.
 * \param brush_name The pointer to the name of the image for the sizzle out effect
 * \param x The x position of the particle effect.
 * \param y The y position of the particle effect.
 * \param delay The time between each particle effect spawns.
 */
void BombActor::SizzleOut(int num_particles, const char* brush_name,
                          float x, float y, float delay)
{
    VisualEffect black_smoke_effect, white_smoke_effect;
    
    CIwFVec2 sr      = CIwFVec2(0.1f,0.3f); // scale range
    CIwFVec4 vr      = CIwFVec4(-1,1,0,0);  // velocity range
    CIwFVec4 cv      = CIwFVec4(0,0,0,-5);  // color velocity
    
    // create black and white smokes
    white_smoke_effect.InitWithSmokeEffects(getScene(), num_particles, delay, WHITE_COLOR, cv, sr, vr,0);
    black_smoke_effect.InitWithSmokeEffects(getScene(), num_particles/3, delay, BLACK_COLOR, cv, sr, vr,0);
    
    // generate the random smoke effects
    white_smoke_effect.GenerateRandomParticles(brush_name, L_ACTORS, x, y);
    black_smoke_effect.GenerateRandomParticles(brush_name, L_ACTORS, x, y);
    
    this->setDestroyed(true);
}

/**
 *  \brief General NotifyCollison for this actor
 *  \param other The other that this actor collided with.
 */
void BombActor::NotifyCollision(CIwGameActor* other)
{
    setTimeline(IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->findTimeline("Detonate"));
    if(!Landed)
    {
        Timeline->restart();
    }
    GameObjectActor::NotifyCollision(other);
    
    // explode if collided with an baseactor entity or another bomb
    if(other->getActualClassTypeHash() == CIwGameString("baseactor").getHash() || other->getType() == DROP_BOMB)
        SetCountdownTimer(100);
    else
        SetCountdownTimer(2000);
}

/**
 *  \brief Notify this actor that it has collided with a stone.
 *
 *  \param stone The bomb that this actor collided with.
 */
void BombActor::NotifyCollisionWithStone(CollidableActor* stone)
{
    // call general NotifyCollision first
    NotifyCollision(stone);
    
    float pos_x, pos_y;
    
    pos_x = getPosition().x;
    pos_y = getPosition().y;
    
    int bomb_orientation = FindActorOrientationTo(stone, ActorOrientation::Y_AXIS);
    
    // sizzle out the bomb if it was below the stone at collision
    if(bomb_orientation == ActorOrientation::BELOW && HasLanded())
    {
        SizzleOut(13, "Brush_Generic_Circle", pos_x, pos_y, 1.0f);
        return;
    }

}
