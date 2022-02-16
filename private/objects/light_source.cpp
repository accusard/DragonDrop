//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// light_source.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#include "light_source.h"

/**
 * \brief Creates a source of light.
 * \param pos Position of the light source.
 * \param col The color of the light.
 */
LightSource::LightSource(CIwFVec2 pos, CIwColour col) : Position(pos), Color(col), m_hasRendered(false)
{
    
}

/**
 * \brief Destructor
 */
LightSource::~LightSource()
{
    
}

/**
 * \brief Sets the position of the light source
 */
void LightSource::SetPosition(CIwFVec2 pos)
{
    Position = pos;
}

/**
 * \brief Sets the color of the light source
 */
void LightSource::SetColor(CIwColour col)
{
    Color = col;
}

/**
 * \brief Render
 */
void LightSource::Render()
{
    if(!m_hasRendered)
    {
        GAME->DisplayTextInHUD("Rendering..");
        m_hasRendered = true;
    }
}
