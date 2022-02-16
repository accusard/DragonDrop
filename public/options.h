//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// options.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Options class to handle various game settings
//--------------------------------------------------------------------------------------------------------

#if !defined(_OPTIONS_H_)
#define _OPTIONS_H_

//#include "game.h"
#include "IwGeomFVec2.h"
#include "IwGameXoml.h"

const bool      DEBUGGING           = false;    // display debugging information in HUD
const int       START_WORLD         = 1;
const int       START_AT_LEVEL      = 1;
const float     ZOOM_LEVEL          = 1.0f;		// 1.5f or 1.0f
const bool      TOUCH_MOVE          = false;
const bool      MUSIC_ON            = true;
const bool      SOUND_ON            = true;

const bool      HAS_UMBRELLA        = false;
const CIwFVec2  PLAYER_START_POS    = CIwFVec2(0,0);
const bool      CUSTOM_POSITION     = false;
const bool      SHOW_FIXTURE        = false;
const bool      BOUND_CAMERA        = true;
const bool      SHOW_PLAYER_SENSOR  = false;

class GameOptions : public IIwGameXomlResource
{
private:
    bool        Debugging;
    bool        TouchMove;
    bool        MusicOn;
    bool        SoundOn;
    float       MusicVol;
    float       SoundVol;
    bool        m_hasUmbrella;
    CIwFVec2    m_playerPos;
    bool        m_customPos;
    CIwFVec2    m_StartAt;
    float       m_world;
    float       m_level;
    bool        m_showFixture;
    bool        m_boundCamera;
    bool        m_showPSensor;
    
public:
    GameOptions() : Debugging(DEBUGGING), TouchMove(TOUCH_MOVE), MusicOn(MUSIC_ON), SoundOn(SOUND_ON), MusicVol(0.0f), SoundVol(1.0f), m_hasUmbrella(HAS_UMBRELLA), m_playerPos(PLAYER_START_POS), m_customPos(CUSTOM_POSITION), m_StartAt(0,0),m_world(START_WORLD), m_level(START_AT_LEVEL), m_showFixture(SHOW_FIXTURE),m_boundCamera(BOUND_CAMERA), m_showPSensor(SHOW_PLAYER_SENSOR) {}
    
    bool    LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    void	Init();
    void    SetMusicOn(bool on = true) { MusicOn = on; }
    void    SetSoundOn(bool on = true) { SoundOn = on; }
    void    SetDebugging(bool on = true)    {Debugging = on;}
    
    bool        IsMusicOn() const { return MusicOn; }
    bool        IsSoundOn() const { return SoundOn; }
    float       GetMusicVol() const { return MusicVol; }
    float       GetSoundVol() const { return SoundVol; }
    bool        GetTouchMoveOption() const { return TouchMove; }
    bool        IsDebugging() const {return Debugging; }
    bool        HasUmbrella() const {return m_hasUmbrella; }
    CIwFVec2    GetPlayerPos() const {return m_playerPos;}
    void        SetPlayerPos(const CIwFVec2 pos) {m_playerPos = pos;}
    bool        HasCustomPos() const {return m_customPos;}
    float       GetWorld() const {return m_world;}
    float       GetLevel() const {return m_level;}
    CIwFVec2    StartAt() {return m_StartAt;}
    bool        ShowFixture() const {return m_showFixture;}
    bool        BoundCam() const {return m_boundCamera;}
    bool        ShowPSensor() const {return m_showPSensor;}
};

class GameOptionsCreator : public IIwGameXomlClassCreator
{

public:
    GameOptionsCreator()
    {
        setClassName("gameoptions");
    }
    
    IIwGameXomlResource* CreateInstance(IIwGameXomlResource* parent)
    {
        return new GameOptions();
    }
};

#endif
