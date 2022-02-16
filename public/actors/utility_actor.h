//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK 
//	Powered by Pocketeers Limited's IwGame Engine
//
// utility_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// CollidableActor class
//--------------------------------------------------------------------------------------------------------

#if !defined(_UTILITY_ACTOR_)
#define _UTILITY_ACTOR_

#include "IwGame.h"
#include "IwGameActorParticles.h"

#include "objectdata.h"
#include "observer.h"
#include "game_timer.h"

class GameObjectActor;
class PowerUpActor;

//--------------------------------------------------------------------------------------------------------
// ActorOrientation class interface
//--------------------------------------------------------------------------------------------------------

class ActorOrientation
{
public:
    enum eAxis
    {
        X_AXIS,
        Y_AXIS
    };
    
    enum eOrientation{
        ABOVE,
        BELOW,
        RIGHT,
        LEFT,
        UNDETERMINED
    };
    
    ActorOrientation(CIwGameActor* actor);
    
    int		FindOrientation(CIwGameActor* other_actor, ActorOrientation::eAxis axis);
    int     FindOrientationFromX(const float x);
    int     FindOrientationFromY(const float y);
    bool    IsWithinActorSize(CIwGameActor* other_actor) const;
    
private:
    CIwGameActor*   m_currentActor;
    eOrientation    m_orientation;
    CIwVec2         m_length;
    
};

//--------------------------------------------------------------------------------------------------------
// BurnableActor class interface
//--------------------------------------------------------------------------------------------------------

class BurnableActor
{
public:
    BurnableActor();
    
    void    Init(int max_burn_level = 1);
    int     GetBurnLevel() const;
    void    ApplyBurn();
    bool    CanCrumble();
    
    
private:
    int     m_burnLevel;
    
};

//--------------------------------------------------------------------------------------------------------
// Collidable Particles class interface
//--------------------------------------------------------------------------------------------------------

class CollidableParticles : public CIwGameActorParticles
{
public:
    
    CollidableParticles();
    void ResolveCollisions();
    bool UpdateVisual();
    
private:
    
};

//--------------------------------------------------------------------------------------------------------
// CollidableActor class interface
//--------------------------------------------------------------------------------------------------------

/**
 * \brief A class that utilizes Box2d physics or sprite overlaps to determine collision with others in the 
 *        scene.
 */
class CollidableActor : public CIwGameActorImage
{
public:
    CollidableActor();
    virtual ~CollidableActor() {}
    
    bool Update(float dt);
    bool UpdateVisual();
    bool LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    
    /**
     * \name Collisions
     *
     */
    void            ResolveCollisions();    // every class that is derived from Collidable actor need
                                            // to define their own ResolveCollisions()
    void            ResolveSpriteCollisions(CIwGameScene* scene, int entity_type);
    void            ResolveBox2dCollisions();
    
    virtual void    NotifyCollisionWithBomb(CollidableActor* actor);
    virtual void    NotifyCollisionWithExplosion(CollidableActor* actor);
    virtual void    NotifyCollisionWithStone(CollidableActor* actor);
    virtual void    NotifyCollisionWithTrampoline(CollidableActor* actor);
    virtual void    NotifyCollisionWithCandle(CollidableActor* actor);
    virtual void    NotifyCollisionWithPlatform(CollidableActor* actor);
    virtual void	NotifyCollisionWithPlayer(CollidableActor* actor);
    virtual void	NotifyCollisionWithEnemy(CollidableActor* actor);
    virtual void	NotifyCollisionWithCollectable(CollidableActor* actor);
    virtual void	NotifyCollisionWithGoal(CollidableActor* actor);
    virtual void	NotifyCollisionWithFixture(CollidableActor* actor);
    virtual void    NotifyCollisionWithPowerup(PowerUpActor* actor);
    virtual void    NotifyCollisionWithHeavyObject(CollidableActor* object);
    virtual void    NotifyCollisionWithParticles(CollidableParticles* particles);
    virtual void    NotifyCollisionWithSpikedBall(CollidableActor* ball);
    virtual void    NotifyCollisionWithSword(CIwGameActor* sword);
    
public:
    // TODO: #C4892 separate functions that doesn't have anything to do with Collidable concept
    // TODO: #F8892 move function IsFacing() from BaseActor to CollidableActor class
    void                SetAttributes(const char* name, CIwFVec2 pos, int type);
    virtual GameTimer*  GetObjectTimer();
    virtual bool        ResetOnLevelStart() const;
    void                UpdateAnimFrameVec2(CIwGameActor* target, const char* anim_name, CIwFVec2& from_pos,
                                CIwFVec2& to_pos, float duration);
    virtual bool        Respawn();
    void                ResetPosition();
    void                ResetAngle();
    int                 FindActorOrientationTo(CollidableActor* other, ActorOrientation::eAxis axis);
    
protected:
    bool m_resetOnLevelStart;      /**< The object can be restored to its original position if the player hits
                               the reset button */
};

//--------------------------------------------------------------------------------------------------------
// Box2dPhysics class interface
//--------------------------------------------------------------------------------------------------------

class Box2dPhysicsUti
{
public:
    //void SetAttributes(CIwGameActor& target, const char* name, CIwFVec2 pos, int type);
    bool AssignBox2dPhysics(CollidableActor& target, CIwGameShape* shape, CIwGameBox2dMaterial* material, bool sensor);
    bool AssignBox2dPhysics(CollidableActor& target, const char* shp, const char* mat, bool sensor);
    bool AssignBox2dPhysics(CollidableActor& target, int type);
    
    void ShowVisualCollision(CollidableActor& actor); // to show collision boundaries for debugging
    
};

//--------------------------------------------------------------------------------------------------------
// BodyPart class interface
//--------------------------------------------------------------------------------------------------------

/**
 * \brief Determines the position of an actor's body part relative to the actor's center.
 */
class BodyPart
{
public:
    BodyPart() {}
    BodyPart(CollidableActor* actor, const char* n, float x, float y);
    
    const char* GetName();
    CIwFVec2    GetPos() const;
    
private:
    CollidableActor*    m_actor;    /**< The pointer to the main actor */
    const char*         m_name;     /**< The name of this body part */
    CIwFVec2            m_offSetPos;/**< The offset position of this body part from the actor's center */
};
#endif
