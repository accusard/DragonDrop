//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// toolbar.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Toolbox interface to the game
//--------------------------------------------------------------------------------------------------------

#if !defined(_BAR_ACTOR_H_)
#define _BAR_ACTOR_H_

#include "IwGame.h"
#include "IwGameUI.h"

#include "game.h"
#include "base_actor.h"
#include "scene.h"
#include "hitbox_extension.h"

class HUDTextDisplay;

//--------------------------------------------------------------------------------------------------------
//  Class ToolBarBox - used for its positions in the game's HUD toolbox and to align the available tool
//  objects for the current scene
//--------------------------------------------------------------------------------------------------------

class ToolBarBox : public CIwGameUILabel, HitBoxExtension
{
private:
	bool		Occupied;

public:
	ToolBarBox() : CIwGameUILabel(), Occupied(false) {}
	bool		isOccupied() { return Occupied;}
	void		setOccupied(bool set) { Occupied = set; }
};

class BarActorCreator : public IIwGameXomlClassCreator
{
public:
	BarActorCreator()
	{
		setClassName("toolbarbox");
	}

	IIwGameXomlResource* CreateInstance(IIwGameXomlResource *parent)
	{
		return new ToolBarBox();
	}
};

//--------------------------------------------------------------------------------------------------------
//  Class BarIcon - interface icons for the draggable objects
//--------------------------------------------------------------------------------------------------------

class BarIcon : public CIwGameUILabel, HitBoxExtension
{
private:
	int                     Quantity;
	int                     OrigQuantity;
	bool                    Remove;
    CIwGameDummySprite*     HitBox;
    
public:
	BarIcon() : CIwGameUILabel(), Quantity(0), OrigQuantity(0), Remove(false), HitBox(NULL)
    {
        HoldFocus = true;
        setActualClassType("baricon");
    }
    ~BarIcon(){}
    
	bool            LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
	bool            Update(float dt);
    
	void            ResetQuantity();
	void            SetRemove(bool remove = true) { Remove = remove; }
	bool            IsRemoved() const { return Remove; }
    void            OnBeginTouch(int, int, int, bool);
    void            OnEndTouch(int,int,int,bool);
    CIwGameActor*   HitTest(float x, float y);
    
    void            ActivateColorSensor(); 

};

class BarIconCreator : public IIwGameXomlClassCreator
{
public:
	BarIconCreator()
	{
		setClassName("baricon");
	}

	IIwGameXomlResource*	CreateInstance(IIwGameXomlResource* parent)
	{
		return new BarIcon();
	}
};

#endif
