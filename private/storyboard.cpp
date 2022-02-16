//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK Copyright 2018-2022
//	Powered by Pocketeers Limited's IwGame Engine
//
// storyboard.cpp
//	Programmed by: Vanny Sou
//
//--------------------------------------------------------------------------------------------------------

#include "storyboard.h"

Storyboard::Storyboard(const char* scenes_to_load, unsigned int num_of_scenes)
{
    m_numOfScenes   = num_of_scenes;
}

bool Storyboard::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
    if(!CIwGameActorImage::LoadFromXoml(parent, load_children, node))
        return false;
    
    for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
    {
        unsigned int name_hash = (*it)->getName().getHash();
        
        if(name_hash == CIwGameString::CalculateHash("numofscenes"))
        {
            m_numOfScenes = (*it)->GetValueAsInt();
        }
    }
    
    return true;
}

unsigned int Storyboard::GetNumOfScenes() const
{
    return m_numOfScenes;
}

void Storyboard::OnEndTouch(int index, int x, int y, bool allow_bubble)
{
    m_hasTapped = true;
}

void StoryboardSceneManager::Init()
{
    
}

void StoryboardSceneManager::AddStoryScene(CIwGameActor *storyscene)
{
    StoryScenes.push_back(storyscene);
}
void StoryboardSceneManager::RemoveStoryScene(CIwGameActor *storyscene)
{
    
}
