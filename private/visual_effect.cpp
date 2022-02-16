//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// visual_effect.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// VisualEffect class implementation
//--------------------------------------------------------------------------------------------------------

#include "IwGameActorParticles.h"
#include "visual_effect.h"
#include "utility_actor.h"

//--------------------------------------------------------------------------------------------------------
// VisualEffect class implementation
//--------------------------------------------------------------------------------------------------------

VisualEffect::VisualEffect() : m_currentScene(NULL), m_data(ParticleEffectsData()), m_visualParticles(NULL),
                                m_numOfParticles(0)
{
    
}

VisualEffect::~VisualEffect()
{
    
}

/**
 * \brief Initilize a VisualEffect object and create CollidableParticles.
 *
 * \param scene The pointer to the current game scene.
 * \param num_particles Takes the address of an int.\
 * \return false and cancel initialization if scene == NULL
 */
bool VisualEffect::Init(CIwGameScene* scene, const int& num_particles)
{
	if(scene == NULL)
        return false;

    Box2dPhysicsUti b2dutil;
    
    // create particle actor
    CollidableParticles*  particles_actor = new CollidableParticles();
    
    // assign
    m_currentScene      = scene;
    m_visualParticles   = particles_actor;
    m_numOfParticles    = num_particles;
    
    // add particle actor to the current game scene before initializing it
    m_currentScene->addActor(particles_actor);
    particles_actor->Init(num_particles);
    
    return true;
}

/**
 * \brief The getter to m_visualParticles
 *
 * \return a pointer to the m_visualParticles data.
 */
CIwGameActorParticles* VisualEffect::GetVisualParticles()
{
    return m_visualParticles;
}

/**
 * \brief Initialize the VisualEffect object with the properties of smoke effects, and create CollidableParticles.
 *
 * Assigns color, color velocity, radomized scale and velocity range to produce the effects.
 *
 * \return cancel initialization if scene == NULL
 */
void VisualEffect::InitWithSmokeEffects(CIwGameScene* scene, const int& num_particles,
                                        float delay,
                                        const CIwFVec4& col,
                                        const CIwFVec4& col_velocity,
                                        const CIwFVec2& scale_range,
                                        const CIwFVec4& velocity_range,
                                        const int coll_size)
{
    // create collidable particles
    if( !VisualEffect::Init(scene, num_particles) )
       return;
    
    // assign the ParticleEffectsData
    m_data.spawn_delay = delay / num_particles;
    m_data.gravity     = -0.1f;                     // default gravity
    m_data.color       = col;
    m_data.color.w     = 150;                       // default color transparency
    m_data.color_vel   = col_velocity;
    m_data.vel_range    = velocity_range;
    m_data.scale_range  = scale_range;
    
    // randomize the scale and velocity
    m_visualParticles->setScaleMode(CIwGameActorParticles::PAM_Random);
    m_visualParticles->setVelocityMode(CIwGameActorParticles::PAM_Random);
    
    
    // set collision size

    int size = coll_size;

    m_visualParticles->setCollisionSize(size);
    
}

/**
 * \brief Create randomly generated particles.
 *
 * \param img_name the name of the brush to use for this particle
 * \param layer the layer of the particle
 * \param x the x position of the particle
 * \param y the y position of the particle
 */
void VisualEffect::GenerateRandomParticles(const char* img_name, e_GameLayers layer,
                                           float x, float y)
{
    CIwFVec2 sr(m_data.scale_range);
    CIwFVec4 vr(m_data.vel_range);
    CIwFVec2 ar(m_data.angle_range);
    CIwFVec2 pr(m_data.pos_range);
    
    CIwGameBrushImage*  brush;
    CIwGameImage*       image;
    CIwRect             src_rect;
    CIwFVec2            pos;
    
    brush       = (CIwGameBrushImage*)m_currentScene->getResourceManager()->FindResource(img_name,
                                                                          CIwGameXomlNames::Brush_Hash);
    image       = brush->getImage();
    src_rect    = brush->getSrcRect();
    pos         = CIwFVec2(x,y);
    
    m_visualParticles->setImage(image);
    m_visualParticles->setPosition(pos);
    m_visualParticles->setLayer(layer);
    
    // set the range
    m_visualParticles->setScaleRange(sr);
    m_visualParticles->setVelocityRange(vr);
    m_visualParticles->setPositionRange(pr);
    m_visualParticles->setAngleRange(ar);
    
    m_visualParticles->setType(PARTICLES);
    m_visualParticles->GenerateRandomParticles(m_numOfParticles, src_rect, m_data.color, m_data.color_vel,
                                               m_data.duration, m_data.count, m_data.spawn_delay,
                                               m_data.gravity);
}

/**
 * \brief Generate particles that are collidable in the scene.
 *
 * \param img_name the name of the brush to use for this particle
 * \param layer the layer of the particle
 * \param x the x position of the particle
 * \param y the y position of the particle
 */
void VisualEffect::GenerateCollidableParticles(const char* img_name, e_GameLayers layer,
                                 float x, float y)
{
    GenerateRandomParticles(img_name, layer, x, y);
}

/**
 * \brief This function takes a pointer to an actor and slice its image in half.
 * \param actor The pointer to the actor doing the effect.
 * \param layer The layer of the visual effect image.
 * \param vel_right The distance the half image will travel to the right.
 * \param vel_left The distance the half image will travel to the left
 * \param vel_up the upper range of the velocity on the y-axis
 * \param vel_down the lower range of the velocity on the y-axis
 * \param center_gap The distance between the sliced image
 */
void VisualEffect::SliceActor(CIwGameActor* actor,
                              const float vel_right,
                              const float vel_left,
                              const float vel_up,
                              const float vel_down,
                              const float center_gap,
                              const int layer)
{
	CIwGameScene* scene         = actor->getScene();
	CIwGameBrushImage* brush    = brush = (CIwGameBrushImage*)actor->getVisual()->getBrush();
    
    // need access to scene and brush for this effect
	if(scene == NULL || brush == NULL)
		return;
    
	int num_particles = 1; 

    // create particle actor
	CIwGameActorParticles* particle = new CIwGameActorParticles;
    
    // add particle actor to the current game scene before initializing it
	scene->addActor(particle);
	
    // create the image to be sliced
    CIwGameImage* image = brush->getImage();
	particle->setImage(image);
    
    // Init properties
	particle->Init(num_particles);
	particle->setPosition(actor->getPosition());
	particle->setLayer(layer);

    // divide the source rectangle image
	CIwRect src_rect1 = brush->getSrcRect();
	CIwRect src_rect2 = brush->getSrcRect();
	src_rect1.h = src_rect1.h * 0.5;// * 0.6;
	src_rect2.w = src_rect2.w * 0.5;

    // set the color
	CIwFVec4 color(255,255,255,255);
	CIwFVec4 color_vel(0,0,0,-5);
	
    // set the mode
	particle->setAngleMode(CIwGameActorParticles::PAM_Random);
	particle->setAngVelocityMode(CIwGameActorParticles::PAM_Random);
	particle->setVelocityMode(CIwGameActorParticles::PAM_Random);

	CIwFVec2 angle_r(0,360);
	particle->setAngleRange(angle_r);

	CIwFVec2 angle_vel_r(0,10);
	particle->setAngVelocityRange(angle_vel_r);
    
    // generate first image
	CIwFVec4 vel_r(center_gap, vel_right, vel_down, vel_up);
	particle->setVelocityRange(vel_r);
	particle->GenerateRandomParticles(num_particles, src_rect1, color, color_vel, 1, 1, 0,0.5f);
    
    // generate second image
	particle->setAngleMode(CIwGameActorParticles::PAM_Normal);
	CIwFVec4 vel_l(-center_gap, -vel_left, vel_down, vel_up);
	particle->setVelocityRange(vel_l);
	particle->GenerateRandomParticles(num_particles, src_rect2, color, color_vel, 1, 1, 0,0.5f);
}

/**
 * \brief This effect uses two different brush images to explode the actor into pieces
 */
void VisualEffect::ExplodeActor(CIwGameActor* actor,
                                CIwGameBrushImage* large_piece,
                                CIwGameBrushImage* small_piece,
                                const char* sound,
                                const int layer)
{
    CIwGameScene* scene         = actor->getScene();
    
    // need access to scene and brush for this effect
	if(scene == NULL || large_piece == NULL || small_piece == NULL)
		return;
    
    int num_particles = 4;
    CIwGameImage* large_image = NULL;
    CIwGameImage* small_image = NULL;
    float gravity;
    
	CIwGameActorParticles* explosion_particles = new CIwGameActorParticles();
	scene->addActor(explosion_particles);
    
    large_image = large_piece->getImage();
    small_image = small_piece->getImage();
    gravity = 0.1f * (scene->getBox2dWorld()->getGravity().y);
    
	explosion_particles->Init(num_particles);
	explosion_particles->setImage(large_image);
	explosion_particles->setPosition(actor->getPosition());
	explosion_particles->setLayer(layer);
	
	CIwRect src_rect = large_piece->getSrcRect();
    CIwRect src_rect2 = small_piece->getSrcRect();
    
	//only need pieces of the exploded actor
	src_rect.w *= 0.5;
	src_rect.h *= 0.5;
    
	CIwFVec4 color(255,255,255,255);
	CIwFVec4 color_vel(0,0,0,0);
    
	//RandomMode
	explosion_particles->setAngleMode(CIwGameActorParticles::PAM_Random);
	explosion_particles->setAngVelocityMode(CIwGameActorParticles::PAM_Random);
	explosion_particles->setVelocityMode(CIwGameActorParticles::PAM_Random);
	explosion_particles->setScaleMode(CIwGameActorParticles::PAM_Random);
	explosion_particles->setPositionMode(CIwGameActorParticles::PAM_Random);
    
	//Set the range for the randomized parameters
	CIwFVec2 angle_range(0,360);
	explosion_particles->setAngleRange(angle_range);
    
    CIwFVec2 scale_range;
	CIwFVec2 angle_vel_range;
    
	CIwFVec4 vel_range(-15,15,-15,15);
	explosion_particles->setVelocityRange(vel_range);
    
	CIwFVec2 pos_range(50,50);
	explosion_particles->setPositionRange(pos_range);
    
	// generate the large particles
    scale_range = CIwFVec2(0.6, 1);
	explosion_particles->setScaleRange(scale_range);
    angle_vel_range = CIwFVec2(-5,5);
    explosion_particles->setAngVelocityRange(angle_vel_range);
	explosion_particles->GenerateRandomParticles(num_particles, src_rect, color, color_vel, 2, 1, 0,
                                                 gravity);
    
    // generate the small particles
    scale_range = CIwFVec2(0.0f, 1.0f);
	explosion_particles->setScaleRange(scale_range);
    angle_vel_range = CIwFVec2(-10,10);
    explosion_particles->setAngVelocityRange(angle_vel_range);
    explosion_particles->GenerateRandomParticles(num_particles * 2, src_rect2, color, color_vel, 2, 1,
                                                 0, gravity);
    
    if(sound != NULL)
        IW_GAME_AUDIO->PlaySound(sound);
}
