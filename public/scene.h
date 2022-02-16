//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// scene.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#if !defined(_SCENE_H_)
#define _SCENE_H_

#include "IwGameScene.h"
#include "IwGameUI.h"
#include "IwList.h"

#include "actors/static_actor.h"
#include "game.h"
#include "explosion.h"
#include "actors/toolbar.h"
#include "objectdata.h"
#include "actors/base_actor.h"
#include "actors/debug_actor_text.h"
#include "condition.h"
#include "objects/lighting.h"
#include "game_timer.h"

class SensorColorActor;
class HUDTextDisplay;
class ButtonIcon;
class BaseActor;
class PlayerActor;

/**
 * \brief Used to store data for the scene.
 */
struct SceneData
{
    int     item_id;
    bool    collected;
};

//
//
//	GameScene Class
//
//

class GameScene : public CIwGameScene
{
private:
	CIwFVec2				SpawnPosition;          // Used to keep object's safe spawn position when game is paused
	bool					m_safeToSpawnObject;    /**< Set to true if dragged objects can be spawned into the scene */
	CIwGameActor*			DropActor;              // The current object to spawn and drop in the scene
	SensorColorActor*       DropSensor;             // Non visual actor to sense the collision boundaries of various other actors
	float					PrevTimeStep;           // Record previous TimeStep to go back to when unpausing
	CIwFVec2				PlayerStartPos;         // Player's initial start position
	CIwFVec2				GoalPos;                // Location where the player needs to be to end the level
    CIwRect                 CameraBound;            // limit the camera to a RECT
    
    GameTimer               m_respawnTimer;           /**< A timer use to determine when to reset the level */
    
    // collected items for this scene
    int                     m_numOfOrbsCollected;
    int                     m_numOfMapsCollected;
    
protected:

public:    
	GameScene();
	~GameScene();
    
	int				Init(int max_collide=128, int max_layers=10, bool doSleep=true);
	void			Update(float dt);
	bool			LoadFromXoml(IIwGameXomlResource *parent, bool load_children, CIwGameXmlNode *node);
	void			NotifySuspending(CIwGameScene* new_scene);
    void            NotifyResuming(CIwGameScene* old_scene);
    
    void            PauseObjectTimers();
    void            ResumeObjectTimers();
    
	void            DropObject(int type, CIwFVec2 position);	//Spawn an object in the scene using type and position
    CIwGameActor*   CreateNewGameObject(int type);
    
	bool			IsSpawnSafe() const {return m_safeToSpawnObject;}
	void			SetSpawnSafe(bool safe) { m_safeToSpawnObject = safe; }
	float			GetPrevTimeStep() { return PrevTimeStep; }
    void            SetRespawnTimer(uint64 duration);
    
	void			ResetCollectableData();
    
    bool            StartCollisionEvent(e_conditions cond_type, BaseActor* target, int target_state);
                    // check condition type and then switches state
    
	CIwFVec2		GetPlayerStartPos() const { return PlayerStartPos; }
	void			SetPlayerStartPos(float x, float y);
	CIwFVec2		GetGoalPos() const {return GoalPos; }
	void			SetGoalPos(float, float);
	void			LoadCamera(CIwGameActor*);
    CIwFVec2        GetSpawnPosition() const { return SpawnPosition; }
    void            SetSpawnPosition(CIwFVec2 pos) { SpawnPosition = pos; }
    CIwGameActor*   GetDropActor() const { return DropActor; }
    void            SetDropActor(CIwGameActor* actor) { DropActor = actor; }
    SensorColorActor* GetSensorActor() const { return DropSensor; }
    CIwFVec2        HudToMainScene(float x, float y);
    CIwRect        GetCameraBound() const {return CameraBound;}
    
	// Particle/special effects
	//CIwGameActor*	CreateExplosion(CIwFVec2 position, float explosion_radius);				// Create explosion particles at position
	CIwGameActor*	AssignPhysicsProperty(CIwGameActor* actor, ObjectData* data);			//Assign physics properties
	void			ShakeActor(CIwGameActor* actor);										// Shake effect

	//Death and destruction
	CIwGameActorParticles*	ExplodeActor(CIwGameActor* actor, int num_particles);
	CIwGameActorParticles*	SliceActor(CIwGameActor* actor, CIwFVec4 range_right, CIwFVec4 range_left);
	void					SquashActor(CIwGameActor* actor);
    
    // saving
    void        SaveSceneData();
    void        SaveCollectedToData(CIwGameActor* c_item, CIwGameFile &file);
    void        LoadCollectionData();
    
    // collectables
    int             GetNumOfOrbsCollected() const;
    int             GetNumOfMapsCollected() const;
    void            CollectItem(int type);
    void            AddUpCollectables();
    
    CIwGameActor*   FindCollectionItemInScene(int _id);
    
public:
   
    void    ResetSceneObjects();
    void    DestroyDroppedObjects();							// remove all player-placed game objects
	void	RespawnEnemies();									// Respawn enemies
	void	EndOfLevel();
    void    EndOfBoss();
};

class SceneCreator : public IIwGameXomlClassCreator
{
public:
	SceneCreator()
	{
		setClassName("gamescene");
	}
	IIwGameXomlResource *CreateInstance(IIwGameXomlResource *parent)
	{
		return new GameScene();
	}
};

//
//
//	HUDScene Class
//
//

class HUDScene : public CIwGameScene
{
private:
    HUDTextDisplay*     HUDText;
    int                 m_numOfDebugButtons; /**< keep track of the number of debug buttons and marginize accordingly */
    
public:
	HUDScene() : HUDText(NULL), m_numOfDebugButtons(0) {}
    ~HUDScene() {}
    
	int                 Init(int max_collide=128, int max_layers=10, bool doSleep=true);
	void                Update(float dt);
	bool                LoadFromXoml(IIwGameXomlResource *parent, bool children, CIwGameXmlNode* node);
    HUDTextDisplay*     GetHUDText() const { return HUDText; }
    void                ResetHUD();
    void                SetHUDTextDisplay(HUDTextDisplay* hud);
    
    void                AddNumOfDebugButton();
    int                 GetNumOfDebugButtons() const;
    void                ResetObjectQty();
    //void              LinkHUDTextToButton(ButtonIcon* actor);
};

class HUDCreator : public IIwGameXomlClassCreator
{
public:
	HUDCreator()
	{
		setClassName("hudscene");
	}
	IIwGameXomlResource *CreateInstance(IIwGameXomlResource* parent)
	{
		return new HUDScene();
	}
};

//
//
//	MapScene Class
//
//

class MapScene : public CIwGameScene
{
private:
    int     MapsCollected;
    int     OrbsCollected;
    
public:
    MapScene() : CIwGameScene(), MapsCollected(0), OrbsCollected(0)
    {
        
    }
    
    int     GetMapsCollected() const {return MapsCollected; }
    int     GetOrbsCollected() const {return OrbsCollected; }
    void    AddMapsCollected(int num) {MapsCollected += num;}
    void    AddOrbsCollected(int num) {OrbsCollected += num;}
};

#endif
