//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// game.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Game class for the main game engine
//--------------------------------------------------------------------------------------------------------

#if !defined(_GAME_H_)
#define _GAME_H_

#include "IwGame.h"
#include "IwGameActorParticles.h"
#include "IwList.h"

#include "debugdraw.h"
#include "objectdata.h"
//#include "scene.h"
/*
#include "scene.h"
#include "gamecamera.h"
#include "actors/base_actor.h"
#include "actors/debug_actor_text.h"
#include "actors/toolbar.h"
#include "options.h"
#include "game_state.h"
#include "utility_actor.h"

*/

//#define SAFE_DELETE_LIST(x) for (_Iterator it = x.begin(); it != x.end(); ++it) delete *it;

// global variables
const CIwFVec4 WHITE_COLOR = CIwFVec4(255,255,255,255);
const CIwFVec4 BLACK_COLOR = CIwFVec4(0,0,0,255);
const CIwFVec4 RED_COLOR = CIwFVec4(255,0,0,255);

const int EXT_TOOLBAR_ICON_HEIGHT = 175;
const int EXT_BUTTON_ICON_HEIGHT = 150;

//The singleton class
#define GAME	Game::getInstance()

class GameOptions;
class State;
class GameState;
class HUDTextDisplay;
class GameScene;

struct GameData
{
    int collected_maps;
    int collected_orbs;
};

enum e_droppedObjects	{   DROP_PLATFORM=500,
                            DRAG_PLATFORM,
                            DROP_TRAMPOLINE,
                            DROP_STONE,
                            DROP_BOMB,
                            DROP_CANDLE,
                            TOTAL_OBJECTS };

enum e_EntityType		{   ENEMY=300,
                            PLAYER,
                            EXPLOSION,
                            PARTICLES,
                            FIREBREATH,
                            SENSOR,
                            CONSTRICTOR,
                            OBSTACLES,
                            GOAL_FLAG,
                            ORB,
                            MAP,
                            EVENT_TRIGGER,
                            SPIKED_BALL};

enum e_ForegroundType   {   FORE_GROUND = 400,
                            FIXTURE,
                            PARALLAX_BG,
                            AESTHETIC_BG };

enum e_InterfaceType	{   UI_LEFT = 200,
                            UI_RIGHT,
                            UI_PAUSE,
                            UI_JUMP,
                            UI_RESUME,
                            UI_RESTART,
                            UI_NEXT,
                            UI_ACTION,
                            UI_DEBUG_BUTTON,
                            UI_TEST,
                            UI_LEVEL_SELECT,
                            UI_RETURN,
                            UI_BUTTON,
                            UI_STORYBOARD,
                            UI_MAX};

enum e_GameLayers		{   L_BACKGROUND,
                            L_HIDDEN,
                            L_FOREGROUND,
                            L_FIXTURE,
                            L_COLLECTIBLES,
                            L_ACTORS,
                            L_TEXT,
                            L_HUD,
                            L_DEBUG = 9 };

class Game	:	public CIwGame
{
	CDEFINE_SINGLETON(Game)
protected:
    mutable int         World;
    mutable int         Level;
    CIwGameString       CurrentLevelString;
	GameOptions*        UserOptions;
    HUDTextDisplay*     HUDText;
    CIwGameString       CurrentMusic;

    CIwList<State*>     m_states;
    State*              m_currentState;
    bool                m_stateChanged;
    unsigned int        m_lastState;
    CIwGameString       m_storyboard;

	int					m_collectedMaps;    /**< The total number of maps collected in the game. */
	int					m_collectedOrbs;    /**< The total number of orbs collected in the game. */
    CIwGameString       m_Data;             /**< TODO: need to preallocate memory string to avoid
                                             memory fragmentations */
	
public:
	typedef CIwList<State*>::iterator _StateIterator;
    _StateIterator       begin() {return m_states.begin(); }
    _StateIterator       end() {return m_states.end(); }
    
    void	Init(bool enable_http = false, bool enable_extensions = true, bool gl_conmpatible = false);
	void	Release();
	bool	Update();
	void	Draw();
    bool	LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode *node);
    
    // saving
    void    LoadGameData();
    void    SaveGameData();
    
    void    SaveSceneActorsPositions();
    void    GetLevelUISelectPos(CIwGameString &data);
    void    GetLevelFixturePos(CIwGameString &data);
    void    GetLevelForegroundPos(CIwGameString &data);
    void    GetLevelParallaxPos(CIwGameString &data);
    void    GetLevelEnemyPos(CIwGameString &data);
    void    GetLevelObjectiveGoal(CIwGameString &data);
    void    GetLevelAestheticsBGPos(CIwGameString &data);
    void    GetLevelMapFragmentsPos(CIwGameString &data);
    void    GetLevelOrbPos(CIwGameString &data);
    void    GetLevelObstacles(CIwGameString &data);

    
    // collectible items
    void            AddNumOfMapFragments(int num);
    void            AddNumOfOrbs(int num);
    
    int             GetNumOfCollectedMaps() const;
    int             GetNumOfCollectedOrbs() const;
    
    void            LoadMap();
	void            DisplayStartLevel(CIwGameString string, bool reformat_str = true);
	unsigned long   GetStateID() const;
	void			AddState(State* state, bool change = true);
	void			RemoveState(State* state);
	void			ChangeState(unsigned long id);
	State*			GetCurrentState();
    unsigned int    GetLastState() const { return m_lastState; }
    
    // menu
	void            LoadPauseMenu();
    void            ShowDebugMenu();
    void            HideDebugMenu();
	void            CancelPause();
    
	void            ResetLevel();
	bool            LoadPlayer();
	bool            LoadPlayer(CIwFVec2 pos);
    int             GetWorld() const { return World; }
    int             GetLevel() const {return Level; }
	int             GetNextLevel() const;
    void            SetWorld(int world) {World = world;}
    void            SetLevel(int level) {Level = level;}
    void            SetMusic(CIwGameString music_str, bool restart = true);
    CIwGameString   GetCurrentMusic() const { return CurrentMusic; }
    void            PlayMusic();
	int             GetCurrentLevel() const { return Level; }
    void            LoadLevel();
	void            LoadLevel(int world, int lvl);
    void            LoadLevel(CIwGameString path, CIwGameString string);
	void            DestroyOldLevel();
	GameOptions*    GetUserOptions() const { return UserOptions; }
    void            SetUserOptions(GameOptions* option) {UserOptions = option;}
    void            SetHUDText(HUDTextDisplay* hud) { HUDText = hud; }
    HUDTextDisplay* GetHUDText() const {return HUDText;}
    void            ReloadHUDText();
    void            DisplayTextInHUD(CIwGameString string);
    void            SetCurrentLevelString(int world, int level)     //set reference to current level
                    {
                        CIwGameString string = "Level";
                        string += world;
                        string += "_";
                        string += level;
                        
                        CurrentLevelString = string;
                    }
    GameScene*      GetMainGameScene();
    
    void            SetCurrentLevelString(CIwGameString string) { CurrentLevelString = string;}
    CIwGameString   GetCurrentLevelString() { return CurrentLevelString; }
    
    //find the smallest and largest vertex in a polygon
    CIwFVec2        FindSmallestVertex(CIwGameGeomShapePolygon* polygon);
    CIwFVec2        FindLargestVertex(CIwGameGeomShapePolygon* polygon);
    
    void            LoadDebugHUD();
    void            UpdateDebuggingInfo();
    bool            CheckTouch();
    void            ClearTimelineFromParent(CIwGameAnimTimeline* timeline);
    void            LoadBoss(const char* bossName);
    
    void            SetStoryboard(const CIwGameString& story)
                    {
                        m_storyboard = story;
                        CIwGameError::LogError("Setting the storyboard: ", m_storyboard.c_str());
                    }
    CIwGameString   GetStoryboard() const
                    {
                        return m_storyboard;
                    }
    
    // utility
    ObjectData GetBox2dObjectData(int type) const;

    
};

class GameXomlAction : public IIwGameXomlAction
{
public:
	enum ActionType
	{
		Action_LoadLevel		=	0,
		Action_StartLevel		=	1,
        Action_LoadMap          =   2,
        Action_ChangeState      =   3,
        Action_LoadBoss         =   4,
        Action_LoadTestLevel    =   5,
		Action_Maximum			=	6,
	};

protected:
	ActionType	Type;
	

public:
	GameXomlAction(ActionType type)
	{
		Type = type;
		switch (Type )
		{
		case Action_LoadLevel:
			setActionName("loadlevel");
			break;
		case Action_StartLevel:
			setActionName("startlevel");
			break;
        case Action_LoadMap:
            setActionName("loadmap");
            break;
        case Action_ChangeState:
            setActionName("changestate");
            break;
        case Action_LoadBoss:
            setActionName("loadboss");
            break;
        case Action_LoadTestLevel:
            setActionName("loadtestlevel");
            break;
		}
	}
	void Execute(IIwGameXomlResource *source, CIwGameAction* action);
};

#endif
