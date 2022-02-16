//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// StoryboardSceneManager.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------

#if !defined(_STORYBOARD_H_)
#define _STORYBOARD_H_

#include "IwGameActor.h"

class Storyboard : public CIwGameActorImage
{
public:
    Storyboard() : m_numOfScenes(0),m_hasTapped(false) {}
    Storyboard(const char* scenes_to_load, unsigned int num_of_scenes);
    
    unsigned int    GetNumOfScenes() const;
    
    void OnEndTouch(int index, int x, int y, bool allow_bubble);
    bool LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    bool HasTapped() const {return m_hasTapped;}
    void SetTapped(bool tapped =true) { m_hasTapped = tapped; }
    
private:
    unsigned int    m_numOfScenes;
    bool            m_hasTapped;
};

class StoryboardCreator : public IIwGameXomlClassCreator
{
public:
    StoryboardCreator()
    {
        setClassName("storyboard");
    }
    IIwGameXomlResource* CreateInstance(IIwGameXomlResource* parent)
    {
        return new Storyboard();
    }
};

class StoryboardSceneManager
{
public:
    void    Init();
    void    AddStoryScene(CIwGameActor *storyscene);
    void    RemoveStoryScene(CIwGameActor *storyscene);
    
protected:
    CIwList<CIwGameActor*> StoryScenes;
    
};

#endif
