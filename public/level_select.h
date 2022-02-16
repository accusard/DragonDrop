//--------------------------------------------------------------------------------------------------------
//  LevelSelect class - interface to handle level selections
//--------------------------------------------------------------------------------------------------------

#if !defined(_LEVEL_SELECT_H_)
#define _LEVEL_SELECT_H_

#include "game.h"
#include "IwGameUI.h"


enum e_levelStatus {LOCKED,PLAYABLE,};

class LevelSelect : public CIwGameUIBase
{
public:
    LevelSelect();
    
    /**
     * \name Touch.
     *
     * These functions handle the touch event of the actor
     */
    void    OnBeginTouch(int index, int x, int y, bool allow_bubble);
    void    OnEndTouch(int index, int x, int y, bool allow_bubble);
    
    bool	UpdateVisual();
    bool    LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    
    bool	LevelPlayable() const;
    
private:
    // properties
    bool                m_hasCompleted;     	/**< level has been completed by the player */
    bool                m_levelPlayable;        /**< level have been locked or unlocked */
	
};

class LevelSelectCreator : public IIwGameXomlClassCreator
{
public:
    LevelSelectCreator()
    {
        setClassName("levelselectactor");
    }
    IIwGameXomlResource* CreateInstance(IIwGameXomlResource* parent)
    {
        return new LevelSelect();
    }
};

#endif