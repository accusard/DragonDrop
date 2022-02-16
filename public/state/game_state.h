//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// game_state.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Classes for various game states
//--------------------------------------------------------------------------------------------------------

#if !defined(_STATE_H_)
#define _STATE_H_

#include "game.h"
#include "storyboard.h"
#include "toolbar.h"
#include "light_source.h"

// State ID
const unsigned long STATE_STORY     = 1;
const unsigned long STATE_TITLE     = 2;
const unsigned long STATE_DEBUG     = 3;
const unsigned long STATE_PLAYING   = 4;
const unsigned long STATE_PAUSE     = 5;
const unsigned long STATE_MAP       = 6;

//Storyboard strings
const CIwGameString INTRO_STR       = "Intro";
const CIwGameString WORLD2_STR      = "sky";
const CIwGameString WORLD3_STR      = "volc";
const CIwGameString END_STR         = "end";

class GameScene;
class ButtonIcon;
/********************************************************************************************************
 State base interface
 ********************************************************************************************************/

class State
{
public:
	State(unsigned long id = 0);
    virtual ~State() {}

	virtual void Load();
	virtual void Close();
	
	virtual void Update();
	virtual void Render();
	
	unsigned long GetID() { return m_id; }
    void SetID(unsigned long id) { m_id = id; }

private:
	unsigned long m_id;
    
protected:
    GameScene* m_currentScene;

};

/********************************************************************************************************
 Game state interface
 ********************************************************************************************************/

class GameState : public State
{
public:
    GameState();
    virtual ~GameState();
};

/********************************************************************************************************
 Title state interface
 ********************************************************************************************************/

class Title : public State
{
public:
    Title();
    
    void Load();
    void Close();
    void Update();
    
private:
    ButtonIcon*       m_startGameButton;
    
};

/********************************************************************************************************
 Debug state interface
 ********************************************************************************************************/

class Debug : public State
{
public:
    Debug();
    void Load();
    
};

/********************************************************************************************************
 Story state interface
 ********************************************************************************************************/

class Story : public State
{
public:
    Story();
    
    void Load();
    void Close();
    void Update();
    void Render();
    
    Storyboard* GetCurrentStoryScene();
    void        LoadNextStoryScene();
    void        SetNumOfScenes(unsigned int num);
    
private:
    CIwList<CIwGameString>  m_sceneStrings;
    CIwGameBrushImage*      m_nextStoryBrush;
    Storyboard*             m_currentStoryScene;
    ButtonIcon*             m_skipButton;
    CIwGameScene*           m_gameScene;
    unsigned int            m_numOfScenes;
};

/********************************************************************************************************
 Playing state interface
 ********************************************************************************************************/

class Playing : public State
{
public:
    Playing();
    
    void Load();
    void Close();
    void Update();
    void Render();
    
private:
    //GameScene*   m_currentScene;
};

/********************************************************************************************************
 Pause state interface
 ********************************************************************************************************/

class Pause : public State
{
public:
    Pause();
    
    void Load();
    void Close();
    void Update();
    void Render();
    
private:
    //GameScene* m_currentScene;
};

#endif
