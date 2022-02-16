//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// game_state.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#include "game_state.h"
#include "enemy_actor.h"

/********************************************************************************************************
 State base class implementations
 ********************************************************************************************************/

State::State(unsigned long id) : m_currentScene(NULL)
{
	m_id = id;
}

void State::Load()
{

}

void State::Close()
{

}

void State::Update()
{
    //GAME->CIwGame::Update();
}

void State::Render()
{

}

/********************************************************************************************************
 GameState base class implementations
 ********************************************************************************************************/

GameState::GameState()
{
    
}

GameState::~GameState()
{
    
}

/********************************************************************************************************
 Title_State implementations
 ********************************************************************************************************/

Title::Title() : State(STATE_TITLE)
{
    m_startGameButton = NULL;
}
void Title::Load()
{
    GAME->SetMusic("DDtheme.mp3", true);

    IW_GAME_XOML->Process(GAME, "Title.xml");
    IW_GAME_XOML->Process(GAME, "Logo.xml");
    IW_GAME_XOML->Process(GAME, "Credits.xml");
    
    CIwGameScene* title_scene = GAME->findScene("TitleScene");
    
    if(title_scene != NULL)
    {
        m_startGameButton = (ButtonIcon*)title_scene->findActor("StartButton");
    }
    
}
void Title::Close()
{
    CIwGameScene* title_scene = GAME->findScene("TitleScene");
    
    if(title_scene != NULL)
        GAME->removeScene(title_scene);
}
void Title::Update()
{
    State::Update();
    if(m_startGameButton->getStartEvent())
    {
        GAME->ChangeState(STATE_STORY);
    }
}

/********************************************************************************************************
 Debug_State implementations
 ********************************************************************************************************/

Debug::Debug() : State(STATE_DEBUG)
{
    
}
void Debug::Load()
{
    IW_GAME_XOML->Process(GAME, "TitleDebug.xml");
    CIwGameScene* scene = GAME->findScene("TitleScene");
    GAME->changeScene(scene);
    
    CIwGameAnimTimeline* timeline = scene->getTimelinesManager()->findTimeline("scene_fade");
    scene->setTimeline(timeline);
    timeline->restart();
}

/********************************************************************************************************
 Story_State implementations
 ********************************************************************************************************/

Story::Story() : State(STATE_STORY)
{
	m_currentStoryScene = NULL;
    m_nextStoryBrush    = NULL;
    m_skipButton        = NULL;
    m_gameScene         = NULL;
    m_numOfScenes       = 0;
}

void Story::Load()
{
    //load the name of the story
    CIwGameError::LogError("Loading the storyboard: ", GAME->GetStoryboard().c_str());
    m_sceneStrings.push_back(GAME->GetStoryboard());
    CIwGameString string = m_sceneStrings.front();
    string += ".xml";
    IW_GAME_XOML->Process(GAME, string.c_str());
    
    //get pointer to the scene name
    m_gameScene = GAME->findScene("StoryScene");
    
    //get pointer to the storyboard
    m_currentStoryScene = (Storyboard*)m_gameScene->findActor("Storyboard");
    
    m_numOfScenes = m_currentStoryScene->GetNumOfScenes();
    for(unsigned int i = 1; i <= m_numOfScenes; i++)
    {
        CIwGameString scene_num = CIwGameString(i).c_str();
        
        //get the name of the scene and add the suffix
        string = m_sceneStrings.front();
        string += scene_num;
        m_sceneStrings.push_back(string);
    }
    
    //get pointer to the skip button
    m_skipButton = (ButtonIcon*)m_gameScene->findActor("SkipButton");
    
    //load the first story image
    Render();
}

void Story::LoadNextStoryScene()
{}

void Story::Close()
{
    m_sceneStrings.clear();
    m_numOfScenes = 0;
    
    IW_GAME_AUDIO->StopMusic();
    
}

void Story::Update()
{
    State::Update();
    
    if(m_skipButton->getStartEvent())
    {
        GAME->ChangeState(STATE_PAUSE);
        CIwGameScene* scene = GAME->findScene("StoryScene");
        GAME->removeScene(scene);
        IW_GAME_XOML->Process(GAME, "WorldMap.xml");
        return;
    }
    
    if(m_currentStoryScene->HasTapped() && m_numOfScenes != 0)
    {
        //render the next scene to screen
        Render();
        m_currentStoryScene->SetTapped(false);
    }
}

void Story::Render()
{
    //remove the old scene
    m_numOfScenes--;
    CIwGameString old_img = m_sceneStrings.front();
    CIwGameImage* image = (CIwGameImage*)m_gameScene->getResourceManager()->findResource(old_img.c_str(), CIwGameXomlNames::Image_Hash);
    if(image != NULL)
        m_gameScene->getResourceManager()->removeResource(image);
    
    m_sceneStrings.erase(&m_sceneStrings.front());
    
    //get the next scene
    CIwGameString brush_name = m_sceneStrings.front();
    m_nextStoryBrush = (CIwGameBrushImage*)m_gameScene->getResourceManager()->findResource(brush_name.c_str(), CIwGameXomlNames::Brush_Hash, false);
    
    //load
    if(m_nextStoryBrush != NULL)
        m_currentStoryScene->SetFromBrush(m_nextStoryBrush);
}

Storyboard* Story::GetCurrentStoryScene()
{
    return m_currentStoryScene;
}

/********************************************************************************************************
 Play State implementations
 ********************************************************************************************************/

Playing::Playing() : State(STATE_PLAYING)
{
}

void Playing::Load()
{
    GAME->HideDebugMenu();
    
    m_currentScene = (GameScene*)GAME->findScene("MainScene");
    if(m_currentScene == NULL)
    {
        m_currentScene = (GameScene*)GAME->getCurrentScene();
        GAME->SetMusic("level1.mp3");
    }
    
    
}

void Playing::Close()
{
    m_currentScene = NULL;
}

void Playing::Update()
{
    
    if(m_currentScene->getNameHash() != CIwGameString("MainScene").getHash())
        return;
    
    if(m_currentScene->IsSpawnSafe() && m_currentScene != NULL)
    {
        CIwFVec2 pos = m_currentScene->GetSpawnPosition();
        int type = m_currentScene->GetDropActor()->getType();
        m_currentScene->DropObject(type, pos);
        m_currentScene->SetSpawnSafe(false);
    }
    
    State::Update();
}

void Playing::Render()
{
    
}

/********************************************************************************************************
 Pause State implementations
 ********************************************************************************************************/

Pause::Pause() : State(STATE_PAUSE)
{
}

void Pause::Load()
{
    GAME->ShowDebugMenu();
    
    m_currentScene = (GameScene*)GAME->findScene("MainScene");
    
    if(m_currentScene != NULL)
    {
        m_currentScene->setColour(150,150,150,255);
        m_currentScene->setConstantTimeStep(0.0001f);
    }
}

void Pause::Close()
{
    if(m_currentScene != NULL)
    {
        // lit foreground color
        m_currentScene->setColour(255,255,255,255);
        m_currentScene->setConstantTimeStep(m_currentScene->GetPrevTimeStep());
        
        m_currentScene = NULL;
    }
}

void Pause::Update()
{
    State::Update();
}

void Pause::Render()
{
    
}

