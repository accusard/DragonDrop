//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// scene.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// The implementation of scene.cpp
//--------------------------------------------------------------------------------------------------------

#include "scene.h"
#include "game.h"
#include "game_object_actor.h"
#include "player_actor.h"
#include "enemy_actor.h"
#include "debug_actor_text.h"
#include "IwGameTemplates.h"
#include "gamecamera.h"
#include "debug_actor_text.h"
#include "actors/collectable_actor.h"
#include "bomb_actor.h"
#include "stone_actor.h"
#include "trampoline_actor.h"
#include "game_state.h"

GameScene::GameScene() : SpawnPosition(0,0), m_safeToSpawnObject(false), DropActor(NULL), DropSensor(NULL), PrevTimeStep(ConstantTimeStep),PlayerStartPos(0,0), GoalPos(0,0), CameraBound(0,0,0,0), m_numOfOrbsCollected(0), m_numOfMapsCollected(0) {}

GameScene::~GameScene()
{
    
}
    
int GameScene::GetNumOfOrbsCollected() const
{
    return m_numOfOrbsCollected;
}

int GameScene::GetNumOfMapsCollected() const
{
    return m_numOfMapsCollected;
}

void GameScene::CollectItem(int type)
{
    switch (type)
    {
        case MAP:
        {
            m_numOfMapsCollected++;
        } break;
            
        case ORB:
        {
            m_numOfOrbsCollected++;
        } break;
            
        default:
            break;
    }
}

/**
 *  \brief
 *  
 */
int GameScene::Init(int max_collide, int max_layers, bool doSleep)
{
    Box2dPhysicsUti box2d;
    
	CIwGameScene::Init(max_collide, max_layers, doSleep);
    
    CIwGameAnimTimeline* timeline = NULL;
    
    //check if timeline is local
    timeline = getTimelinesManager()->findTimeline("blink_red");
    if(timeline == NULL)
        timeline = (CIwGameAnimTimeline*)IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->findTimeline("blink_red");
    
    
    DropSensor = new SensorColorActor();
    addActor(DropSensor);
    
    DropSensor->Init(NULL, timeline);
    DropSensor->SetAttributes("SensorActor", CIwFVec2(0,0), SENSOR);
    //DropSensor->AssignBox2dPhysics("SensorShape", "SensorMaterial", true);
    DropSensor->setActive(false);
    
    //getBox2dWorld()->getWorld()->SetDebugDraw(&GAME->m_debugDraw);
    //GAME->m_debugDraw.SetFlags(DebugDraw::e_shapeBit);
    
	return 1;
}

void GameScene::Update(float dt)
{
    if(m_respawnTimer.HasTimedOut())
        GAME->ResetLevel();
    
    // TODO: B3893 move to Game::Update to utilize m_stateChange
    if(GAME->GetStateID() != STATE_PAUSE)
        CIwGameScene::Update(dt);

}

/**
 * \brief Create an object of a certain type and spawn it at the specified location
 *
 * \param type The type of the object.
 * \param position The position to spawn at.
 */
void GameScene::DropObject(int type, CIwFVec2 position)
{
    ObjectData              objdata;
	CIwGameScene*           scene;
    int                     w, h;
    float                   vel_y;
    CIwGameAnimTimeline*	timeline;
	const char*             timeline_name;
    CIwGameBrushImage*      image;
    GameObjectActor*        actor;
    
    if(type == DRAG_PLATFORM)
        type = DROP_PLATFORM;
    
    scene   = GAME->findScene("MainScene");
    objdata = GAME->GetBox2dObjectData(type);
    
	// object position is based on zoom level of the camera
	objdata.position.x	= position.x / scene->getCamera()->getScale();
	objdata.position.y	= position.y / scene->getCamera()->getScale();

    actor = (GameObjectActor*)CreateNewGameObject(objdata.type);
    
	image	= (CIwGameBrushImage*)ResourceManager->FindResource(objdata.image, CIwGameXomlNames::Brush_Hash);
	w		= image->getSrcRect().w;
	h		= image->getSrcRect().h;


	timeline_name = "Object_Drop";

	timeline    = IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->findTimeline(timeline_name);
    
	vel_y       = CIwFVec2(getBox2dWorld()->getGravity()).y / (float)2.1;
    
	actor->setScene(this);
	actor->Init(image, w, h, objdata);
	actor->setVelocity(0, -vel_y);
    actor->setTimeline(timeline);
    
    actor->SetAsDroppedObject();
    //actor->setActualClassType("droppedgameobjectactor");
    
	this->addActor(actor);
	timeline->restart();
}

/**
 * \brief This function returns a GameObjectActor class
 *
 *  TODO: Will need to update each case to return a different derived class of GameObjectActor
 */
CIwGameActor* GameScene::CreateNewGameObject(int type)
{
    GameObjectActor* actor = NULL;
    switch (type)
    {
        case DROP_BOMB:
        {
            actor = new BombActor();
        }
        break;
            
        case DROP_PLATFORM:
        {
            actor =  new PlatformActor();
        }
        break;
            
        case DROP_CANDLE:
        {
            actor =  new GameObjectActor();
        }
        break;
        case DROP_STONE:
        {
            actor =  new StoneActor();
        }
        break;
        case DROP_TRAMPOLINE:
        {
            actor =  new TrampolineActor();
        }
        break;
    }

    return actor;
}

bool GameScene::LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode *node)
{
	if(!CIwGameScene::LoadFromXoml(parent, load_children, node))
		return false;
	
	// Load XOML tags
	for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int attrib_name_hash = (*it)->getName().getHash();

		if(attrib_name_hash == CIwGameXomlNames::PhysicsTimestep_Hash)
		{
			PrevTimeStep = (*it)->GetValueAsFloat();
			//break;
		}
		else
		if(attrib_name_hash == CIwGameString("playerstartpos").getHash())
		{
            bool debugging = GAME->GetUserOptions()->IsDebugging();
            bool custom_pos = GAME->GetUserOptions()->HasCustomPos();
            
            if(debugging && custom_pos)
            {
                PlayerStartPos = GAME->GetUserOptions()->GetPlayerPos();
                //break;
            }
            else
            (*it)->GetValueAsPoint(PlayerStartPos);
		}
        else
        if(attrib_name_hash == CIwGameString("camerabound").getHash())
        {
            bool boundCam = GAME->GetUserOptions()->BoundCam();
            
            if(boundCam)
                (*it)->GetValueAsRect(CameraBound);
        }
        else
        if(attrib_name_hash == CIwGameString("music").getHash())
        {
            CIwGameString music_str = (*it)->GetValue();
            CIwGameString curr_str = GAME->GetCurrentMusic();
            
            if(GAME->GetUserOptions()->IsMusicOn())
            {
                if(curr_str.getHash() != music_str.getHash())
                {
                    GAME->SetMusic(music_str.c_str());
                }
            }
        }
	}
    
    LoadCollectionData();
    
	return true;
}

void GameScene::NotifySuspending(CIwGameScene* new_scene)
{
	CIwGameScene::NotifySuspending(new_scene);
    
    PauseObjectTimers();
}

void GameScene::NotifyResuming(CIwGameScene* old_scene)
{
    CIwGameScene::NotifyResuming(old_scene);
    
    ResumeObjectTimers();
}

/**
 * \brief Goes through the scene's actor list and pause the actor's timer
 */
// TODO: Z8511 fix function to not go through every single object in the scene
void GameScene::PauseObjectTimers()
{
    for(_Iterator it = begin(); it != end(); ++it)
    {
        // leave actors of this type alone
        if( (*it)->getType() == PARTICLES )
            return;
        
        GameTimer* timer;
        timer = static_cast<CollidableActor*>(*it)->GetObjectTimer();
        if(timer != NULL)
            timer->Pause();
    }
}

/**
 * \brief Goes through the scene's actor list and resume the actor's timer
 */
// TODO: Z8510 fix function to not go through every single object in the scene
void GameScene::ResumeObjectTimers()
{
    for(_Iterator it = begin(); it != end(); ++it)
    {
        // leave actors of this type alone
        if( (*it)->getType() == PARTICLES )
            return;
        
        GameTimer* timer;
        timer = static_cast<CollidableActor*>(*it)->GetObjectTimer();
        if(timer != NULL)
            timer->Resume();
    }
}

CIwGameActorParticles* GameScene::ExplodeActor(CIwGameActor* actor, int num_particles)
{
    IW_GAME_AUDIO->PlaySound("dynamite_explode");
	CIwGameActorParticles* explosion_particles = new CIwGameActorParticles();
	addActor(explosion_particles);
    
	CIwGameImage* brush_image = NULL;
	CIwGameBrushImage* brush = (CIwGameBrushImage*)actor->getVisual()->getBrush();
	if(brush != NULL)
		brush_image = brush->getImage();
	else
		CIwGameError::LogError("WARNING: ExplodeActor can only explode actors with brush images!");

	explosion_particles->Init(num_particles);
	explosion_particles->setImage(brush_image);
	explosion_particles->setPosition(actor->getPosition());
	explosion_particles->setLayer(9);
    explosion_particles->setType(PARTICLES);
	CIwRect src_rect = brush->getSrcRect();

	//only need pieces of the exploded actor
	src_rect.w *= 0.5;
	src_rect.h *= 0.5;

	CIwFVec4 color(255,255,255,255);
	CIwFVec4 color_vel(0,0,0,0);

	//RandomMode
	explosion_particles->setAngleMode(CIwGameActorParticles::PAM_Random);
	explosion_particles->setAngVelocityMode(CIwGameActorParticles::PAM_Random);
	explosion_particles->setVelocityMode(CIwGameActorParticles::PAM_Random);
	explosion_particles->setScaleMode(CIwGameActorParticles::PAM_Random);
	explosion_particles->setPositionMode(CIwGameActorParticles::PAM_Random);

	//Set the range for the randomized parameters
	CIwFVec2 scale_r(0.6, 1);
	explosion_particles->setScaleRange(scale_r);

	CIwFVec2 angle_r(0,360);
	explosion_particles->setAngleRange(angle_r);

	CIwFVec2 angle_vel_r(-5,5);
	explosion_particles->setAngVelocityRange(angle_vel_r);

	CIwFVec4 velocity_r(-50,50,-50,50);
	explosion_particles->setVelocityRange(velocity_r);

	CIwFVec2 pos_r(50,50);
	explosion_particles->setPositionRange(pos_r);

	//generate the particles
	explosion_particles->GenerateRandomParticles(num_particles, src_rect, color, color_vel, 2, 1, 0,1);

	//shake background
	CIwGameActor* foreground = findActor("Foreground");
	ShakeActor(foreground);

	return explosion_particles;
}

CIwRect GetHalfImageRect(CIwGameBrushImage* image)
{
    CIwRect rect = image->getSrcRect();
    rect.w *= 0.5f;
    rect.h *= 0.5f;
    
    return rect;
}

CIwGameActorParticles* GameScene::SliceActor(CIwGameActor* actor, CIwFVec4 vRange_r, CIwFVec4 vRange_l)
{
	int num_particles = 1; 

	CIwGameActorParticles* particle = new CIwGameActorParticles;
	addActor(particle);

	CIwGameImage* image = NULL;
	CIwGameBrushImage* brush = (CIwGameBrushImage*)actor->getVisual()->getBrush();
	if(brush != NULL)
		image = brush->getImage();
	else
		CIwGameError::LogError("WARNING: SliceActor() can only slice actors with brush images!");

	particle->Init(num_particles);
	particle->setImage(image);
	particle->setPosition(actor->getPosition());
	particle->setLayer(9);

	CIwRect src_rect1 = brush->getSrcRect();
	CIwRect src_rect2 = brush->getSrcRect();

	src_rect1.h = src_rect1.h * 0.5;// * 0.6;
	src_rect2.w = src_rect2.w * 0.5;

	CIwFVec4 color(255,255,255,255);
	CIwFVec4 color_vel(0,0,0,-5);
	
	particle->setAngleMode(CIwGameActorParticles::PAM_Random);
	particle->setAngVelocityMode(CIwGameActorParticles::PAM_Random);
	particle->setVelocityMode(CIwGameActorParticles::PAM_Random);

	CIwFVec2 angle_r(0,360);
	particle->setAngleRange(angle_r);

	CIwFVec2 angle_vel_r(0,10);
	particle->setAngVelocityRange(angle_vel_r);

	CIwFVec4 velocity_r(vRange_r);
	particle->setVelocityRange(velocity_r);

	particle->GenerateRandomParticles(num_particles, src_rect1, color, color_vel, 1, 1, 0,0.5f);

	particle->setAngleMode(CIwGameActorParticles::PAM_Normal);
    
	CIwFVec4 velocity_l(vRange_l);
	particle->setVelocityRange(velocity_l);

	particle->GenerateRandomParticles(num_particles, src_rect2, color, color_vel, 1, 1, 0,0.5f);

	((BaseActor*)actor)->setActive(false);
	return particle;
}

CIwGameActor* GameScene::AssignPhysicsProperty(CIwGameActor* actor, ObjectData* data)
{
	CIwGameShape* shape = (CIwGameShape*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource(data->shape, CIwGameXomlNames::Shape_Hash);
	CIwGameBox2dMaterial* material = (CIwGameBox2dMaterial*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource(data->material, CIwGameXomlNames::Box2dMaterial_Hash);
    
    CIwFVec2 actor_pos = actor->getPosition();
    
	if(shape == NULL)
		CIwGameError::LogError("Could not find shape");
	if(material == NULL)
		CIwGameError::LogError("Could not find material");
		
	//Box2d properties
	actor->setAngle(-actor->getAngle());
	CIwGameBox2dBody* body = actor->getBox2dBody();
	body = new CIwGameBox2dBody();
	body->setBodyShape(shape);
	body->InitBody(this->getBox2dWorld(), shape, material, &actor_pos, actor->getAngle(), 0, 0);
	body->setCollisionFlags(1,1,1);
	body->setUserData(actor);

	actor->setVelocityDamping(0,0);
	actor->setAngularVelocityDamping(0);
	actor->setCollidable(true);
	actor->setBox2dBody(body);

	delete body;
		
	return actor;
}

void GameScene::ShakeActor(CIwGameActor* actor)
{
	if(actor != NULL)
	{
		CIwGameAnimTimeline* timeline = getTimelinesManager()->findTimeline("shake_actor");
		if(timeline != NULL)
		{
			actor->setTimeline(timeline);
			timeline->restart();
		}
		else
			CIwGameError::LogError("Could not find timeline");
	}
}

void GameScene::SquashActor(CIwGameActor* actor)
{
    CIwGameAnimTimeline* timeline = (CIwGameAnimTimeline*)getTimelinesManager()->FindTimeline("squash_actor", this);

		if(timeline != NULL)
		{
			actor->setVelocity(0,0);
			actor->setCollidable(false);
			actor->setTimeline(timeline);
            timeline->restart();
		}
		else
			CIwGameError::LogError("Could not locate timeline resource.");

		((BaseActor*)actor)->setActive(false);
}

void GameScene::SaveSceneData()
{
    CIwGameFile     file;
    CIwGameString   file_path;
    CIwGameString   level_str;

    file_path = "\\scenedata";
    level_str = GAME->GetCurrentLevelString();
    level_str.Replace("Level", "");
    
    file_path += level_str;
    file_path += ".bin";
    
    file.Open(file_path.c_str(), "wb");
    
    for(_Iterator it = begin(); it != end(); ++it)
    {
        int type = (*it)->getType();
        
        if(type == MAP || type == ORB)
        {
            CollectableActor* c_item = (CollectableActor*)(*it);
            if(c_item->HoldToCollect())
            {
                int item_type = c_item->Collect();
                this->CollectItem(item_type);
            }
            
            this->SaveCollectedToData(c_item, file);
        }
    }

}

/**
 * \brief Add up the total number of collected items to the global game variable
 */
void GameScene::AddUpCollectables()
{
    GAME->AddNumOfMapFragments(m_numOfMapsCollected);
    GAME->AddNumOfOrbs(m_numOfOrbsCollected);
    
    m_numOfOrbsCollected = 0;
    m_numOfMapsCollected = 0;
}
void GameScene::SaveCollectedToData(CIwGameActor* c_item, CIwGameFile &file)
{
    SceneData       data;
    
    int             id = ((CollectableActor*)c_item)->GetID();
    bool            c = ((CollectableActor*)c_item)->HasCollected();
    
    data.item_id    = id;
    data.collected  = c;
    
    file.Write(&data, sizeof(SceneData));
}

/**
 * \brief Load data from the file to the scene.
 */
void GameScene::LoadCollectionData()
{
    
    CIwGameFile     file;
    CIwGameString   file_path;
    CIwGameString   level_str;
    
    file_path = "\\scenedata";
    level_str = GAME->GetCurrentLevelString();
    level_str.Replace("Level", "");
    
    file_path += level_str;
    
    file_path += ".bin";
    
    if(file.Open(file_path.c_str(), "rb"))
    {
        do
        {
            SceneData  data;

            if(!file.Read(&data, sizeof(SceneData)))
                break;

            CollectableActor* c_item = (CollectableActor*)FindCollectionItemInScene(data.item_id);

            if(c_item != NULL)
                c_item->SetCollected(data.collected);

        }
        while(1==1);
    }
    
    
}


CIwGameActor* GameScene::FindCollectionItemInScene(int _id)
{
    CollectableActor* actor;
    
    for(_Iterator it = begin(); it != end(); ++it)
    {
        actor = (CollectableActor*)(*it);
        if(actor->GetID() == _id)
        {
            return actor;
        }
    }
    
    return NULL;
}

/**
 * \brief   Destroy objects that have been dropped into the scene by the player. The object needs to have
 * its ActualClassTypeHash be set as "isdropped"
 */
void GameScene::DestroyDroppedObjects()
{
    unsigned int is_dropped_hash = CIwGameString("isdropped").getHash();
    
    for(_Iterator it = begin(); it != end(); ++it)
    {
        if( (*it)->getActualClassTypeHash() == is_dropped_hash)
        {
            (*it)->setDestroyed(true);
        }
    }
}

/**
 * \brief   Reset all game objects' original state in the scene that wasn't dropped by the player
 */
void GameScene::ResetSceneObjects()
{
    int                 obj_type;
    CollidableActor*    resettable_actor;
    CollectableActor*   c_item;
    
    for(_Iterator it = begin(); it != end(); ++it)
    {
        resettable_actor    = (CollidableActor*)(*it);
        obj_type            = (*it)->getType();

        //reset original state
        if(obj_type != PARTICLES)
            resettable_actor->Respawn();
        
        //reset collectibles
        if(obj_type == MAP || obj_type == ORB)
        {
            
            c_item = (CollectableActor*)(*it);
            
            c_item->setActive(!c_item->HasCollected());
            
            if(!c_item->HasCollected())
                c_item->SetHoldToCollect(false);
            
        }
    }
}

void GameScene::RespawnEnemies()
{
	for(_Iterator it = begin(); it != end(); ++it)
	{
		if((*it)->getType() == ENEMY)
		{
			BaseActor* actor = (BaseActor*)(*it);
			actor->Respawn();
		}
	}
}

void GameScene::EndOfLevel()
{
    
	IW_GAME_XOML->Process(GAME, "LevelComplete.xml");
    
	HUDScene* scene = (HUDScene*)GAME->findScene("HUD");
	CIwGameActor* pause_actor = scene->findActor("UI_PAUSE");
    
	if(scene != NULL)
		scene->setAllowFocus(false);
    
	if(pause_actor != NULL)
		pause_actor->setVisible(false);
}

void GameScene::EndOfBoss()
{
    IW_GAME_XOML->Process(GAME, "boss_complete.xml");
    
	HUDScene* scene = (HUDScene*)GAME->findScene("HUD");
	CIwGameActor* pause_actor = scene->findActor("UI_PAUSE");
    
	if(scene != NULL)
		scene->setAllowFocus(false);
    
	if(pause_actor != NULL)
		pause_actor->setVisible(false);
}

void GameScene::SetRespawnTimer(uint64 duration)
{
    m_respawnTimer.setDuration(duration);
}

/**
 *  \brief  Reset the number of maps and orbs collected
 *  TODO:   need to move action bar to a different function
 */
void GameScene::ResetCollectableData()
{
    // reset collected data
    m_numOfMapsCollected = 0;
    m_numOfOrbsCollected = 0;
    

    //action bar
	CIwGameActor* actionbar = (CIwGameActor*)GAME->findScene("HUD")->findActor("ActionBar");
	actionbar->setActive(false);

}

bool GameScene::StartCollisionEvent(e_conditions cond_type, BaseActor* target, int target_state)
{
    bool start = false;
    ActorState* curr_state = target->GetActorStateManager()->GetCurrentState();
    
    /*
    // check if current state allow state-change
    start = curr_state->CheckStateCondition(cond_type);
    if(start)
    {
        target->SetCondition(cond_type, true);
        target->ChangeState(target_state);
    }
    */
    return start;
}

void GameScene::SetPlayerStartPos(float x, float y)
{
	PlayerStartPos.x = x;
	PlayerStartPos.y = y;
}

void GameScene::SetGoalPos(float x, float y)
{
	GoalPos.x = x;
	GoalPos.y = y;
}

void GameScene::LoadCamera(CIwGameActor* target)
{
    /*
    //find the first and last fixture in the scene
	CIwGameActor* first_fixture = NULL;
    CIwGameActor* last_fixture = NULL;
    
    for(_Iterator it = begin(); it != end(); it++)
    {
        if((*it)->getType() == FIXTURE)
            last_fixture = (*it);
            
        if((*it)->getType() == FIXTURE && first_fixture == NULL)
        {
            first_fixture = (*it);
        }
    }

    CIwGameError::LogError("first fixture: ", first_fixture->getName().c_str());
    CIwGameError::LogError("last fixture: ", last_fixture->getName().c_str());
    */
    
	CIwRect bound = CIwRect(0,0,0,0);
    
	setCamera(new GameCamera());
	GameCamera* cam = (GameCamera*)getCamera();

	cam->SetTarget(target);
	cam->SetCameraYOffset(CAM_OFFSET_Y);
	cam->SetCameraXOffset(CAM_OFFSET_X);
	cam->SetEasingRange(CIwFVec2(0,0));
	cam->setScale(ZOOM_LEVEL);

	// Load the camera boundary
    bound.x = CameraBound.x;
    bound.y = CameraBound.y;
    bound.w = CameraBound.w;
    bound.h = CameraBound.h;
    
	cam->SetBoundary(bound);
}

CIwFVec2 GameScene::HudToMainScene(float x, float y)
{
    CIwFVec2 pos = CIwFVec2(x,y);
    
    pos.x += getCamera()->getPosition().x; // / getCamera()->getScale());
    pos.y += getCamera()->getPosition().y; // / getCamera()->getScale());
    
    return pos;
}

//--------------------------------------------------------------------------------------------------------
//	HUDScene implementation
//--------------------------------------------------------------------------------------------------------

int HUDScene::Init(int maxcollide, int maxlayers, bool doSleep)
{
    CIwGameScene::Init(maxcollide, maxlayers, doSleep);
    
    CIwGameFont* font = (CIwGameFont*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource("trebuchet_12", CIwGameXomlNames::Font_Hash);
    CIwGameString text = "";
    
    if(font != NULL)
    {
        HUDTextDisplay* hudDisplay = new HUDTextDisplay();
        addActor(hudDisplay);
        hudDisplay->setScene(this);
        hudDisplay->setName("HUDText");
        hudDisplay->InitTextDisplay(font, text);
        HUDText = hudDisplay;
        GAME->SetHUDText(hudDisplay);
    }
    
	return 1;
}

void HUDScene::ResetObjectQty()
{
    for(_Iterator it = begin(); it != end(); ++it)
    {
        if((*it)->getActualClassTypeHash() == CIwGameString("baricon").getHash())
        {
            BarIcon* icon = (BarIcon*)(*it);
            icon->ResetQuantity();
        }
    }
}

void HUDScene::Update(float dt)
{
	CIwGameScene::Update(dt);
}

bool HUDScene::LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode *node)
{
	if(!CIwGameScene::LoadFromXoml(parent, load_children, node))
		return false;
    
    setLayer(L_HUD);
    
	return true;
}
void HUDScene::SetHUDTextDisplay(HUDTextDisplay* hud)
{
    HUDText = hud;
}

void HUDScene::ResetHUD()
{
    CIwGameScene* pause_scene = GAME->findScene("PauseScene");
    
    GAME->removeScene(this);
    
    if(pause_scene != NULL)
        GAME->removeScene(pause_scene);
    
	IW_GAME_XOML->Process(GAME, "TempHUD.xml");
    GAME->BringSceneToFront(GAME->findScene("HUD"));
    HUDText->SetTextDisplay("");
    /*
    ButtonIcon* actor = NULL;
    
    actor = (ButtonIcon*)GAME->findScene("PauseScene")->findActor("UI_Resume");
    LinkHUDTextToButton(actor);

    actor = (ButtonIcon*)GAME->findScene("PauseScene")->findActor("UI_Restart");
    LinkHUDTextToButton(actor);
     */
}
void HUDScene::AddNumOfDebugButton()
{
    m_numOfDebugButtons++;
}

int HUDScene::GetNumOfDebugButtons() const
{
    return m_numOfDebugButtons;
}
/*
void HUDScene::LinkHUDTextToButton(ButtonIcon* actor)
{    
    CIwGameError::LogError("Linking HUD Text to button..");
    actor->SetHUDText(HUDText);
}
*/





