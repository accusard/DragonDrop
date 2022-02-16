//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// visual_effect.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// VisualEffect class implementation
//--------------------------------------------------------------------------------------------------------

#if !defined(_VISUAL_EFFECT_H_)
#define _VISUAL_EFFECT_H_

#include "IwGameActor.h"
#include "IwGameBrush.h"
#include "IwGeomFVec4.h"

#include "game.h"

class CollidableParticles;

/**
 * \brief Contains data that define the properties of VisualEffect objects.
 *
 */
struct ParticleEffectsData
{
    float       spawn_delay;
    float       duration;
    int         count;
    float       gravity;
    CIwFVec4    color;
    CIwFVec4    color_vel;
    CIwFVec2    scale_range;
    CIwFVec4    vel_range;
    CIwFVec2    angle_range;
    CIwFVec2    pos_range;
    
    ParticleEffectsData() :
    spawn_delay(0.25f),
    duration(2.0f),
    count(1),
    gravity(1.0f),
    color(255,255,255,255),
    color_vel(0,0,0,0),
    scale_range(0.0f,1.0f),
    vel_range(-5,5,1,1),
    angle_range(0,0),
    pos_range(0,0)
    {}
};

/**
 * \brief  This class contain a set of functions that utilizes and instantiate IwGame engine particle
 *  effect systems to produce various visual effects.
 */
class VisualEffect
{
public:
    
    VisualEffect();
    ~VisualEffect();
    
    bool Init(CIwGameScene* scene, const int& num_particles);
    void InitWithSmokeEffects(CIwGameScene* scene, const int& num_particles,
                              float delay,
                              const CIwFVec4& col,
                              const CIwFVec4& col_velocity,
                              const CIwFVec2& scale_range,
                              const CIwFVec4& velocity_range,
                              const int coll_size);
    
    CIwGameActorParticles* GetVisualParticles();
    
    void GenerateRandomParticles(const char* img_name, e_GameLayers layer,
                                 float x, float y);
    
    void GenerateCollidableParticles(const char* img_name, e_GameLayers layer,
                                     float x, float y);
    
    // the following functions use modes and ranges to generate the specified effects
    void SliceActor(CIwGameActor* actor,
                    const float vr_left,
                    const float vr_right,
                    const float vr_up = 0.0f,
                    const float vr_down = 0.0f,
                    const float center_gap = 2,
                    const int layer = L_ACTORS);
    
    void ExplodeActor(CIwGameActor* actor,
                      CIwGameBrushImage* large_piece,
                      CIwGameBrushImage* small_piece,
                      const char* sound,
                      const int layer = L_ACTORS);
    
    CIwGameActor* BreakActor();
    void DustImpact();
    
private:
    
    
    
    CIwGameScene*           m_currentScene;
    ParticleEffectsData     m_data;
    CollidableParticles*    m_visualParticles;      /*<< A pointer to the CIwGameActorParticles object
                                                     attached to this Visual Effect. */
    int                     m_numOfParticles;
};

#endif
