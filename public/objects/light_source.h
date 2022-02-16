//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// light_source.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#if !defined (__dragon_drop_m__light_source__)
#define __dragon_drop_m__light_source__

#include "IwGeomFVec2.h"
#include "IwColour.h"

#include "game.h"

/**
 * \brief Represents a light source
 *
 * This class stores the position and color of the light source.
 */
class LightSource
{
public:
    LightSource(CIwFVec2 pos, CIwColour col);
    ~LightSource();
    
    void        SetPosition(CIwFVec2 pos);
    void        SetColor(CIwColour);
    
    void        Render();
    
private:
    CIwFVec2    Position;       /**< the position of the light source in the scene */
    CIwColour   Color;          /**< the color of the light source */
    
    bool        m_hasRendered;   /**< used for debugging */
    
};

#endif /* defined(__dragon_drop_m__light_source__) */
