//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// placementdata.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  XomlActorPlacementData class implementation
//--------------------------------------------------------------------------------------------------------

#include "placementdata.h"
#include "IwGameActor.h"
#include "game.h"

/**
 * \brief The constructor; should be called before the iterator loop.
 * \param heading The name of the heading of this actor's placement.
 * \param temp_name The name of the template to call in xoml to instantiate the actor.
 * \param type The type of the actor to itertate through
 */
XomlActorPlacementData::XomlActorPlacementData(const char* heading, const char* temp_name, const int type)
: m_templateName(temp_name), m_actorType(type)
{
    m_scene = GAME->getCurrentScene();
    FormatHeading(heading);
}

/**
 * \brief This function places the appropriate formatting to end the actor's placement data and add it's 
 * collective data to the passed argument. The function is called outside the iterator loop.
 * \param data A reference of the data to be added to.
 */
void XomlActorPlacementData::FormatEnd(CIwGameString &data)
{
    DataEnd();
    
    data += m_data;
}

/**
 * \brief Use the argument to format a heading for this XomlActorPlacementData object
 * \param h The heading of the data to be formatted
 */
void XomlActorPlacementData::FormatHeading(const char* h)
{
    m_data += "<!-- [-";
    m_data += CIwGameString(h);
    m_data += "-] -->\r";
}

/**
 * \brief Formats the name and position of the actor's placement.
 * \param name The name of the actor.
 * \param pos The position of the actor.
 * \param line_end If true adds a line break after outputting the pos string.
 */
void XomlActorPlacementData::FormatPlacementData(CIwGameString name, CIwFVec2 pos, bool line_end)
{
    StartTemplate();
    FormatName(name);
    FormatPosition(pos, line_end);
}

/**
 * \brief 
 */
void XomlActorPlacementData::FormatName(CIwGameString name)
{    
    CIwGameString formatted_data;
    
    formatted_data += "name=\"";
    formatted_data += name;
    formatted_data += "\"";
    
    m_data += formatted_data;
    LineEnd(false);
}

void XomlActorPlacementData::FormatPosition(const CIwFVec2 pos, bool line_end)
{
    CIwGameString formatted_data;
    
    formatted_data += "pos=\"";
    formatted_data += pos.x;
    formatted_data += ", ";
    formatted_data += pos.y;
    formatted_data += "\"";
    
    m_data += formatted_data;
    LineEnd(line_end);
}

void XomlActorPlacementData::FormatCustomDataNewLine(const CIwGameString string, const CIwGameString value)
{
    StartTemplate();
    FormatCustomData(string,value);
}

void XomlActorPlacementData::FormatCustomData(const CIwGameString string, float value, bool line_end)
{
	CIwGameString v = CIwGameString(value);
    
    FormatCustomData(string, v, line_end);
}

void XomlActorPlacementData::FormatCustomData(const CIwGameString string, CIwFVec2 value, bool line_end)
{
    CIwGameString v;
    
    v += value.x;
    v += ", ";
    v += value.y;
    
    FormatCustomData(string, v, line_end);
}

void XomlActorPlacementData::FormatCustomData(const CIwGameString string, const int value, bool line_end)
{
    CIwGameString v = CIwGameString(value);
    
    FormatCustomData(string, v, line_end);
}

void XomlActorPlacementData::FormatCustomData(const CIwGameString string, const CIwGameString value, bool line_end)
{
    CIwGameString formatted_data;
    
    formatted_data += string;
    formatted_data += "=\"";
    formatted_data += value;
    formatted_data += "\"";
        
    m_data += formatted_data;
    LineEnd(line_end);

}
void XomlActorPlacementData::StartTemplate()
{
    m_data += "<FromTemplate Template=\"";
    m_data += m_templateName;
    m_data += "_template\" ";
}

void XomlActorPlacementData::StartCustomData(const char* text)
{
	m_data += "<";
	m_data += text;
	m_data += " ";
}

/**
 *  \brief If line_end is not true, make a space. Otherwise end the line between each data
 */
void XomlActorPlacementData::LineEnd(bool line_end)
{
    if(!line_end)
        m_data += " ";
    else
        m_data += " />\r";
}

/**
 *  \brief Adds paragraph spacing between XomlActorPlacementData
 *  \return Returns the string '\r\r'
 */
void XomlActorPlacementData::DataEnd()
{
    m_data += "\r\r";
}

int XomlActorPlacementData::GetActorDataType() const
{
    return m_actorType;
}

CIwGameScene* XomlActorPlacementData::GetScene() const
{
    return m_scene;
}
