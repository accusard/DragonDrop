//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// enemy_actor.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// EnemyActor class implementation
//--------------------------------------------------------------------------------------------------------

#include "actors/enemy_actor.h"
#include "player_actor.h"
#include "target_movement.h"
#include "visual_effect.h"

EnemyActor::EnemyActor() :
BaseActor(),
m_pointOfAttack(-1,-1),
m_activeRange(0,0,0,0),
Flying(false),
m_attackSpeed(0.1f),
m_attackRange(200.0f)
{
    m_resetOnLevelStart = true;
}

bool EnemyActor::Update(float dt)
{
    if(IsOffScreen())
        setActive(false);
    
    
    return BaseActor::Update(dt);
}

/**
 * \brief Find the closest target of a certain type and checks if it is in range.
 * \return the pointer to a CIwGameActor object. NULL if a target is out of range or the type specified
 * is less than 0.
 */
CIwGameActor* EnemyActor::AcquireTarget(const int type)
{
    //find a target if he is in active range
    CIwGameActor* target = Scene->findClosestActor(Position.x, Position.y, type);
    
    if(target->isActive() && TargetInRange(target))
    {
        return target;
    }
    
    return NULL;
}

/**
 * \brief Checks a set of minimum and maximum range to see if the target is within m_activeRange of this actor.
 *
 * Currently only checks the horizontal positions of the target.
 *
 * \return true if the target is within the range of this actor.
 */
bool EnemyActor::TargetInRange(CIwGameActor* target)
{
    bool in_range = false;
    
    CIwFVec2 enemy_pos	= this->getPosition();
    CIwFVec2 target_pos	= target->getPosition();
    
    float min_range		= enemy_pos.x - m_activeRange.x;
    float max_range		= enemy_pos.x + m_activeRange.w;
    
    if(target_pos.x >= min_range && target_pos.x <= max_range)
    {
        in_range = true;
    }
    
    return in_range;
}

bool EnemyActor::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
	if (!BaseActor::LoadFromXoml(parent, load_children, node))
		return false;    

    //default values
    CIwRect active_range;
    
    /*
    //find the patrol timeline "patrol<num>"
    CIwGameString string = "patrol";
    CIwGameString enemy_num = getName();
    enemy_num.Replace("Enemy", "");
    
    string += enemy_num;
    
    PatrolTimeline = (CIwGameAnimTimeline*)Scene->getTimelinesManager()->findTimeline(string.c_str());
    */
	 // Custom XOML Tags
	for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
        unsigned int name_hash = (*it)->getName().getHash();
        if(name_hash == CIwGameString::CalculateHash("flying"))
        {
            bool flying = (*it)->GetValueAsBool();
            Flying = flying;
        }
        else
        if(name_hash == CIwGameString::CalculateHash("activerange"))
        {
            (*it)->GetValueAsRect(active_range);//(active_range);
        }
        else
        if(name_hash == CIwGameString::CalculateHash("attackspeed"))
        {
            m_attackSpeed = (*it)->GetValueAsFloat();
        }
	}

    //assign values
    m_activeRange = active_range;
    
	return true;
}

void EnemyActor::ResolveCollisions()
{
    CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();
    
    for(int t = 0; t < started.getSize(); t++)
    {
        CIwGameBox2dCollidable* collision = started.element_at(t);
        if(collision != NULL)
        {
            CollidableActor* actor = (CollidableActor*)collision->getUserData();
            if(actor != NULL)
                actor->NotifyCollisionWithEnemy(this);
        }
    }
}

void EnemyActor::NotifyCollisionWithSword(CIwGameActor* sword)
{
    if(!CanBeHurt())
        return;
    
    // check sprite collision
    //bool collided = CheckCollisionRect(sword);
    
    //if(collided)
    //{
        ApplyDamage(1);
    //}
    
    VisualEffect effect;
    
    effect.SliceActor(this, 5, 5);
    
    setDestroyed(true);
}

void EnemyActor::OnTapped(int index, int x, int y, bool allow_bubble)
{
    PlayerActor* player = (PlayerActor*)GAME->findScene("MainScene")->findActor("Player");
    
    if(!player->IsActionBarActive() || !player->HasItem(SWORD))
        return;
    
    // TODO: #B5893 implement sword attack here
    /*

     */
}

void EnemyActor::Jump()
{
	setVelocityY(JUMPING_HEIGHT);
}

void EnemyActor::SetPointOfAttack(float point_x, float point_y)
{
    m_pointOfAttack.x = point_x;
    m_pointOfAttack.y = point_y;
}

/**
 * \brief A point of attack is a position of type CIwFVec where the actor will
            move to before it begin its attack
 */
CIwFVec2 EnemyActor::CreatePointOfAttack()
{
    return CIwFVec2(-1,-1);
}

/**
 * \brief Move the actor towards the point of attack.
 */
void EnemyActor::MoveToPointOfAttack()
{
    // point of attack have not been set
    if(m_pointOfAttack.x == -1 && m_pointOfAttack.y == -1)
        return;
    
    // move towards the point of attack
    CIwFVec2 pos(m_pointOfAttack.x, m_pointOfAttack.y);
    float max_spd = GetMaxSpeed();
    MoveToTarget(pos, max_spd);
}
