//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// sensor_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// Declaration of base actor class to all other actors with movements
//--------------------------------------------------------------------------------------------------------

#if !defined(_SENSOR_ACTOR_H_)
#define _SENSOR_ACTOR_H_

//#include "game.h"
//#include "utility_actor.h"
#include "utility_actor.h"

class BaseActor;

//--------------------------------------------------------------------------------------------------------
// SensorActor class to be used solely as a sensor to other actors around it
//--------------------------------------------------------------------------------------------------------

class SensorActor : public CollidableActor
{
    using CIwGameActor::NotifyCollision;
    using CIwGameActorImage::Init;
    
private:
    bool        Contacted;

    BaseActor*  AttachedTo;
    int         m_dmgType;
    
public:
    SensorActor();
    virtual ~SensorActor() {}
    
    void    Init(BaseActor* actor);
    void    Init(BaseActor* actor, CIwGameBrushImage* image);
    bool    Update(float dt);
    void    SetContacted(bool contacted = true) { Contacted = contacted; }
    bool    HasContacted() const { return Contacted; }
    
    bool    LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    bool    PostLoadFromXoml(IIwGameXomlResource* parent, CIwGameXmlNode* node);
    
    // collisions
    void    ResolveCollisions();                           //checks for contacts
    void    NotifyCollision(SensorActor* other);
    
    void    UpdatePosition();
    
    virtual void NotifyLinkedActorMadeContact();
    virtual void NotifyLinkedActorLostContact();
    BaseActor* CheckForContact();
    
    void        SetAttachedTo(BaseActor* actor) {AttachedTo = actor;}
    BaseActor*  GetAttachedTo()                 {return AttachedTo; }
    int         GetDmgType() const              {return m_dmgType;}
    void        SetDmgType(int type)            {m_dmgType = type;}
};

class SensorActorCreator : public IIwGameXomlClassCreator
{
public:
    SensorActorCreator() {setClassName("sensoractor");}
    IIwGameXomlResource*    CreateInstance(IIwGameXomlResource* parent)
    {
        return new SensorActor();
    }
};

//--------------------------------------------------------------------------------------------------------
// SensorColorActor class that change the color of it's linkedTo actor when it makes contact
//--------------------------------------------------------------------------------------------------------

class SensorColorActor : public SensorActor
{
private:
    CIwGameAnimTimeline* BlinkingTimeline;
    
public:
    SensorColorActor() : SensorActor(), BlinkingTimeline(NULL) {}
    ~SensorColorActor()
    {
        if(BlinkingTimeline != NULL && !BlinkingTimeline->isManaged())
            SAFE_DELETE(BlinkingTimeline);
    }
    
    void Init(BaseActor* actor, CIwGameAnimTimeline* timeline);
    bool Update(float dt);
    void ResolveCollisions();
    //void NotifyLinkedActor(int notifyType);
    void NotifyLinkedActorMadeContact();
    void NotifyLinkedActorLostContact();
};

//--------------------------------------------------------------------------------------------------------
// SensorPropertyActor class that change the properties of it's linkedTo actor when it makes contact
//--------------------------------------------------------------------------------------------------------

class SensorPropertyActor : public SensorActor
{
private:
    
public:
    SensorPropertyActor() : SensorActor()
    {
        setWrapPosition(true);
       
    }
    
    ~SensorPropertyActor()
    {
        SAFE_DELETE(Box2dBody);
    }
    
    bool Update(float dt);
    bool LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    void ResolveCollisions();
    void NotifyCollision(CIwGameActor* other);
    void NotifyCollisionWithTrampoline(CollidableActor* trampoline_actor);
};

class SensorActorPropertyCreator : public IIwGameXomlClassCreator
{
public:
    SensorActorPropertyCreator()
    {
        setClassName("sensorpropertyactor");
    }
    
    IIwGameXomlResource* CreateInstance(IIwGameXomlResource* parent)
    {
        return new SensorPropertyActor();
    }
};

//--------------------------------------------------------------------------------------------------------
// SensorWeaknessActor class will react to anything that it is weak against
//--------------------------------------------------------------------------------------------------------

class SensorWeaknessActor : public SensorActor
{
private:
    int     m_weakTo;
    
public:
    SensorWeaknessActor() : SensorActor() {}
    
    void ResolveCollisions();
    void NotifyCollision(CIwGameActor* other);
    bool LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
};

class SensorWeaknessActorCreator : public IIwGameXomlClassCreator
{
public:
    SensorWeaknessActorCreator()
    {
        setClassName("sensorweaknessactor");
    }
    
    IIwGameXomlResource* CreateInstance(IIwGameXomlResource* parent)
    {
        return new SensorWeaknessActor();
    }
};

#endif
