//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// boss_dragon.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//  BossDragon class interface
//--------------------------------------------------------------------------------------------------------

#ifndef dragon_drop_hb_boss_dragon_h
#define dragon_drop_hb_boss_dragon_h

#include "enemy_actor.h"
#include "game_timer.h"
/**
 * \brief This actor will move toward a designated anchor and will begin its attack once it reaches the 
 * point of attack.
 *
 * When a player is within range. The actor will find the nearest anchor and move towards that point. 
 * Its point of attack is usually the anchor that have been placed in the level beforehand. Once it 
 * reaches that point, it will check if a player is nearby. If there is a player, it will change its 
 * state to an attack state.
 */
class BossDragon : public EnemyActor
{
private:
    GameTimer           m_fireBreathTimer;      /**< The duration of the actor's main attack. */
    BodyPart            m_mouth;                /**< Used to determine the origin of the actor's current
                                                 mouth position for a fire breath attack */
    CIwGameActorParticles*   m_breathAttack;      /**< A pointer to the firebreath attack */
    
public:
    BossDragon();
    ~BossDragon();
    
    bool    LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node);
    bool    Update(float dt);
    
    void    Init();
    void    Init(const char* image_name, float init_poa, BodyPart mouth);
    void    StartAttack();
    bool    UpdateAttack(CIwGameActor* actor);
    void    StopAttack();
    
    void    NotifyTargetAcquired();
    void    NotifyMovementFinished();
    
    void    SetBreathDuration(const uint64 duration);
    void    BreatheFire(const int num_particles,
                        float x, float y,
                        const CIwFVec2& sr, const CIwFVec4& vr, const CIwFVec4&,
                        float delay);
    
    /**
     * \name Collision
     *
     * Functions relating to the collision of various game objects.
     */
    void    NotifyCollisionWithStone(CollidableActor* stone);
    void    NotifyCollisionWithPlatform(CollidableActor* platform);
    void    NotifyCollisionWithSpikedBall(CollidableActor *ball);
    
    /**
     * \name Movement
     *
     * Funcions relating to the movement of this actor.
     */
     bool	MovingDamageOthers();
    
    /**
     * \name Actions
     *
     * Functions relating to the action of this actor
     */
    void        ActionsOnNotify(Event event);
    void        NotifyDying();
    CIwFVec2    CreatePointOfAttack();
    
};

class BossDragonCreator : public IIwGameXomlClassCreator
{
public:
    BossDragonCreator() { setClassName("BossDragon"); }
    IIwGameXomlResource* CreateInstance(IIwGameXomlResource* parent) { return new BossDragon(); }
};
#endif
