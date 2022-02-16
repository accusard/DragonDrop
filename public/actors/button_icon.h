//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// button_icon.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  button icon
//--------------------------------------------------------------------------------------------------------

#if !defined(_BUTTON_ICON_H_)
#define _BUTTON_ICON_H_

#include "IwGame.h"
#include "IwGameUI.h"

#include "game.h"
#include "base_actor.h"
#include "scene.h"
#include "hitbox_extension.h"

class HUDTextDisplay;



//--------------------------------------------------------------------------------------------------------
//  ButtonIcon class - scene specific buttons
//--------------------------------------------------------------------------------------------------------

class ButtonIcon : public CIwGameUIBase, HitBoxExtension
{
    using CIwGameUIBase::Init;
    
private:
    
    CIwGameDummySprite*     HitBox;
    bool                    StartEvent;        // is true after an OnEndTouch event
    
public:
	ButtonIcon();
    void            Init(unsigned int name_hash);
    
	void            LoadActionBar(int type);
    CIwGameActor*   HitTest(float x, float y);
    bool            getStartEvent() { return StartEvent; }
    
    void            SetType(unsigned int type);
    
    bool            Update(float dt);
	bool            LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
	void            OnTapped(int index, int x, int y, bool allow_bubble);
	void            OnBeginTouch(int index, int x, int y, bool allow_bubble);
	void            OnEndTouch(int index, int x, int y, bool allow_bubble);
};

class ButtonIconCreator : public IIwGameXomlClassCreator
{
public:
	ButtonIconCreator()
	{
		setClassName("buttonicon");
	}

	IIwGameXomlResource*	CreateInstance(IIwGameXomlResource* parent)
	{
		return new ButtonIcon();
	}
};

#endif
