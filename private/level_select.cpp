//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// level_select.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#include "level_select.h"
#include "options.h"

//--------------------------------------------------------------------------------------------------------
//  Implementation of LevelSelect class
//--------------------------------------------------------------------------------------------------------

LevelSelect::LevelSelect() : CIwGameUIBase(), m_hasCompleted(false), m_levelPlayable(LOCKED)
{
    setType(UI_LEVEL_SELECT);
}

void LevelSelect::OnBeginTouch(int index, int x, int y, bool allow_bubble)
{
    CIwGameUIBase::OnBeginTouch(index, x, y, allow_bubble);
    
    //CIwGameString string = getName();
    //GAME->GetHUDText()->SetTextDisplay(string.c_str());
}

void LevelSelect::OnEndTouch(int index, int x, int y, bool allow_bubble)
{
    CIwGameUIBase::OnEndTouch(index, x, y, allow_bubble);
    
    if(GAME->GetUserOptions()->IsDebugging())
        return;
    
    CIwGameString string = getName();
    
    CIwGameString level = string.getSubString(7);
    GAME->SetLevel(level.GetAsInt());
    
    CIwGameString world = string.getSubString(5);
    CIwGameString sub   = world.getSubString(1);

    world.Replace(sub.c_str(), "");

    GAME->SetWorld(world.GetAsInt());
    
    GAME->LoadLevel();
    
    //GAME->removeScene(getScene());
    //GAME->LoadLevel(getName());
    //GAME->changeScene(GAME->findScene("MainScene"));
}

bool LevelSelect::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
	if(!CIwGameUIBase::LoadFromXoml(parent,load_children,node))
		return false;
    
    bool playable = false;
    
	for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); ++it)
	{
		unsigned int name_hash = (*it)->getName().getHash();
        
		if(name_hash == CIwGameString("playable").getHash())
        {
            playable = (*it)->GetValueAsBool();
            
            if(playable)
            {
                setTappable(true);
                m_levelPlayable = playable;
            }
            else
            {
                setTappable(false);
                getVisual()->setAlphaMode(CIwMaterial::ALPHA_HALF);
            }
        }
    }
    
    setDraggable(true);
    
    return true;
}

bool LevelSelect::UpdateVisual()
{
	return CIwGameUIBase::UpdateVisual();
}

bool LevelSelect::LevelPlayable() const
{
	return m_levelPlayable;
}

