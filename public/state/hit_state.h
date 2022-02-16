//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// state_actor.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------

#if !defined(_HIT_STATE_)
#define _HIT_STATE_

#include "base_actor.h"
#include "actor_state.h"

class Hit : public ActorState
{
public:
    Hit(BaseActor* actor,
        CIwGameAnimTimeline* hover_timeline,
        uint64 nex_update,
        int dmg = 1);
    
    void Load();
    void Update();
    
    bool CanBeHurt() const;
    
private:
    int         m_dmg;   /**< The damage to apply to the actor */

};

#endif /* defined(__dragon_drop_m__actor_state__) */
