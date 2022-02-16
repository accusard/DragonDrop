//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// toolbar.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#include "actors/toolbar.h"
#include "player_actor.h"
#include "sensor_actor.h"
#include "game_state.h"

//--------------------------------------------------------------------------------------------------------
//  Implementation of BarIcon class
//--------------------------------------------------------------------------------------------------------
bool BarIcon::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
	if(!CIwGameUILabel::LoadFromXoml(parent, load_children, node))
		return false;
    
	for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int attrib_name_hash = (*it)->getName().getHash();
		if(attrib_name_hash == CIwGameString("quantity").getHash())
		{
			Quantity = (*it)->GetValueAsInt();
			OrigQuantity = Quantity;

			CIwGameString bar_actor;
            
			// find an unoccupied bar space and load the icon
			for(int i = 1; i < 7; i++)
			{
				bar_actor = "Bar";
				int num = i;
				bar_actor += CIwGameString(num).c_str();
				ToolBarBox* bar = (ToolBarBox*)Scene->findActor(CIwGameString(bar_actor).c_str());

				if(bar != NULL && bar->isOccupied() != true)
				{
                    //bar->setLinkedTo(this);
                    //setLinkedTo(bar);
					Position = bar->getPosition();
					setOriginalPosition(Position.x, Position.y);
					bar->setOccupied(true);
					setText(CIwGameString(Quantity).c_str());
					getTextActor()->setPosition(30,30);
					getTextActor()->setLayer(9);
                    
					break;
				}
			}
		}
        else //set the type of the object
        if(attrib_name_hash == CIwGameXomlNames::Type_Hash)
        {
            unsigned int type = (*it)->GetValue().getHash();
            
            if(type == CIwGameString("trampoline").getHash())
            {
                setType(DROP_TRAMPOLINE);
            }
            else
            if(type == CIwGameString("platform").getHash())
            {
                setType(DRAG_PLATFORM);
            }
            else
            if(type == CIwGameString("bomb").getHash())
            {
                setType(DROP_BOMB);
            }
            else
            if(type == CIwGameString("stone").getHash())
            {
                setType(DROP_STONE);
            }
            else
            if(type == CIwGameString("candle").getHash())
            {
                setType(DROP_CANDLE);
            }
        }
	}
    
    HitBox = InitHitBox(this, 0, EXT_TOOLBAR_ICON_HEIGHT);

	return true;
}

void BarIcon::ResetQuantity()
{
    CIwColour color = TextActor->getOriginalColour();
	Quantity = OrigQuantity;
	setTextColour(color);
	setText(CIwGameString(Quantity).c_str());

}

bool BarIcon::Update(float dt)
{
	if(isDestroyed())
		return false;
    
	if(!isSelected() && Position != OriginalPosition)
	{
		Position = OriginalPosition;
        setText(CIwGameString(Quantity).c_str());
		if(Quantity <= 0)
		{
			Quantity = 0;

			//disable icon
			setEnabled(false);
			TextActor->setColour(255,0,0,255);
		}
	}
	else
	if(Quantity > 0 && !isEnabled())
	{
		setEnabled(true);
	}
    
	return CIwGameUILabel::Update(dt);
}

/**
 * \brief Activates the sensor of the dragging bar icon.
 *
 */
void BarIcon::ActivateColorSensor()
{
    int                 type        = getType();
    GameScene*          mainscene   = (GameScene*)GAME->findScene("MainScene");
    SensorColorActor*   drop_sensor = mainscene->GetSensorActor();
    
    //Activate SensorActor
	if(drop_sensor != NULL)
	{
		drop_sensor->setActive(true);
		drop_sensor->setLinkedTo(this);
        drop_sensor->RemoveBody();
        
        Box2dPhysicsUti box2d;
        box2d.AssignBox2dPhysics(*drop_sensor, type);
        
        //drop_sensor->AssignBox2dPhysics(type);
	}
}

void BarIcon::OnBeginTouch(int index, int x, int y, bool bubble)
{
    IW_GAME_AUDIO->PlaySound("toolbar_grab");
    CIwGameUILabel::OnBeginTouch(index, x, y, bubble);
    
    if(!isEnabled())
        return;
    
    GAME->ChangeState(STATE_PAUSE);
    
    ActivateColorSensor();
    
    if(isSelected())
    {
        DragAnchor.y -= 125;
        
        int type = getType();
        int w = 0;
        int h = 0;
        CIwGameBrushImage* image;
        
        // some objects need to be rescaled when dropping in the main game scene
        switch(type)
        {
            case DROP_CANDLE:
            {
                image = (CIwGameBrushImage*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource("Brush_Candle_world", CIwGameXomlNames::Brush_Hash);
                w = image->getSrcRect().w;
                h = image->getSrcRect().h;
                
                getVisual()->setDestSize(w, h);
                CIwGameError::LogError("Width..", CIwGameString(w).c_str());
                CIwGameError::LogError("Height..", CIwGameString(h).c_str());
                break;
            }
            case DROP_BOMB:
            {
                image = (CIwGameBrushImage*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource("Brush_Bomb_world", CIwGameXomlNames::Brush_Hash);
                w = image->getSrcRect().w;
                h = image->getSrcRect().h;
                
                getVisual()->setDestSize(w, h);
                break;
            }
        }
    }
    
}

void BarIcon::OnEndTouch(int index, int x, int y, bool bubble)
{
    GAME->ChangeState(STATE_PLAYING);
    
    IW_GAME_AUDIO->PlaySound("toolbar_place");
    
    GameScene* mainscene = (GameScene*)GAME->getCurrentScene();
	
    if(!mainscene->GetSensorActor()->HasContacted())
    {
        mainscene->SetSpawnSafe(true);
        mainscene->SetDropActor(this);
        mainscene->SetSpawnPosition(getPosition() + mainscene->getCamera()->getPosition());
        
        //update object's available qty
        Quantity -= 1;
    }
    else
    {
        ((HUDScene*)Scene)->GetHUDText()->SetTextDisplay("Cannot place object there!");
    }
    
    // deactivate SensorActor
	if(mainscene->GetSensorActor() != NULL)
    {
        mainscene->GetSensorActor()->SetContacted(false);
        mainscene->GetSensorActor()->getLinkedTo()->setTimeline(NULL);
		mainscene->GetSensorActor()->setActive(false);
    }
    
   	GAME->CancelPause();
    
    CIwGameUILabel::OnEndTouch(index, x, y, bubble);

}

CIwGameActor* BarIcon::HitTest(float x, float y)
{
    if(HitBox == NULL || !HitBox->HitTest(x,y))
        return CIwGameUIBase::HitTest(x, y);
    
    return this;
}

