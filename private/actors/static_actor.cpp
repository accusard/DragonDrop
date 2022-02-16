//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// static_actor.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------

#include "actors/static_actor.h"
#include "actors/game_object_actor.h"
#include "actors/base_actor.h"
#include "actors/player_actor.h"

StaticActor::StaticActor() : ResizeFixture(false), MaintainSizeDepth(false)
{}


bool StaticActor::Update(float dt)
{
	if(IsOffScreen())
		return false;
    /*
    if(isDragging() && GAME->GetUserOptions()->IsDebugging())
    {        
    	CIwFVec2 pos = Position;
    	CIwGameString string;
        
        string += getName().c_str();
        string += "'s position: ";
    	string += CIwGameString((int)pos.x).c_str();
    	string += ", ";
    	string += CIwGameString((int)pos.y).c_str();
    	
        GAME->GetHUDText()->SetTextDisplay(string.c_str());
    }
    */
	return CIwGameActorImage::Update(dt);
}

bool StaticActor::UpdateVisual()
{
	return CIwGameActorImage::UpdateVisual();
}
/*
void StaticActor::ResolveCollisions()
{
	
	if(Box2dBody == NULL)
		return;

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
*/
void StaticActor::NotifyCollision(CIwGameActor* other)
{
    if(other->getType() == PLAYER)
    {
        if(getType() == GOAL_FLAG)// && other->getType() == PLAYER)
            ((GameScene*)Scene)->EndOfLevel();
        else
        if(getType() == EVENT_TRIGGER)
        {
            CIwGameString string = getName();
            
            GAME->SetStoryboard(string);
            
            ((GameScene*)Scene)->EndOfBoss();
        }
    }
}



bool StaticActor::LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode* node)
{
    
	if (!CIwGameActorImage::LoadFromXoml(parent, load_children, node))
		return false;
    
	//Custom XOML tags
	for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int attrib_name_hash = (*it)->getName().getHash();
        if(attrib_name_hash == CIwGameString("rotateangle").getHash())
        {
            float angle = (*it)->GetValueAsFloat();
            setAngle(angle);
        }
        else
        if(attrib_name_hash == CIwGameString("maintainsizedepth").getHash())
        {
            MaintainSizeDepth = (*it)->GetValueAsBool();
        }
        else
		if(attrib_name_hash == CIwGameXomlNames::Type_Hash)
		{
			unsigned int type_value_hash = (*it)->GetValue().getHash();
			
            if(type_value_hash == CIwGameString::CalculateHash("spikedball"))
            {
                setType(SPIKED_BALL);
            }
            else
			if(type_value_hash == CIwGameString("goalflag").getHash())
			{
				setType(GOAL_FLAG);
			}
            else
            if(type_value_hash == CIwGameString("eventtrigger").getHash())
            {
                setType(EVENT_TRIGGER);
            }
            else
            if(type_value_hash == CIwGameString("fixture").getHash())
            {
                setType(FIXTURE);
                
                if(GAME->GetUserOptions()->ShowFixture())
                {
                    int w, h;
                    CIwGameBrushImage* image = (CIwGameBrushImage*)Scene->getResourceManager()->FindResource("Brush_Generic_Square", CIwGameXomlNames::Brush_Hash);
                    
                    w = image->getSrcRect().w;
                    h = image->getSrcRect().h;
                    
                    Init(image,w,h);
                }
            }
            else
            if(type_value_hash == CIwGameString("foreground").getHash())
            {
                setType(FORE_GROUND);
            }
            else
            if(type_value_hash == CIwGameString("parallax_bg").getHash())
            {
                setType(PARALLAX_BG);
            }
            else
            if(type_value_hash == CIwGameString("aesthetic_bg").getHash())
            {
               setType(AESTHETIC_BG);
            }
            else
            if(type_value_hash == CIwGameString("obstacles").getHash())
            {
                setType(OBSTACLES);
            }
		}
        else
        if(attrib_name_hash == CIwGameXomlNames::Layer_Hash)
        {
            unsigned int layer_hash = (*it)->GetValue().getHash();
            if(layer_hash == CIwGameString("foreground").getHash())
            {
               setLayer(L_FOREGROUND);
            }
            else
            if(layer_hash == CIwGameString("fixture").getHash())
            {
                setLayer(L_FIXTURE);
            }
            else
            if(layer_hash == CIwGameString("background").getHash())
            {
                setLayer(L_BACKGROUND);
            }
            else
            if(layer_hash == CIwGameString("collectibles").getHash() ||
                    layer_hash == CIwGameString("collectables").getHash() )
            {
                setLayer(L_COLLECTIBLES);
            }
            else
            if(layer_hash == CIwGameString("actor").getHash())
            {
                setLayer(L_ACTORS);
            }
        }
		else
        if(attrib_name_hash == CIwGameString("resizefixture").getHash())
        {
            bool resize = false;
            resize = (*it)->GetValueAsBool();
            
            if(resize)
                ResizeFixture = true;
        }
	}
	
	if(ResizeFixture && getType() == FIXTURE)
    {
        
        float width = 0;
        float height = 0;
        
        ShowVisualCollision();
        
        width = Visual->getDestWidth();
        height = Visual->getDestHeight();
        
        getVisual()->setAlphaMode(CIwMaterial::ALPHA_HALF);
        
        //ResizeActorBox2dShape(width, height);
        
    }
    
    if(MaintainSizeDepth)
    {
        float depth = getDepth();
        
        float newsize_w = getVisual()->getDestWidth() * depth;
        float newsize_h = getVisual()->getDestHeight()* depth;
        
        getVisual()->setDestSize(newsize_w, newsize_h);
    }
    
	return true;
	
}

void StaticActor::OnBeginTouch(int index, int x, int y, bool allow_bubble)
{
	CIwGameActor::OnBeginTouch(index, x, y, allow_bubble);
}

void StaticActor::ResizeActorBox2dShape(int width, int height)
{    
    //define new shape
    CIwGameGeomShapePolygon* polygon = new CIwGameGeomShapePolygon();
    CIwGameShape* shape = new CIwGameShape();
    shape->setShape(polygon);
    
    //currently, ResizeActorBox2dShape support shape uses 4 vertices only
    polygon->Vertices = new CIwFVec2[4]();
    polygon->NumVertices = 4;
    
    //translate fixture from the passed value width and height
    //each vertex point rotate clockwise
    //x is negative when going left and positive when going right
    //y is negative when going down and positive when going up
    polygon->Vertices[0].x = width / 2;
    polygon->Vertices[0].y = -1 * height / 2;
    
    polygon->Vertices[1].x = width /2;
    polygon->Vertices[1].y = height / 2;
    
    polygon->Vertices[2].x = -1 * width / 2;
    polygon->Vertices[2].y = height / 2;
    
    polygon->Vertices[3].x = -1 * width / 2;
    polygon->Vertices[3].y = -1 * height /2;
    
    Box2dBody->setFixture(shape, Box2dBody->getBodyMaterial(), 0, 0);
    
    SAFE_DELETE(shape);
}

/**
 * \brief Show the visual collision of this actor.
 *
 */
void StaticActor::ShowVisualCollision()
{
    Box2dPhysicsUti b2dutil;
    
    b2dutil.ShowVisualCollision(*this);
}

bool StaticActor::IsOffScreen()
{
    if(GAME->GetUserOptions()->IsDebugging() || Scene->getExtents().h == 0)
        return false;
    
    if(getPosition().y < Scene->getExtents().h * 0.45f)
        return false;
    
    CIwGameString string = getName();
    string += " was destroyed due to scene extents: ";
    CIwGameError::LogError(string.c_str(), CIwGameString(Scene->getExtents().h * 0.45f).c_str());

    return true;
}

bool StaticActor::GetMaintainSizeDepth() const
{
    return MaintainSizeDepth;
}
