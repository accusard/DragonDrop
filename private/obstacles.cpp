//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// obstacles.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Classes for objects and obstacles
//--------------------------------------------------------------------------------------------------------

#include "obstacles.h"

bool Wind::Update(float dt)
{
    if(!SensorActor::Update(dt))
       return false;
    
    BaseActor* actor = CheckForContact();
    
    if(actor != NULL)// && actor->HasItem(UMBRELLA))
    {
        float windVelocity = m_speed * m_direction;
        actor->SetHoverValue(windVelocity);
        //actor->ChangeState(ACTOR_HOVER);
        actor->setVelocityY(windVelocity);
    }
    
    return true;
}

bool Wind::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
    if(!SensorActor::LoadFromXoml(parent, load_children, node))
        return false;
    
    for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
    {
        unsigned int name_hash = (*it)->getName().getHash();
        
        if(name_hash == CIwGameString("speed").getHash())
        {
            m_speed = (*it)->GetValueAsFloat();
        }
        else
        if(name_hash == CIwGameString("direction").getHash())
        {
            CIwGameString dir = (*it)->GetValue().getHash();
            
            if(dir == CIwGameString("up").getHash())
                m_direction = UP;
            else
            if(dir == CIwGameString("down").getHash())
                m_direction = DOWN;
            else
            if(dir == CIwGameString("left").getHash())
                m_direction = LEFT;
            else
            if(dir == CIwGameString("right").getHash())
                m_direction = RIGHT;
            
            //m_direction = (*it)->GetValueAsInt();
        }
    }
    
    return true;
}
