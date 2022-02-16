//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// button_icon.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#include "actors/button_icon.h"
#include "player_actor.h"
#include "game_state.h"


//--------------------------------------------------------------------------------------------------------
//  Implementation of ButtonIcon class
//--------------------------------------------------------------------------------------------------------

ButtonIcon::ButtonIcon() : CIwGameUIBase(), HitBox(NULL), StartEvent(false)
{
    CIwColour bg_color, selected_color;
    
    selected_color.Set(150,150,150,255);
    bg_color.Set(255,255,255,255);
    
    
    CIwGameBrushImage* brush = (CIwGameBrushImage*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource("Brush_Generic_Square", CIwGameXomlNames::Brush_Hash);
    
    setActualClassType("buttonicon");
    setPercPos(false);
    setDraggable(false);
    setSelected(false);
    setColour(bg_color);
    setSelectedBackgroundColour(selected_color);
    setBackgroundBrush(brush);
}

void ButtonIcon::Init(unsigned int name_hash)
{
    if(name_hash == CIwGameString("ui_reloadXOML").getHash())
    {
        setSelectType(SelectType_Normal);
    }
    else
    if(name_hash == CIwGameString("ui_debugExport").getHash())
    {
        setSelectType(SelectType_Toggle);
    }
    else
    {
        //default
        setSelectType(SelectType_Normal);
    }
    
}

bool ButtonIcon::Update(float dt)
{
	if(!CIwGameUIBase::Update(dt))
		return false;

	return true;
}

bool ButtonIcon::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
	if(!CIwGameUIBase::LoadFromXoml(parent,load_children,node))
		return false;
    
    // initialization of specific button types
    Init(getNameHash());
    
    // get a reference to the toolbox
    CIwGameActor* toolbox = Scene->findActor("ToolBox");
    
	for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); ++it)
	{
		unsigned int name_hash = (*it)->getName().getHash();
        
		if(name_hash == CIwGameXomlNames::Type_Hash)
		{
			unsigned int type = (*it)->GetValue().getHash();
            SetType(type);
		}
	}
    
    if(getType() == UI_DEBUG_BUTTON)
    {
        CIwRect margin;
        HUDScene* scene;
        int _debug_buttons, margin_offset;
        
        scene = (HUDScene*)GAME->findScene("HUD");
        if(scene != NULL)
        {
            scene->AddNumOfDebugButton();
            
            _debug_buttons = scene->GetNumOfDebugButtons();
            margin_offset = (_debug_buttons * 10) - 5 ;
            
            if(_debug_buttons < 2)
                margin = CIwRect(0, -5, -5, 0);
            else
                margin = CIwRect(0, -5, -margin_offset, 0);
            
            CalculateMargins(margin);
        
            setDocking(DK_TopRight);
        }
    }
    
    HitBox = InitHitBox(this, 0, EXT_BUTTON_ICON_HEIGHT);
    
	return true;
}

void ButtonIcon::SetType(unsigned int type)
{
    if(type == CIwGameString("ui_restart").getHash())
    {
        Type = UI_RESTART;
    }
    else
    if(type == CIwGameString("ui_next").getHash())
    {
        Type = UI_NEXT;
    }
    else
    if(type == CIwGameString("ui_test").getHash())
        Type = UI_TEST;
    else
    if(type == CIwGameString("ui_left").getHash())
    {
        Type = UI_LEFT;
        //InitHitBox(toolbox, false, true);
        setActualClassType("interfacebuttonicon");
        //return true;
    }
    else
    if(type == CIwGameString("ui_right").getHash())
    {
        Type = UI_RIGHT;
        //InitHitBox(toolbox, false, true);
        setActualClassType("interfacebuttonicon");
        //return true;
    }
    else
    if(type == CIwGameString("ui_pause").getHash())
        Type = UI_PAUSE;
    else
    if(type == CIwGameString("ui_jump").getHash())
    {
        Type = UI_JUMP;
        //InitHitBox(toolbox, false, true);
        setActualClassType("interfacebuttonicon");
        //return true;
    }
    else
    if(type == CIwGameString("ui_resume").getHash())
    {
        Type = UI_RESUME;
    }
    else
    if(type == CIwGameString("ui_action").getHash())
        Type = UI_ACTION;
    else
    if(type == CIwGameString("ui_return").getHash())
    {
        Type = UI_RETURN;
    }
    else
    if(type == CIwGameString("ui_storyboard").getHash())
    {
        Type = UI_STORYBOARD;
    }
    else
    if(type == CIwGameString("ui_debug_button").getHash())
    {
        Type = UI_DEBUG_BUTTON;
        
    }
}

void ButtonIcon::OnBeginTouch(int index, int x, int y, bool allow_bubble)
{
    CIwGameUIBase::OnBeginTouch(index, x, y, allow_bubble);
    
    //cancel pause if type is a movement interface
    if(getActualClassTypeHash() == CIwGameString("interfacebuttonicon").getHash())
        GAME->CancelPause();
    
    if(getName() == "UI_Jump")
        ((PlayerActor*)getTarget())->Jump(0, true);
}

void ButtonIcon::OnEndTouch(int index, int x, int y, bool allow_bubble)
{
	CIwGameUIBase::OnEndTouch(index, x, y, allow_bubble);
    
	if(getName() == "UI_Jump")
    {
        ActorState* state = static_cast<PlayerActor*>(getTarget())->GetActorStateManager()->GetCurrentState();
        
        if(state->GetID() != ACTOR_FALLING)
        {
            //slow down jump when player release button
            b2Vec2 vel = getTarget()->getBox2dBody()->getBody()->GetLinearVelocity();
            vel.y *= 0.4f;
            getTarget()->setVelocity(vel.x, vel.y);
        }
    }
    StartEvent = true;
    
    if(getName() == "ActionBar")
    {
        PlayerActor* actor = (PlayerActor*)GAME->findScene("MainScene")->findActor("Player");
        
        // TODO: #H8493 find a better spot to check if actor has sword
        if(actor->HasItem(SWORD))
        {
            actor->GetActorStateManager()->OnNotify(actor, EVENT_START_ATTACK);
            return;
        }
        
        if(!isSelected())
        {
            actor->SetActionBar(false);
        }
        else
        {
            actor->SetActionBar(true);
        }
    
    }
}

void ButtonIcon::OnTapped(int index, int x, int y, bool allow_bubble)
{
    CIwGameUIBase::OnTapped(index, x, y, allow_bubble);
	index = getType();
    
	switch (index)
	{
        case UI_PAUSE:
		{
			GAME->LoadPauseMenu();
		}
		break;
        case UI_RESUME:
		{
			GAME->CancelPause();
		}
		break;
        case UI_RESTART:
		{
            CIwGameScene* mainscene = GAME->findScene("MainScene");
			CIwGameScene* levelcomplete_scene = GAME->findScene("LevelComplete");
            
			GAME->CancelPause();
			GAME->ResetLevel();
            
			if(GAME->getCurrentScene() != mainscene)
			{
				GAME->changeScene(mainscene);
				mainscene->setActive(true);
			}
			if(levelcomplete_scene != NULL)
				GAME->removeScene(levelcomplete_scene);
		}
		break;
        case UI_NEXT:
		{
            // destroy Old Level
			GAME->DestroyOldLevel();
			GAME->LoadLevel(GAME->GetWorld(), GAME->GetNextLevel());
		}
		break;
        case UI_DEBUG_BUTTON:
        {
            //reload button
            if(getName() == "ui_reloadXOML")
            {
                GameOptions* option = GAME->GetUserOptions();

                IW_GAME_XOML->Process(option, "gameoptions.xml");

                bool customPos = option->HasCustomPos();

                if(customPos)
                {
                    CIwGameActor* actor = GAME->findScene("MainScene")->findActor("Player");
                    CIwFVec2 pos = actor->getPosition();
                    
                    option->SetPlayerPos(pos);
                    //need to implement function to write pos to .xml file before processing "gameoptions.xml"
                }

                int world = GAME->GetWorld();
                int level = GAME->GetLevel();

                GAME->DestroyOldLevel();
                GAME->LoadLevel(world, level);
            }
            
            //export button
            if(getName() == "ui_debugExport")
            {
                GAME->SaveSceneActorsPositions();
            }
           
        }
        break;
        case UI_TEST:
		{
            // TODO: #B6123 fix to load test level
			//GAME->DestroyOldLevel();
			//GAME->LoadLevel(999, 999);
		}
		break;
        case UI_JUMP:
        {
            
           // PlayerActor* player = (PlayerActor*)GAME->findScene("MainScene")->findActor("Player");
           // player->Jump(JUMPING_HEIGHT, true);
             
        }
        break;
        case UI_RETURN:
        {
            // save
            IW_GAME_AUDIO->StopAllSounds();
            IW_GAME_AUDIO->StopMusic();
            GAME->SaveGameData();
            
            GAME->CancelPause();
            IW_GAME_XOML->Process(GAME, "WorldMap.xml");
            CIwGameScene* map_scene = GAME->findScene("WorldMap");
            GAME->removeAllScenes(map_scene);
            
            //if(GAME->GetUserOptions()->IsDebugging())
            //    GAME->LoadDebugHUD();
            //else
                //GAME->SetHUDText(NULL);
        }
        break;
        case UI_STORYBOARD:
        {
            GAME->removeAllScenes(9999);
            if(GAME->GetUserOptions()->IsDebugging())
                GAME->LoadDebugHUD();
            else
                GAME->SetHUDText(NULL);
            
            GAME->ChangeState(STATE_STORY);
        
        }
        break;
            
	}
}

void ButtonIcon::LoadActionBar(int type)
{
	CIwGameBrushImage* brush;
	CIwGameString brush_name;
	setActive(true);
	setType(type);
	setSelected(true);
    PlayerActor* player = (PlayerActor*)GAME->findScene("MainScene")->findActor("Player");

	switch(type)
	{
	case SWORD:
		{
			brush_name = "Brush_Sword";
		}
		break;
	case BUBBLE:
		{
			brush_name = "Brush_Bubble";
		}
		break;
	case SPRING:
		{
			brush_name = "Brush_SpringBoots";
		}
		break;
	case UMBRELLA:
		{
			brush_name = "Brush_Umbrella";
		}
		break;
	default:
		brush_name = "Brush_Generic_Square";
	}

    /*
    if(player != NULL)
        player->SetAbility(type);
    */
    
	brush = (CIwGameBrushImage*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource(brush_name.str(), CIwGameXomlNames::Brush_Hash);
	SetFromBrush(brush);
	setBackgroundBrush(brush);
    player->SetActionBar();
}

CIwGameActor* ButtonIcon::HitTest(float x, float y)
{
    if(HitBox == NULL || !Visual->isVisibleWithParents())
        return NULL;
    
    if(HitBox->HitTest(x,y))
        return this;
    
    return NULL;
}


