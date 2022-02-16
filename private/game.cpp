//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// game.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#include "s3e.h"
#include "Iw2D.h"
#include "IwGx.h"
#include "IwResManager.h"

#include "math.h"

#include "game.h"
#include "player_actor.h"
#include "static_actor.h"
#include "game_object_actor.h"
#include "base_actor.h"
#include "enemy_actor.h"
#include "scene.h"
#include "toolbar.h"
#include "obstacles.h"
#include "storyboard.h"
#include "level_select.h"
#include "bomb_actor.h"
#include "stone_actor.h"
#include "trampoline_actor.h"
#include "powerup_actor.h"
#include "ball_actor.h"
#include "placementdata.h"
#include "boss_dragon.h"
#include "debugdraw.h"
#include "game_state.h"
#include "gamecamera.h"

CDECLARE_SINGLETON(Game)

typedef CIwList<CIwGameActor*>::iterator    _It;

void Game::Init(bool enable_http, bool enable_extensions, bool gl_conmpatible)
{
	//Initialize the game
	CIwGame::Init(enable_http);

	srand(time(NULL));

	//custom classes for IwGame XOML
	IW_GAME_XOML->addClass(new SceneCreator());
	IW_GAME_XOML->addClass(new PlayerActorCreator());
	IW_GAME_XOML->addClass(new StaticActorCreator());
    IW_GAME_XOML->addClass(new GameObjectActorCreator());
	IW_GAME_XOML->addClass(new CollectableActorCreator());
	IW_GAME_XOML->addClass(new EnemyActorCreator());
	IW_GAME_XOML->addClass(new HUDCreator());
	IW_GAME_XOML->addClass(new BarActorCreator());
	IW_GAME_XOML->addClass(new BarIconCreator());
	IW_GAME_XOML->addClass(new ButtonIconCreator());
    IW_GAME_XOML->addClass(new PowerUpActorCreator());
    IW_GAME_XOML->addClass(new MapActorCreator());
    IW_GAME_XOML->addClass(new OrbActorCreator());
    IW_GAME_XOML->addClass(new SensorActorCreator());
    IW_GAME_XOML->addClass(new SensorActorPropertyCreator());
    IW_GAME_XOML->addClass(new LevelSelectCreator());
    IW_GAME_XOML->addClass(new WindCreator());
    IW_GAME_XOML->addClass(new StoryboardCreator());
    IW_GAME_XOML->addClass(new GameOptionsCreator());

    IW_GAME_XOML->addClass(new SensorWeaknessActorCreator());
    IW_GAME_XOML->addClass(new AppendageCreator());
    IW_GAME_XOML->addClass(new BombActorCreator());
    IW_GAME_XOML->addClass(new StoneActorCreator());
    IW_GAME_XOML->addClass(new TrampolineActorCreator());
    IW_GAME_XOML->addClass(new BossDragonCreator());
    IW_GAME_XOML->addClass(new SpikedBallActorCreator());

	//custom actions for the IwGame XOML system
	for(int t = 0; t < CIwGameXomlAction_Global::Action_Max; t++)
		IW_GAME_XOML->addAction(new GameXomlAction((GameXomlAction::ActionType)t));
    
    //Load user options
    UserOptions = new GameOptions();
    UserOptions->Init();
    
    //CIwGameXmlParser* xml = new CIwGameXmlParser();
    //xml->Parse("gameoptions.xml");
    
    //IIwGameXomlResource* instance = UserOptions;
    IW_GAME_XOML->Process(UserOptions, "gameoptions.xml");
    //delete xml;
    //delete instance;
    
	//init game data
    HUDText         = NULL;
    World           = UserOptions->StartAt().x;
    Level           = UserOptions->StartAt().y;
    CurrentMusic    = NULL;

    m_currentState  = NULL;
    m_stateChanged  = false;
    m_lastState     = NULL;
    m_storyboard    = "";
    m_collectedMaps = 0;
    m_collectedOrbs = 0;
    
	// load global game resources
	IW_GAME_XOML->Process(this, "Common.xml");
    IW_GAME_XOML->Process(this, "object_sprites.xml");
    
    //load animation data
    IW_GAME_XOML->Process(this, "animations.xml");
    
    //if(!UserOptions->IsDebugging())
    //{
        AddState(new Story(), false);
        GAME->SetStoryboard(INTRO_STR);
    //}

    AddState(new Title(), true);
    AddState(new Playing(), false);
    AddState(new Pause(), false);
    
    
    SetCurrentLevelString(World, Level);
    
    LoadGameData();
    
    //DebugDraw debugdraw;
    
    
}

void Game::Release()
{

    SAFE_DELETE(UserOptions);
    
    SAFE_DELETE(HUDText);
        
    if (m_currentState != NULL)
    	m_currentState->Close();
    
    for(_StateIterator it = m_states.begin(); it != m_states.end(); ++it)
        delete *it;
    
    m_states.clear();
    
	CIwGame::Release();
}

bool Game::Update()
{
    m_stateChanged = false;
    
    if(!CIwGame::Update())
        return false;
    
    if (m_currentState != NULL)
    	m_currentState->Update();
    
    // TODO: F3891 condense this bock *****************
    // TODO: G8930 functions NotifySuspending() and NotifyResuming() have pointer to 'new_scene' and 'old_scene'
    //          utilize this instead of calling function LoadPauseMenu() and CancelPauseMenu()
    if(m_stateChanged == true)
    {
        if(m_currentState->GetID() == STATE_PAUSE)
        {
            for(_Iterator it = CIwGame::begin(); it != CIwGame::end(); ++it)
            {
                (*it)->NotifySuspending(NULL);
            }
        }
        if(m_currentState->GetID() == STATE_PLAYING)
        {
            for(_Iterator it = CIwGame::begin(); it != CIwGame::end(); ++it)
            {
                (*it)->NotifyResuming(NULL);
            }
        }
    }
    // condense this bock ******************************
    
    // move into the actual state?
    if(m_currentState->GetID() != STATE_STORY)
        UpdateDebuggingInfo();
    
    return true;
}
void Game::AddState(State* state, bool change)
{
	m_states.push_back(state);
	
	if(change == false)
		return;
		
	if(m_currentState != NULL)
		m_currentState->Close();
		
	m_currentState = m_states.back();
	m_currentState->Load();
}
void Game::ChangeState(unsigned long id)
{
    int num = id;
    
    for(_StateIterator it = m_states.begin(); it != m_states.end(); ++it)
    {
        if( (*it)->GetID() == id )
        {
            m_lastState = id;
            
            if(m_currentState != NULL)
				m_currentState->Close();
            
            m_currentState = *it;
            m_currentState->Load();
            
            m_stateChanged = true;
            
            break;
        }
            
    }
}
void Game::RemoveState(State* state)
{
    for(_StateIterator it = m_states.begin(); it != m_states.end(); ++it)
    {
        if((*it) == state)
        {
            m_states.erase(it);
            delete *it;
            return;
        }
    }
	//m_states.erase(&state);
}

State* Game::GetCurrentState()
{
	return m_currentState;
}

void Game::UpdateDebuggingInfo()
{
    //show debugging info
    if(GetUserOptions()->IsDebugging() && getTouchFocus(0) != NULL && GAME->findScene("HUD") != NULL)
    {
        if(getTouchFocus(0)->isDragging())
        {
            CIwGameActor*   tf_actor        = getTouchFocus(0);
            int             tf_actor_type   = tf_actor->getType();
            CIwFVec2        pos             = getTouchFocus(0)->getPosition();
            CIwGameString   string;
            
            string += getTouchFocus(0)->getName().c_str();
            string += "'s position: ";
            string += CIwGameString((int)pos.x).c_str();
            string += ", ";
            string += CIwGameString((int)pos.y).c_str();
            
            if(tf_actor_type == MAP || tf_actor_type == ORB)
            {
                string += " ID = ";
                string += ((CollectableActor*)tf_actor)->GetID();
            }
            
            GetHUDText()->SetTextDisplay(string.c_str());
        }
    }
}

bool Game::CheckTouch()
{
    return IW_GAME_INPUT->hasTapped();
}

void Game::ClearTimelineFromParent(CIwGameAnimTimeline* timeline)
{
    //check parent of type CIwGameActor
    CIwGameActor* actor = (CIwGameActor*)timeline->getParent();
    
    if(actor->getTimeline() != timeline)
        return;
    
    if(!timeline->isPlaying())
    {
        actor->setTimeline(NULL);
    }
}

void Game::Draw()
{
	CIwGame::Draw();
}

/**
 * \brief   Function retrieves object data for the specified type
 */
ObjectData Game::GetBox2dObjectData(int type) const
{
    ObjectData data;
    
    data.type = type;
    
    switch(data.type)
	{
        case DROP_TRAMPOLINE:
		{
			data.shape      = "Trampoline_Shape";
			data.material   = "Fast_Bouncy";
			data.image      = "Brush_Trampoline";
			data.heavy      = true;
            data.name       = "trampoline";
		}
            break;
        case DROP_STONE:
		{
			data.shape      = "Stone_Shape";
			data.material   = "Heavy";
			data.image      = "Brush_Stone";
			data.heavy      = true;
            data.name       = "stone";
		}
            break;
        case DROP_PLATFORM:
		{
			data.shape      = "Platform_Shape";
			data.material   = "Solid_Static";
			data.image      = "Brush_Platform";
			data.heavy      = false;
            data.name       = "platform";
		}
            break;
        case DRAG_PLATFORM:
        {
            data.shape      = "Platform_Shape";
            data.material   = "Solid_Dynamic";
            data.image      = "Brush_Platform";
            data.heavy      = false;
            data.name       = "platform";
        }
            break;
        case DROP_BOMB:
		{
			data.shape		= "Bomb_Shape";
			data.material	= "Fast_Bouncy";
			data.image      = "Brush_Bomb_world";
			data.heavy      = false;
			data.angle_vel  = 3;
            data.name       = "bomb";
		}
            break;
        case DROP_CANDLE:
		{
			data.shape      = "Candle_Shape";
			data.material   = "Heavy";
			data.image      = "Brush_Candle_world";
			data.heavy      = true;
            data.name       = "candle";
		}
            break;
	}
    
    return data;
}

/**
 * \brief Load global game data from file.
 */
void Game::LoadGameData()
{
    // load global game data
    CIwGameFile file;
    
    if(file.Open("\\gamedata.bin", "rb"))
    {

            GameData data;
            
            file.Read(&data, sizeof(GameData));

        
            m_collectedOrbs = data.collected_orbs;
            m_collectedMaps = data.collected_maps;

    }
}

/**
 * \brief Save local scene and global game data to file.
 */
void Game::SaveGameData()
{
    // save local scene data
    GameScene* mainscene = (GameScene*)GAME->findScene("MainScene");
    
    mainscene->SaveSceneData();
    mainscene->AddUpCollectables();
    
    // save global game data
    CIwGameFile file;
    GameData    data;
    
    data.collected_maps = m_collectedMaps;
    data.collected_orbs = m_collectedOrbs;
    
    file.Open("\\gamedata.bin", "wb");
    file.Write(&data, sizeof(GameData));
}

/**
 * \brief Save various actors' position to a file called 'actorpos.dds' in the data-ram folder.
 */
void Game::SaveSceneActorsPositions()
{
    
    CIwGameString data;
    CIwGameFile file;

    GetLevelUISelectPos(data);
    
    GetLevelEnemyPos(data);
    GetLevelFixturePos(data);
    GetLevelForegroundPos(data);
    GetLevelAestheticsBGPos(data);
    GetLevelParallaxPos(data);
    GetLevelMapFragmentsPos(data);
    GetLevelOrbPos(data);
    GetLevelObjectiveGoal(data);
    GetLevelObstacles(data);
    
    //export all data to a file called 'actorpos.dds' in the data-ram folder
    if(file.Open("\\actorpos.dds","wb"))
    {
        file.Write((void*)data.c_str(), data.GetLength());
    }
}

/**
 * \brief Export the LevelSelectActor's position from the current scene to data.
 * \param data Data to load into.
 */
void Game::GetLevelUISelectPos(CIwGameString &data)
{
    XomlActorPlacementData pd("LevelSelect", "levelselect", UI_LEVEL_SELECT);
    
    for(_It it = pd.GetScene()->begin(); it != pd.GetScene()->end(); ++it)
    {
        if((*it)->getType() == pd.GetActorDataType() )
        {
            CIwGameString   level       = (*it)->getName();
            CIwFVec2        pos         = (*it)->getPosition();
            CIwGameString   playable    = ((LevelSelect*)(*it))->LevelPlayable();
            
            level.Replace("Level", "");

            pd.FormatCustomDataNewLine("level", level);
            pd.FormatCustomData("pos", pos);
            pd.FormatCustomData("playable",playable,true);
        }
    }
    
    pd.FormatEnd(data);
}

/**
 * \brief Export any Fixture Actors position from the current scene to data.
 * \param data Data to load into.
 */
void Game::GetLevelFixturePos(CIwGameString &data)
{
    XomlActorPlacementData pd("Fixture","fixture",FIXTURE);
    
    for(_It it = pd.GetScene()->begin(); it != pd.GetScene()->end(); ++it)
    {
        if((*it)->getType() == pd.GetActorDataType() )
        {
            CIwGameString   fnum    = (*it)->getName();
            CIwFVec2        pos     = (*it)->getPosition();
            
            fnum.Replace("fixture", "");
            
            pd.FormatCustomDataNewLine("num", fnum);
            pd.FormatCustomData("pos", pos, true);
        }
    }

    pd.FormatEnd(data);
}

/**
 * \brief Export any Foreground Actors' positions from the current scene into data.
 * \param data Data to load into.
 */
void Game::GetLevelForegroundPos(CIwGameString &data)
{
    XomlActorPlacementData pd("Foreground","foreground",FORE_GROUND);
    
    for(_It it = pd.GetScene()->begin(); it != pd.GetScene()->end(); ++it)
    {
        if((*it)->getType() == pd.GetActorDataType() )
        {
            CIwGameString   fgnum   = (*it)->getName();
            CIwFVec2        pos     = (*it)->getPosition();
            
            fgnum.Replace("foreground", "");
            
            pd.FormatCustomDataNewLine("num", fgnum);
            pd.FormatCustomData("pos", pos, true);
        }
    }

    pd.FormatEnd(data);
}

/**
 * \brief Export any parallax background's positions from the current scene into data.
 * \param data Data to load into.
 */
void Game::GetLevelParallaxPos(CIwGameString &data)
{
    XomlActorPlacementData pd("Parallax Background", "parallax", PARALLAX_BG);
    
    for(_It it = pd.GetScene()->begin(); it != pd.GetScene()->end(); ++it)
    {
        if((*it)->getType() == pd.GetActorDataType() )
        {
            CIwGameString   name    = (*it)->getName();
            CIwFVec2        pos     = (*it)->getPosition();
            float           depth   = (*it)->getDepth();
            CIwGameString   m_size  = ((StaticActor*)(*it))->GetMaintainSizeDepth();
            
            pd.FormatPlacementData(name, pos, false);
            pd.FormatCustomData("depth", depth);
            pd.FormatCustomData("maintainsize",m_size,true);
        }
    }
    
    pd.FormatEnd(data);
    
}

/**
 * \brief Export any actors of type enemy's positions from the current scene into data.
 * \param data Data to load into.
 */
void Game::GetLevelEnemyPos(CIwGameString &data)
{
    XomlActorPlacementData pd("Enemy Positions", "enemy", ENEMY);
    
    for(_It it = pd.GetScene()->begin(); it != pd.GetScene()->end(); ++it)
    {
        if((*it)->getType() == pd.GetActorDataType() )
        {
            CIwGameString   name;
            CIwFVec2        pos;
            CIwFVec2        vel;
            CIwGameString   brush;
            float           w;
            float           h;
            CIwGameString   mat;
            
            name    = (*it)->getName();
            pos     = (*it)->getPosition();
            vel     = (*it)->getVelocity();
            brush   = (*it)->getVisual()->getBrush()->getName();
            w       = ((CIwGameGeomShapeBox*)(*it)->getBox2dBody()->getBodyShape()->getShape())->Width;
            h       = ((CIwGameGeomShapeBox*)(*it)->getBox2dBody()->getBodyShape()->getShape())->Height;
            mat     = (*it)->getBox2dBody()->getBodyMaterial()->getName();

            pd.FormatPlacementData(name, pos, false);
            pd.FormatCustomData("vx", vel.x);
            pd.FormatCustomData("vy", vel.y);
            pd.FormatCustomData("image", brush);
            pd.FormatCustomData("w",w);
            pd.FormatCustomData("h",h);
            pd.FormatCustomData("material", mat, true);
        }
    }
    
    pd.FormatEnd(data);
}

/**
 * \brief Export the position of the objective goal found in the current scene into data.
 * \param data Data to load into.
 *  TODO: change StaticActor declaration to FromTemplate and shorten data strings; utilize XomlActorPlacementData
 */
void Game::GetLevelObjectiveGoal(CIwGameString &data)
{
    CIwGameScene* scene;
    typedef CIwList<CIwGameActor*>::iterator _It;
    
    scene = GAME->getCurrentScene();
    
    data += "<!-- [-Objective Positions-] -->\r";
    
    for(_It it = scene->begin(); it != scene->end(); ++it)
    {
        if((*it)->getType() == GOAL_FLAG )
        {
            CIwFVec2    pos = (*it)->getPosition();
            
            data += "<StaticActor Name=\"Goal\" Collidable=\"true\" CollisionFlags=\"1,1,0\" Sensor=\"true\" Type=\"GOALFLAG\" Box2dMaterial=\"GoalFlag\" Shape=\"GoalFlagShape\" Image=\"PlaceHolderImage\" AlphaMode=\"half\" Layer=\"9\" Timeline=\"floater\" Size=\"10,50\" Position=\"";
            data += pos.x;
            data += ",";
            data += pos.y;
            data += "\" />\r";
        }
    }
    
    data += "\r\r";
}

/**
 * \brief Export the positions of the aesthetic backgrounds found in the current scene into data.
 * \param data Data to load into.
 *  TODO: change StaticActor declaration to FromTemplate and shorten data strings; utilize XomlActorPlacementData
 */
void Game::GetLevelAestheticsBGPos(CIwGameString &data)
{
    CIwGameScene* scene;
    typedef CIwList<CIwGameActor*>::iterator _It;
    
    scene = GAME->getCurrentScene();
    
    data += "<!-- [-Aesthetic BG Positions-] -->\r";
    
    for(_It it = scene->begin(); it != scene->end(); ++it)
    {
        if((*it)->getType() == AESTHETIC_BG )
        {
            CIwGameActor*   actor;
            CIwGameString   name;
            CIwFVec2        vel;
            CIwGameString   image;
            CIwFVec2        pos;
            CIwRect         src_rect;
            CIwGameString   layer;
            CIwGameString   depth;
            CIwGameString   m_size = ((StaticActor*)(*it))->GetMaintainSizeDepth();;
            
            actor       = (*it);
            name        = actor->getName();
            vel         = actor->getVelocity();
            image       = ((CIwGameBitmapSprite*)actor->getVisual())->getImage()->getName();
            pos         = actor->getPosition();
            src_rect    = ((CIwGameBitmapSprite*)actor->getVisual())->getSrcRect();
            layer       = actor->getLayer();
            depth       = actor->getDepth();
            
            data += "<StaticActor Name=\"";
            data += name;
            data += "\" Type=\"aesthetic_bg\" Collidable=\"false\" Velocity=\"";
            data += vel.x;
            data += ", ";
            data += vel.y;
            data += "\" WrapPosition=\"true\" Image=\"";
            data += image;
            data += "\" Position=\"";
            data += pos.x;
            data += ", ";
            data += pos.y;
            data += "\" Size=\"0,0\" Angle=\"0\" SrcRect=\"";
            data += src_rect.x;
            data += ", ";
            data += src_rect.y;
            data += ", ";
            data += src_rect.w;
            data += ", ";
            data += src_rect.h;
            data += "\" Color=\"255,255,255,255\" Layer=\"";
            data += layer;
            data += "\" Depth=\"";
            data += depth;
            data += "\" MaintainSizeDepth=\"";
            data += m_size;
            data += "\" />\r";
        }
    }
    
    data += "\r\r";
}

/**
 * \brief Export the positions of any map fragments found in the current scene into data.
 * \param data Data to load into.
 */
void Game::GetLevelMapFragmentsPos(CIwGameString &data)
{
    XomlActorPlacementData pd("Map Fragment Positions", "map", MAP);
    
    for(_It it = pd.GetScene()->begin(); it != pd.GetScene()->end(); ++it)
    {
        if((*it)->getType() == pd.GetActorDataType() )
        {
            CIwGameString   name            = (*it)->getName();
            CIwFVec2        pos             = (*it)->getPosition();
            int             id_value        = ((CollectableActor*)(*it))->GetID();
            bool            end_line_early  = false;
            
            if((*it)->getLayer() != L_HIDDEN)
            {
                end_line_early = true;
            }
            
            pd.FormatPlacementData(name, pos, false);
            pd.FormatCustomData("id", id_value, end_line_early);
            
            if(end_line_early == false)
            {
                pd.FormatCustomData("layer", "hidden", true);
            }
        }
    }
    
    pd.FormatEnd(data);
}

/**
 * \brief Export the positions of any orb objects found in the current scene into data.
 * \param data Data to load into.
 */
void Game::GetLevelOrbPos(CIwGameString &data)
{
    XomlActorPlacementData pd("Orb Positions", "orb", ORB);
    
    for(_It it = pd.GetScene()->begin(); it != pd.GetScene()->end(); ++it)
    {
        if((*it)->getType() == pd.GetActorDataType() )
        {
            CIwGameString   name        = (*it)->getName();
            CIwFVec2        pos         = (*it)->getPosition();
            int             id_value    = ((CollectableActor*)(*it))->GetID();

            
            pd.FormatPlacementData(name, pos, false);
            pd.FormatCustomData("id", id_value, true);
            
        }
    }
    
    pd.FormatEnd(data);
}

/**
 * \brief Export the positions of any obstacles found in the current scene into data.
 * \param data Data to load into.
 */
void Game::GetLevelObstacles(CIwGameString &data)
{
    uint type = CIwGameString("gameobjectactor").getHash();
    XomlActorPlacementData pd("Obstacle Positions", "obstacle", type);

    for(_It it = pd.GetScene()->begin(); it != pd.GetScene()->end(); ++it)
    {
        if((*it)->getActualClassTypeHash() == pd.GetActorDataType())
        {
            CIwGameString   name        = (*it)->getName();
            CIwFVec2        pos         = (*it)->getPosition();
            
            
            pd.FormatPlacementData(name, pos, true);
        }
    }
    
    pd.FormatEnd(data);
}

/**
 * \brief Add to data member m_collectedMaps by the specified int.
 * \param num The number to add.
 */
void Game::AddNumOfMapFragments(int num)
{
    m_collectedMaps += num;
}

/**
 * \brief Add to data member m_collectedOrbs by the specified int.
 * \param num The number to add.
 */
void Game::AddNumOfOrbs(int num)
{
    m_collectedOrbs += num;
}

/**
 * \brief Get the total number of collected maps.
 */
int Game::GetNumOfCollectedMaps() const
{
    return m_collectedMaps;
}

/**
 * \brief Get the total number of collected orbs.
 */
int Game::GetNumOfCollectedOrbs() const
{
    return m_collectedOrbs;
}

void Game::LoadMap()
{
    IW_GAME_XOML->Process(GAME, "WorldMap.xml");
    CIwGameScene* map_scene = GAME->findScene("WorldMap");
    GAME->removeAllScenes(map_scene);
    if(GAME->GetUserOptions()->IsDebugging())
        GAME->LoadDebugHUD();
    else
        GAME->SetHUDText(NULL);
}

void Game::DisplayStartLevel(CIwGameString string, bool reformat_str)
{
	//LoadPlayer();

    HUDScene* scene = (HUDScene*)GAME->findScene("HUD");
    
    if(reformat_str)
    {
        string.Replace("Level", "Level ");
        string.Replace("_", " - ");
        string.Replace(".xml", "");
    }
    
    scene->GetHUDText()->SetTextDisplay(string.c_str());
}

bool Game::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode *node)
{
	if(!IW_GAME_XOML->Process(this, node))
		return false;

	return true;
}

void GameXomlAction::Execute(IIwGameXomlResource *source, CIwGameAction* action)
{
	CIwGame *game = NULL;
	CIwGameScene *scene = NULL;
	CIwGameActor *actor = NULL;

	if (source->getClassTypeHash() == CIwGameXomlNames::Scene_Hash)
	{
		scene = (CIwGameScene*)source;
		game = scene->getParent();
	}
	else
	if (source->getClassTypeHash() == CIwGameXomlNames::Actor_Hash)
	{
		actor = (CIwGameActor*)source;
		scene = actor->getScene();
		game = scene->getParent();
	}

    unsigned int statehash = action->getParameter1().getHash();
    
	switch (Type)
	{
	case Action_LoadLevel:
		{
			GAME->LoadLevel(GAME->GetWorld(), GAME->GetLevel());
		}
		break;
	case Action_StartLevel:
		{

		}
		break;
    case Action_LoadMap:
        {
            GAME->LoadMap();
        }
        break;
    case Action_ChangeState:
        {
            //GAME->ChangeState(int_State);
            //GAME->ChangeState(STATE_STORY);
        }
        break;
    case Action_LoadBoss:
        {
            GAME->LoadBoss(action->getParameter1().c_str());
        }
        break;
    case Action_LoadTestLevel:
        {
            GAME->LoadLevel(GAME->GetWorld(), GAME->GetLevel());
        }
	}
}

unsigned long Game::GetStateID() const
{
    return m_currentState->GetID();
}

void Game::LoadPauseMenu()
{
	ChangeState(STATE_PAUSE);
	findScene("HUD")->findActor("UI_Pause")->setVisible(false);
    
    CIwGameScene* pause_scn = findScene("PauseScene");
	pause_scn->findActor("UI_Resume")->setVisible(true);
	pause_scn->findActor("UI_Restart")->setVisible(true);
    pause_scn->findActor("UI_Return")->setVisible(true);
}

void Game::ShowDebugMenu()
{
    //if in debugging mode load debugging interface
    if(!GetUserOptions()->IsDebugging())
       return;
    
    CIwGameScene* scene = findScene("DebugScene");
    
    if(scene == NULL)
        IW_GAME_XOML->Process(GAME, "DebugHUD.xml");
    else
    {
        typedef CIwList<CIwGameActor*>::iterator _Iterator;
        
        for(_Iterator it = scene->begin(); it != scene->end(); ++it)
        {
            if( (*it)->getType() == UI_DEBUG_BUTTON)
                (*it)->setVisible(true);
        }
    }
}
void Game::HideDebugMenu()
{
    if(!GetUserOptions()->IsDebugging())
        return;
    
    CIwGameScene* scene = findScene("DebugScene");
    
    if(scene != NULL)
    {
        typedef CIwList<CIwGameActor*>::iterator _Iterator;
        
        for(_Iterator it = scene->begin(); it != scene->end(); ++it)
        {
            if( (*it)->getType() == UI_DEBUG_BUTTON)
                (*it)->setVisible(false);
        }
    }
}

void Game::CancelPause()
{
	if(GetCurrentState()->GetID() != STATE_PLAYING)
		ChangeState(STATE_PLAYING);

	findScene("HUD")->setAllowFocus(true);
	findScene("HUD")->findActor("UI_Pause")->setVisible(true);
    
    CIwGameScene* pause_scn = findScene("PauseScene");
    
    if(pause_scn == NULL)
        return;
    
	pause_scn->findActor("UI_Resume")->setVisible(false);
	pause_scn->findActor("UI_Restart")->setVisible(false);
    pause_scn->findActor("UI_Return")->setVisible(false);
}

/**
 *  \brief Function resets all actors to their original position and state
 *  
 */
void Game::ResetLevel()
{
    GameScene*          main_scene  = GAME->GetMainGameScene();
    HUDScene*           hud_scene   = (HUDScene*)GAME->findScene("HUD");
	CollidableActor*    actor       = (CollidableActor*)main_scene->findActor("Player");
	CIwFVec2            orig_pos    = actor->getOriginalPosition();
    GameCamera*         cam         = (GameCamera*)actor->getScene()->getCamera();
    
    // destroy all dropped objects
	main_scene->DestroyDroppedObjects();
    
	// restore draggable objects to original quantity
    hud_scene->GetHUDText()->SetTextDisplay("Restarted Level");
	hud_scene->ResetObjectQty();

	// reset enemies
	main_scene->RespawnEnemies();

	// reset scene objects
	main_scene->ResetCollectableData();
	main_scene->ResetSceneObjects();
    
	// reset player's original properties
    actor->Respawn();

	// update camera
	actor->setVelocity(0,1);
	cam->UpdateThroughUI();
    
    
}

bool Game::LoadPlayer()
{
	GameScene* scene = (GameScene*)GAME->findScene("MainScene");
	CIwGameActor* player = scene->findActor("Player");

	if(player == NULL)
	{
		if(!IW_GAME_XOML->Process(this, "LoadPlayer.xml"))
			return false;

		player = scene->findActor("Player");
	}

	player->setPosition(scene->GetPlayerStartPos());
	scene->LoadCamera(player);
	return true;
}

bool Game::LoadPlayer(CIwFVec2 pos)
{
	if(!LoadPlayer())
		return false;
	
	CIwGameActor* player = GAME->findScene("MainScene")->findActor("Player");
	player->setPosition(pos);
	player->setOriginalPosition(pos.x,pos.y);

	return true;
}

int Game::GetNextLevel() const
{
	Level++;

	return Level;
}

void Game::SetMusic(CIwGameString music_str, bool restart)
{
    CurrentMusic = music_str;

    
    PlayMusic();
}

void Game::PlayMusic()
{
    if(!UserOptions->IsMusicOn() || IW_GAME_AUDIO->isMusicPlaying())
        return;
    
    // TODO: #L1002 every time this is called the previous memory doesn't get release
    if(!IW_GAME_AUDIO->PlayMusic(CurrentMusic.c_str(),0))
        return;
}

void Game::LoadLevel()
{
    int world = World;
    int level = Level;
    LoadLevel(world, level);
}

void Game::LoadLevel(int world, int lvl)
{
    CIwGameString path = "world";
    path += world;
    path += "/xml/";
    
	CIwGameString string = "Level";
    string += CIwGameString(world).c_str();
    string += "_";
	string += CIwGameString(lvl).c_str();
	//string += ".xml";
    
    LoadLevel(path, string);
}

void Game::LoadLevel(CIwGameString load_level, CIwGameString string)
{
    CIwGameScene* no_scene = NULL;
    GAME->removeAllScenes(no_scene);
    
    bool reformat = true;
    
    //load the HUD if it doesn't exist, otherwise reset HUD
	HUDScene* scene = (HUDScene*)findScene("HUD");
	if(scene == NULL)
    {
		IW_GAME_XOML->Process(this, "TempHUD.xml");
        ShowDebugMenu();
    }
	else
		scene->ResetHUD();
    
    SetCurrentLevelString(string);
    
    string += ".xml";
    
    //path and levels together
    load_level += string;
    
    // load the mainscene
    //IW_GAME_XOML->Process(this, "main_scene.xml");
    
    //load current level, otherwise load test level
	if(!IW_GAME_XOML->Process(this, load_level.c_str()))
    {
        CIwGameError::LogError("Error loading: ", GetCurrentLevelString().c_str());
        CIwGameError::LogError("Loading Test Level");
		IW_GAME_XOML->Process(this, "Test.xml"); //start test level
        string = "Test Level";
        reformat = false;
    }
    
    //load player and display the level text in HUD
    LoadPlayer();
	DisplayStartLevel(string, reformat);
    
    //load the PauseScene if it doesn't exist
	GameScene* pause_scene = (GameScene*)findScene("PauseScene");
	if(pause_scene == NULL)
		IW_GAME_XOML->Process(this, "Pause.xml");
    
    //if in debugging mode load debugging interface
    //if(GAME->GetUserOptions()->IsDebugging())
      //  IW_GAME_XOML->Process(this, "DebugHUD.xml");
	
    PlayMusic();
    CancelPause();
    ResetLevel();
}

void Game::DestroyOldLevel()
{
	removeScene(findScene("MainScene"));

	if(findScene("Background") != NULL)
		removeScene(findScene("Background"));

    if(findScene("LevelComplete") != NULL)
        removeScene(findScene("LevelComplete"));
}

CIwFVec2 Game::FindSmallestVertex(CIwGameGeomShapePolygon* polygon)
{
    CIwFVec2 smallest   = polygon->Vertices[0];
    int num_vert        = polygon->NumVertices;
    
    for(int i = 1; i < num_vert; i++)
    {
        if(polygon->Vertices[i].x < smallest.x)
            smallest.x = polygon->Vertices[i].x;
        
        if(polygon->Vertices[i].y < smallest.y)
            smallest.y = polygon->Vertices[i].y;
    }
    
    return smallest;
}

CIwFVec2 Game::FindLargestVertex(CIwGameGeomShapePolygon* polygon)
{
    CIwFVec2 largest   = polygon->Vertices[0];
    int num_vert        = polygon->NumVertices;
    
    for(int i = 1; i < num_vert; i++)
    {
        if(polygon->Vertices[i].x > largest.x)
            largest.x = polygon->Vertices[i].x;
        
        if(polygon->Vertices[i].y > largest.y)
            largest.y = polygon->Vertices[i].y;
    }
    
    return largest;
}

void Game::LoadDebugHUD()
{
    IW_GAME_XOML->Process(GAME, "DebugHUD.xml");
    CIwGameScene* debug_scene = findScene("DebugScene");
    GAME->SetHUDText(NULL);
    /*
    if(debug_scene != NULL)
    {
        HUDTextDisplay* hud_text = hud_scene->GetHUDText();
        GAME->SetHUDText(hud_text);
    }
     */
}

void Game::LoadBoss(const char* bossName)
{
    CIwGameString boss = CIwGameString(bossName).c_str();
    
    boss = "bosses/";
    boss += bossName;
    boss += ".xml";
    
    IW_GAME_XOML->Process(this, boss.c_str());
    
    //CIwGameError::LogError("Loading Boss..", CIwGameString(bossName).c_str());
}
void Game::ReloadHUDText()
{
    //HUDTextDisplay* hud = ((HUDScene*)GAME->findScene("HUD"))->GetHUDText();
    //SetHUDText(hud);
   // HUDText->SetTextDisplay("");
}
void Game::DisplayTextInHUD(CIwGameString string)
{
    if(!GetUserOptions()->IsDebugging())
        return;
    
    CIwGameString text = string;
    GAME->GetHUDText()->SetTextDisplay(text.c_str());
    
}

GameScene* Game::GetMainGameScene()
{
    return static_cast<GameScene*>(findScene("MainScene"));
}
