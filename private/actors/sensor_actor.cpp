//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// sensor_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// Implementation of base actor class to all other actors with movements
//--------------------------------------------------------------------------------------------------------

#include "sensor_actor.h"
#include "game_object_actor.h"
#include "player_actor.h"

//--------------------------------------------------------------------------------------------------------
// Implementation of SensorActor class
//--------------------------------------------------------------------------------------------------------

SensorActor::SensorActor() : CollidableActor(), Contacted(false), AttachedTo(NULL), m_dmgType(-1000)
{
    setActualClassType("sensoractor");
    m_resetOnLevelStart = true;
}
void SensorActor::Init(BaseActor* actor)
{
    if(actor != NULL)
    {
        SetAttachedTo(actor);
    }
    
    setLayer(L_ACTORS);
    //getBox2dBody()->getFixture()->SetSensor(true);
    
}

void SensorActor::Init(BaseActor* actor, CIwGameBrushImage* image)
{
    Box2dPhysicsUti box2d;
    
    Init(actor);
    
    CIwVec2 size = getSize();
    Init(image, size.x, size.y);
    box2d.ShowVisualCollision(*this);
}

bool SensorActor::Update(float dt)
{
    //update the sensor position when dragging around the game scene
    UpdatePosition();
    
    if(getLinkedTo() != NULL)
    {
        if(HasContacted())
            NotifyLinkedActorMadeContact();
        else
            NotifyLinkedActorLostContact();
    }
    
    return CIwGameActorImage::Update(dt);
}

/*
void SensorActor::NotifyLinkedActor(int notifyType)
{
    CIwGameError::LogError("Base class SensorActor::NotifyLinkedActor called..");
}
*/
BaseActor* SensorActor::CheckForContact()
{
    CIwGameSlotArray<CIwGameBox2dCollidable*>& contacts = Box2dBody->getCollisionContacts();
    
    for(int i = 0; i < contacts.getSize(); i++)
    {
        CIwGameBox2dCollidable* active_contact = contacts.element_at(i);
        if(active_contact != NULL)
        {
            BaseActor* actor = (BaseActor*)active_contact->getUserData();
            
            if(actor != NULL)
                return actor;
        }
        else
        {
            contacts.compact();
            return NULL;
        }
        
    }
    
    CIwGameError::LogError("SensorActor::CheckForContact is false");
    return NULL;
}


void SensorActor::ResolveCollisions()
{
    if(!isActive() || Box2dBody == NULL)
        return;
    
    CIwGameSlotArray<CIwGameBox2dCollidable*>& contacts = Box2dBody->getCollisionContacts();
    
    for(int i = 0; i < contacts.getSize(); i++)
    {
        CIwGameBox2dCollidable* active_contact = contacts.element_at(i);
        if(active_contact != NULL)
        {
            if(!Contacted)
            {
                SetContacted();
                break;
            }
        }
        else
        {
            if(Contacted)
            {
                SetContacted(false);
                contacts.compact();
                break;
            }
        }
    }
    
    /*
    CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();
    
	for(int t = 0; t< started.getSize(); t++)
	{
		CIwGameBox2dCollidable* collision = started.element_at(t);
		if(collision != NULL)
		{
			CIwGameActor* actor = (CIwGameActor*)collision->getUserData();
			if(actor != NULL)
			{
				//call the other actor's NotifyCollision that it has collided with this actor
				actor->NotifyCollision(this);
			}
		}
	}
    */
}

void SensorActor::NotifyCollision(SensorActor* other)
{
    if(!HasContacted())
        SetContacted();
    

}

bool SensorActor::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
    if(!CIwGameActorImage::LoadFromXoml(parent, load_children, node))
        return false;
    
    unsigned int value;
    
    for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); ++it)
    {
        unsigned int attrib_name_hash = (*it)->getName().getHash();
        
        if(attrib_name_hash == CIwGameXomlNames::Type_Hash)
        {
            value = (*it)->GetValue().getHash();
            
            if(value == CIwGameString("weakness").getHash())
            {
                setType(WEAKNESS_SENSOR);
                CIwGameError::LogError("Setting weakness type");
            }
        }
        else
        if(attrib_name_hash == CIwGameString("dmgtype").getHash())
        {
            value = (*it)->GetValue().getHash();
            
            if(value == CIwGameString("normal").getHash())
            {
                SetDmgType(DMG_NORMAL);
                CIwGameError::LogError("Setting dmg type");
            }
        }
            
    }
    return true;
}

bool SensorActor::PostLoadFromXoml(IIwGameXomlResource* parent, CIwGameXmlNode* node)
{
    
    return true;
}

void SensorActor::UpdatePosition()
{
    if(!isActive() || getLinkedTo() == NULL)
        return;
    
    float linked_x = getLinkedTo()->getPosition().x;
    float linked_y = getLinkedTo()->getPosition().y;
    
    CIwFVec2 pos = ((GameScene*)Scene)->HudToMainScene(linked_x, linked_y);

    setPosition(pos.x, pos.y);
}

void SensorActor::NotifyLinkedActorMadeContact()
{
    if(AttachedTo != NULL && AttachedTo->isJumping())
    {
        AttachedTo->SetJumping(false);
        AttachedTo->SetActorFriction(0.9f);
    }
}
void SensorActor::NotifyLinkedActorLostContact()
{
    if(AttachedTo != NULL && !AttachedTo->isJumping())
    {
        AttachedTo->SetJumping();
        AttachedTo->SetActorFriction(0.0f);
    }
}

//--------------------------------------------------------------------------------------------------------
// SensorColorActor implementations
//--------------------------------------------------------------------------------------------------------

void SensorColorActor::Init(BaseActor* actor, CIwGameAnimTimeline* timeline)
{
    SensorActor::Init(actor);
    BlinkingTimeline = timeline;
    
    //SensorColorActor have no visual sprites
    CIwGameActor::Reset();
    CIwGameDummySprite* sprite = new CIwGameDummySprite();
    sprite->Init();
    Visual = sprite;
    Scene->getSpriteManager()->addSprite(sprite);
    
}

bool SensorColorActor::Update(float dt)
{
    return SensorActor::Update(dt);
}

/**
 * \brief
 * TODO:	rewrite SensorColorActor::ResolveCollisions to use Overlap() function
 */
void SensorColorActor::ResolveCollisions()
{
    SensorActor::ResolveCollisions();
}

void SensorColorActor::NotifyLinkedActorMadeContact()
{
    if(getLinkedTo()->getTimeline() != BlinkingTimeline)
        getLinkedTo()->setTimeline(BlinkingTimeline);
    
    if(!BlinkingTimeline->isPlaying())
        BlinkingTimeline->restart();
}
void SensorColorActor::NotifyLinkedActorLostContact()
{
    if(getLinkedTo()->getTimeline() != NULL)
    {
        if(!BlinkingTimeline->isPlaying())
            getLinkedTo()->setTimeline(NULL);
    }
}

//--------------------------------------------------------------------------------------------------------
// SensorPropertyActor implementations
//--------------------------------------------------------------------------------------------------------

bool SensorPropertyActor::Update(float dt)
{
    if(!SensorActor::Update(dt))
        return false;
    
    if(HasContacted())
        NotifyLinkedActorMadeContact();
    else
        NotifyLinkedActorLostContact();
    
    return true;
}

bool SensorPropertyActor::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
    if(!SensorActor::LoadFromXoml(parent, load_children, node))
       return false;
    
    GameScene* scene = (GameScene*)GAME->findScene("MainScene");
    setPosition(scene->GetPlayerStartPos());
    
    
    return true;
}

void SensorPropertyActor::ResolveCollisions()
{
    SensorActor::ResolveCollisions();
}

void SensorPropertyActor::NotifyCollision(CIwGameActor* other)
{

}

/**
 *  \brief Notify the base of the player that it has collided with a trampoline.
 *  \param trampoline_actor The trampoline that this actor collided with.
 */
void SensorPropertyActor::NotifyCollisionWithTrampoline(CollidableActor* trampoline_actor)
{
    GameObjectActor*    gameobject  = (GameObjectActor*)trampoline_actor;
    
    if(gameobject->IsBouncy())
    {
        PlayerActor* player = (PlayerActor*)GetAttachedTo();
        b2Vec2 vel = player->GetCurrentVelocity();
        vel.y = -30;
        player->setVelocitybVec2(vel);
    }
    
}
//--------------------------------------------------------------------------------------------------------
// SensorWeaknessActor implementations
//--------------------------------------------------------------------------------------------------------

void SensorWeaknessActor::ResolveCollisions()
{
    CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();
    
	for(int t = 0; t< started.getSize(); t++)
	{
		CIwGameBox2dCollidable* collision = started.element_at(t);
		if(collision != NULL)
		{
			CIwGameActor* actor = (CIwGameActor*)collision->getUserData();
			if(actor != NULL)
			{
				//call the other actor's NotifyCollision that it has collided with this actor
				actor->NotifyCollision(this);
			}
		}
	}
}

void SensorWeaknessActor::NotifyCollision(CIwGameActor* other)
{
    /*
    SensorActor* sensor = ((SensorActor*)other);
    
    if(sensor->GetDmgType() == DMG_IMPACT || other->getType() == DROP_STONE)
       return;
       
    BaseActor* attached_actor = ((SensorActor*)other)->GetAttachedTo();
    
    if(attached_actor != NULL)
    {
        ((GameScene*)getScene())->StartCollisionEvent(CAN_HIT, attached_actor, ACTOR_DEFEND);
    }
     */
}

bool SensorWeaknessActor::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
    if(!SensorActor::LoadFromXoml(parent, load_children, node))
        return false;
    
    unsigned int value;
    
    for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); ++it)
    {
        unsigned int attrib_name_hash = (*it)->getName().getHash();
        
        if(attrib_name_hash == CIwGameString("weaktodmg").getHash())
        {
            value = (*it)->GetValue().getHash();
            if(value == CIwGameString("dmgnormal").getHash())
                m_weakTo = DMG_NORMAL;
            else
            if(value == CIwGameString("dmgimpact").getHash())
                m_weakTo = DMG_IMPACT;
            
        }
    }
    return true;
    
}
