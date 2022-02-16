//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// object_actor.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// GameObjectActor class that players can interact with
//--------------------------------------------------------------------------------------------------------

#include "actors/game_object_actor.h"
#include "sensor_actor.h"
#include "visual_effect.h"
#include "options.h"

GameObjectActor::GameObjectActor(bool destructible, int power)
	: CollidableActor(), BurnableActor(), isLightSource(false), m_isBouncy(false), isHeavy(false), Landed(false), OriginalRestitution(0.0f), OriginalType(-1), m_canHurt(false)
{
	isDestructible	= destructible;
	AttackPower		= power;
    m_resetOnLevelStart  = false;
    
    setActualClassType("gameobjectactor");
    
    //testing
    if(GAME->GetUserOptions()->IsDebugging())
        setDraggable(true);
}

GameObjectActor::~GameObjectActor()
{

}

bool GameObjectActor::Update(float dt)
{
    if(isDestroyed() || !CollidableActor::Update(dt))
        return false;
    
    if(getTimeline() != NULL && !getTimeline()->isPlaying())
        setTimeline(NULL);// remove timeline from object after it has been set
    
	return true;
}

void GameObjectActor::SetBouncy(float32 restitution, bool bouncy)
{ 
	b2Fixture* fixture = getBox2dBody()->getFixture();
    if(fixture->GetRestitution() == restitution)
        return;
    
	fixture->SetRestitution(restitution);

    m_isBouncy = bouncy;
}

void GameObjectActor::SetHeavy(bool heavy) 
{ 
	isHeavy = heavy; 
}

bool GameObjectActor::Init(IIwGameBrush *image, int width, int height, ObjectData& data)
{
    if(!CIwGameActorImage::Init(image, width, height))
       return false;
    
    BurnableActor::Init(1);
    
    CIwGameString   shape       = data.shape;
    CIwGameString   material    = data.material;
    bool            sensor      = false;
    Box2dPhysicsUti    box2d;
    
    //Box2d properties
    box2d.AssignBox2dPhysics(*this, shape.c_str(), material.c_str(), sensor);
    
    // Actor attributes
    setName(data.name.c_str());
    setPosition(data.position);
    setDraggable(false);
    setType(data.type);
    setLayer(data.layer);
    
	setVelocity(0,Velocity.y);
	setAngularVelocity(data.angle_vel);
	OriginalType = Type;
	OriginalRestitution = getBox2dBody()->getFixture()->GetRestitution();

    if(Type == DROP_PLATFORM)
        SetLanded(true);
    
	return true;
}



bool GameObjectActor::UpdateVisual()
{
	return CIwGameActorImage::UpdateVisual();
}

/**
 *  \brief General NotifyCollison for this actor
 *  \param other The other that this actor collided with.
 */
void GameObjectActor::NotifyCollision(CIwGameActor* other)
{
    GameObjectActor* object = (GameObjectActor*)other;
    
    if(other->getType() == FIXTURE || object->HasLanded())
        Landed = true;
}

/**
 * \brief   Explode the actor into pieces
 *          
 */
void GameObjectActor::Explode()
{
    if(!CanBeBlownUp())
        return;
    
    VisualEffect effect;
    
    CIwGameBrushImage* lp; // large piece
    CIwGameBrushImage* sp; // small piece
    const char* sound;
    
    lp = (CIwGameBrushImage*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->FindResource("Brush_Stone", CIwGameXomlNames::Brush_Hash);
    sp = (CIwGameBrushImage*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->FindResource("Brush_Stone_smp", CIwGameXomlNames::Brush_Hash);
    
    sound = "dynamite_explode";
    
    setActive(false);
    
    if(IsDroppedObject())
        setDestroyed(true);
    
    effect.ExplodeActor(this, lp, sp, sound);
}

/**
 *  \brief
 *  TODO: have not been implemented
 */
void GameObjectActor::NotifyCollisionWithBomb(CollidableActor* bomb_actor)
{
    
}

/**
 *  \brief
 */
void GameObjectActor::NotifyCollisionWithParticles(CollidableParticles* fire_breath)
{
    ApplyBurn();
    
    // crumble actor if burn level is less than 1
    if(CanCrumble())
    {
        return;
        //ChangeActorColor(this, BLACK_COLOR);
    }
}

bool GameObjectActor::LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode* node)
{
    if(!CollidableActor::LoadFromXoml(parent, load_children, node))
        return false;
    
    for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
    {
        unsigned int name_hash = (*it)->getName().getHash();
        
        if(name_hash == CIwGameString("landed").getHash())
        {
            bool landed = (*it)->GetValueAsBool();
            
            SetLanded(landed);
        }
        else
        if(name_hash = CIwGameString("resetonlevelstart").getHash())
        {
            bool obs = (*it)->GetValueAsBool();
            m_resetOnLevelStart = obs;
        }
    }
    
    return true;
}

/**
 * \brief Set the object's actual class type hash as "isdropped" is true
 */
void GameObjectActor::SetAsDroppedObject(bool isDropped)
{
    if(isDropped)
        setActualClassType("isdropped");
    else
        setActualClassType("gameobjectactor");
}

/**
 * \brief Checks the object's ActualClassTypeHash
 * \return True if the object's ActualClassTypeHash is = "isdropped"
 */
bool GameObjectActor::IsDroppedObject()
{
    unsigned int is_dropped = CIwGameString("isdropped").getHash();
    
    if(getActualClassTypeHash() == is_dropped)
        return true;
    
    return false;
}

/**
 *  \brief  Returns true if this object can hurt the player
 */
bool GameObjectActor::CanHurtPlayer() const
{
    return m_canHurt;
}

void PlatformActor::ResolveCollisions()
{
    CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();
    
    for(int t = 0; t < started.getSize(); t++)
    {
        CIwGameBox2dCollidable* collision = started.element_at(t);
        if(collision != NULL)
        {
            CollidableActor* actor = (CollidableActor*)collision->getUserData();
            if(actor != NULL)
                actor->NotifyCollisionWithPlatform(this);
        }
    }
}

