//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// lighting.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Implementation of the lighting class
//--------------------------------------------------------------------------------------------------------

#include "lighting.h"


Lighting::Lighting(CIwRect& rect) : CIwGameActorImage(), m_boundingSceneRect(rect), m_centerOfLight(NULL), m_wallBegin(NULL), m_wallEnd(NULL), m_gameScene(NULL)
{
    
}

Lighting::~Lighting(){
    
}

void Lighting::SetBoundingSceneRect(CIwRect& rect)
{
    m_boundingSceneRect = rect;
}

void Lighting::Draw()
{
    // Clear the screen
    IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
    // Allocate a material from the IwGx global cache
    CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
    // Set this as the active material
    IwGxSetMaterial(pMat);
    // Set up screenspace vertex coords
    int16 x1 = (int16)IwGxGetScreenWidth() * 1 / 4;
    int16 x2 = (int16)IwGxGetScreenWidth() * 2 / 4;
    int16 x3 = (int16)IwGxGetScreenWidth() * 3 / 4;
    int16 y1 = (int16)IwGxGetScreenHeight() * 1 / 4;
    int16 y3 = (int16)IwGxGetScreenHeight() * 3 / 4;
    static CIwSVec2 xy3[3];
    xy3[0].x = x2, xy3[0].y = y1;
    xy3[1].x = x1, xy3[1].y = y3;
    xy3[2].x = x3, xy3[2].y = y3;
    IwGxSetVertStreamScreenSpace(xy3, 3);
    // Set up vertex colours
    static CIwColour cols[4] =
    {
        {0xff, 0x00, 0x00},
        {0x00, 0xff, 0x00},
        {0x00, 0x00, 0xff},
    };
    IwGxSetColStream(cols, 3);
    // Draw single triangle
    IwGxDrawPrims(IW_GX_TRI_LIST, NULL, 3);
    // End drawing
    IwGxFlush();
    // Swap buffers
    IwGxSwapBuffers();
    
    CIwGameError::LogError("DRAW");
}
