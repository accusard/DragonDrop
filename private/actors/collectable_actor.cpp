//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// collectable_actor.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#include "actors/collectable_actor.h"
#include "player_actor.h"

//
//
//
//  Base CollectableActor definitions
//
//
//

CollectableActor::CollectableActor() : m_holdToCollect(false), Collected(false), Value(0), HUDText(NULL)
{
    m_resetOnLevelStart = true;
}
CollectableActor::~CollectableActor() {}

void CollectableActor::Init(int _id)
{
    m_identifier = _id;
    
    setActualClassType("collectable");
    if(GAME->GetUserOptions()->IsDebugging())
        setDraggable(true);
    
    
}

bool CollectableActor::HoldToCollect() const
{
    return m_holdToCollect;
}

void CollectableActor::SetHoldToCollect(bool hold)
{
    m_holdToCollect = hold;
}
void CollectableActor::SetCollected(bool collected)
{
	Collected = collected;
}

void CollectableActor::SetID(int _id)
{
    m_identifier = _id;
}

/**
 * \brief Function sets the Collected value to the value held by m_holdToCollect and then returns the item's type
 * \brief Do not collect by returning -1
 */
int CollectableActor::Collect()
{
	if(Collected)
		return -1;
		
	Collected = m_holdToCollect;
	
	return Type;
}

bool CollectableActor::Update(float dt)
{
	if(Collected || m_holdToCollect)// && !Timeline->isPlaying())
		setActive(false);
		//return false;

	return CIwGameActorImage::Update(dt);
}

void CollectableActor::ResolveCollisions()
{	
	if(Box2dBody == NULL)
		return;

	CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();

	for(int t = 0; t< started.getSize(); t++)
	{
		CIwGameBox2dCollidable* collision = started.element_at(t);
		if(collision != NULL)
		{
			CollidableActor* actor = (CollidableActor*)collision->getUserData();
			if(actor != NULL)
			{
				actor->NotifyCollisionWithCollectable(this);
			}
		}
	}
}

void CollectableActor::NotifyCollisionWithCollectable(CollidableActor* other)
{
    if(other->getType() == ENEMY)
        return;
    
	if(!m_holdToCollect && other->getType() == PLAYER)
		m_holdToCollect = true;
    
    CIwGameString string = "Acquired ";
    /*
    if(other->getType() != PLAYER)
    {
        string = other->getName();
        string += " has acquired ";
    }
    */
    string += getName();
    
    HUDText->SetTextDisplay(string.c_str());
}

bool CollectableActor::LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode* node)
{
	if (!CIwGameActorImage::LoadFromXoml(parent, load_children, node))
		return false;
    
    int _id = -1;
    
    for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); ++it)
    {
        unsigned int name_hash = (*it)->getName().getHash();
        if(name_hash == CIwGameXomlNames::Layer_Hash)
        {
            CIwGameString layer = (*it)->GetValue();
            if(layer == CIwGameString("hidden"))
            {
                if(GAME->GetUserOptions()->IsDebugging())
                    CIwGameError::LogError("assigning L_HIDDEN to actor layer");
                
                setLayer(L_HIDDEN);
            }
            else
                setLayer(L_COLLECTIBLES);
        }
        else
        if(name_hash == CIwGameString("id").getHash())
        {
            _id = (*it)->GetValueAsInt();
        }
    }
    
    Init(_id);
    
    // set up a link to the HUDText for on screen text display
    HUDScene* hudscene = (HUDScene*)GAME->findScene("HUD");
    HUDTextDisplay* hudtext = ((HUDScene*)GAME->findScene("HUD"))->GetHUDText();
    HUDText = hudtext;
    
    return true;
}

/**
 * \brief Returns the ID of the current CollectableActor object.
 */
int CollectableActor::GetID() const
{
    return m_identifier;
}

//
//
//
//  MapActor class definitions
//
//
//

void MapActor::NotifyCollisionWithCollectable(CollidableActor* collectable_actor)
{
    if(collectable_actor->getType() == PLAYER)
        IW_GAME_AUDIO->PlaySound("map_pickup");
    
    CollectableActor::NotifyCollisionWithCollectable(collectable_actor);
}

//
//
//
//  OrbActor class definitions
//
//
//

void OrbActor::NotifyCollision(CIwGameActor* other)
{
    if(other->getType() == PLAYER)
    {
        IW_GAME_AUDIO->PlaySound("orb_pickup");
        CollectableActor::NotifyCollision(other);
    }
}
