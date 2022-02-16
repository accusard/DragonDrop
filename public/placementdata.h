//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// placementdata.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  XomlActorPlacementData class interface
//--------------------------------------------------------------------------------------------------------

#if !defined(_PLACEMENTDATA_H_)
#define _PLACEMENTDATA_H_

#include "IwGameString.h"
#include "IwGeomFVec2.h"
#include "IwGameScene.h"

/**
 *  \brief This class formats data into a readable XOML string to be used by various functions found in Game::SaveSceneActorsPositions()
 */
class XomlActorPlacementData
{
public:
    
public:
    XomlActorPlacementData(const char* heading, const char* temp_name, const int type);
    
    void            FormatPlacementData(CIwGameString name, CIwFVec2 pos, bool line_end);
    void            FormatCustomDataNewLine(const CIwGameString string, const CIwGameString value);
	void            FormatCustomData(const CIwGameString string, float value, bool line_end = false);
    void            FormatCustomData(const CIwGameString string, CIwFVec2 value, bool line_end = false);
    void            FormatCustomData(const CIwGameString string, const int value, bool line_end = false);
    void            FormatCustomData(const CIwGameString string, const CIwGameString value, bool line_end = false);

    void            FormatEnd(CIwGameString &data);
    int             GetActorDataType() const;
    CIwGameScene*   GetScene() const;
    
private:
    void    FormatHeading(const char* h);
    void    FormatName(CIwGameString name);
    void    FormatPosition(const CIwFVec2 pos, bool line_end = false);
    void    StartTemplate();
    void    StartCustomData(const char* text);
    void    LineEnd(bool line_end);
    void    DataEnd();
    
private:
    CIwGameScene*   m_scene;
    CIwGameString   m_data;
    CIwGameString   m_templateName;
    int             m_actorType;

};

#endif
