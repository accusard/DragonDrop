//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// obstacles.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  Classes for objects and obstacles
//--------------------------------------------------------------------------------------------------------

#if !defined(_OBSTACLES_H_)
#define _OBSTACLES_H_

#include "IwGameActor.h"
#include "base_actor.h"
#include "sensor_actor.h"

//wind force
const float UP      = -1.5f;
const float DOWN    = 1.0f;
const float LEFT    = -1.0f;
const float RIGHT   = 1.0f;

//tool value
const float UMBRELLA_FALL_VALUE     = 2.0f;

/********************************************************************************************************
 base class implementations
 ********************************************************************************************************/
class Obstacle : public CIwGameActorImage
{
public:
    Obstacle() : CIwGameActorImage() {};
    virtual ~Obstacle() {};
    
private:
    
    
};

/********************************************************************************************************
 Wind obstacle class implementations
 ********************************************************************************************************/

class Wind : public SensorActor
{
public:
    Wind(): SensorActor(),m_speed(0), m_direction(UP)
    {
        setActualClassType("wind");
    };
    Wind(const Wind& that): m_speed(that.m_speed), m_direction(that.m_direction) {};
    Wind& operator=(const Wind& rhs)
    {
        m_speed = rhs.m_speed;
        m_direction = rhs.m_direction;
        return *this;
    }
    
    bool LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    bool Update(float dt);
    float GetWindSpeed() const { return m_speed; }
    float GetWindDirection() const { return m_direction; }
    //BaseActor* CheckOverlap();
    
private:
    float   m_speed;
    float   m_direction;
    
    
};

class WindCreator : public IIwGameXomlClassCreator
{
public:
    WindCreator()
    {
        setClassName("windobstacle");
    }
    IIwGameXomlResource* CreateInstance(IIwGameXomlResource* parent)
    {
        return new Wind();
    }
};

#endif
