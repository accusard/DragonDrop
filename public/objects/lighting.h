//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// lighting.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Interface to the lighting class
//--------------------------------------------------------------------------------------------------------

#ifndef __dragon_drop_m__lighting__
#define __dragon_drop_m__lighting__

#include "IwGxTypes.h"
#include "IwGx.h"

#include "IwGameSprite.h"
#include "Iwgamerender2d.h"
#include "IwGameResources.h"
#include "IwGameActor.h"

#include "scene.h"

class GameScene;

class Lighting : protected CIwGameActorImage
{
public:
    Lighting(CIwRect& boundingRect);
    ~Lighting();
    
    void SetBoundingSceneRect(CIwRect& rect);
    
    void Draw();
    
private:
    CIwRect     m_boundingSceneRect;        /**< the rectanglular area of the current scene's screen */
    CIwFVec2*   m_centerOfLight;
	CIwFVec2*	m_wallBegin;
	CIwFVec2*	m_wallEnd;
	
	GameScene*	m_gameScene;
};
#endif /* defined(__dragon_drop_m__lighting__) */
