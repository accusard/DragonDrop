//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// options.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// Game Object class that players can interact with
//--------------------------------------------------------------------------------------------------------

#include "IwGame.h"
#include "options.h"

void GameOptions::Init()
{
    m_StartAt.x = START_WORLD;
    m_StartAt.y = START_AT_LEVEL;

}


bool GameOptions::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{

    GameOptions* option = (GameOptions*)parent;
    
    for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
    {
        unsigned int attrib_name_hash = (*it)->getName().getHash();
        
        if(attrib_name_hash == CIwGameString("debugging").getHash())
        {
            bool debug = (*it)->GetValueAsBool();
            
            option->SetDebugging(debug);
            
            //only load gameoptions when debugging is set to true
            if(!debug)
                return false;
        }
        else
        if(attrib_name_hash == CIwGameString("touchmove").getHash())
        {
            option->TouchMove = (*it)->GetValueAsBool();
        }
        else
        if(attrib_name_hash == CIwGameString("musicon").getHash())
        {
            option->MusicOn = (*it)->GetValueAsBool();
        }
        else
        if(attrib_name_hash == CIwGameString("soundon").getHash())
        {
            option->SoundOn = (*it)->GetValueAsBool();
        }
        else
        if(attrib_name_hash == CIwGameString("musicvol").getHash())
        {
            option->MusicVol = (*it)->GetValueAsFloat();
        }
        else
        if(attrib_name_hash == CIwGameString("soundvol").getHash())
        {
            option->SoundVol = (*it)->GetValueAsFloat();
        }
        else
        if(attrib_name_hash == CIwGameString("hasumbrella").getHash())
        {
            option->m_hasUmbrella = (*it)->GetValueAsBool();
            //BaseActor* actor = GAME->findScene(
        }
        else
        if(attrib_name_hash == CIwGameString("playerpos").getHash())
        {
            (*it)->GetValueAsPoint(option->m_playerPos);
        }
        else
        if(attrib_name_hash == CIwGameString("startworld").getHash())
        {
            
            //CIwFVec2 startworld(0,0);
            (*it)->GetValueAsPoint(option->m_StartAt);
            
            //GAME->SetWorld(startworld.x);
            //GAME->SetLevel(startworld.y);
        }
        else
        if(attrib_name_hash == CIwGameString("custompos").getHash())
        {
            option->m_customPos = (*it)->GetValueAsBool();
        }
        else
        if(attrib_name_hash == CIwGameString("showfixture").getHash())
        {
            option->m_showFixture = (*it)->GetValueAsBool();
        }
        else
        if(attrib_name_hash == CIwGameString("boundcamera").getHash())
        {
            option->m_boundCamera = (*it)->GetValueAsBool();
        }
        else
        if(attrib_name_hash == CIwGameString("showpsensor").getHash())
            option->m_showPSensor = (*it)->GetValueAsBool();
    }
    
    return IW_GAME_GLOBAL_RESOURCES->getResourceManager()->addResource(this);
}
