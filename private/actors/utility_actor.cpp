//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// utility_actor.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
// Implementation of utility actor classes to all other actors with movements
//--------------------------------------------------------------------------------------------------------

#include "utility_actor.h"
//#include "game.h"
#include "base_actor.h"
#include "options.h"
#include "player_actor.h"
#include "game_state.h"
#include "powerup_actor.h"

//--------------------------------------------------------------------------------------------------------
// Implementation of Box2dPhysics class
//--------------------------------------------------------------------------------------------------------

/**
 * \brief Assign the shape and material to a target actor based on the specified type.
 *
 * \param target The actor to be applied to.
 * \param type Specified to get the Box2dObjectData
 */
bool Box2dPhysicsUti::AssignBox2dPhysics(CollidableActor& target, int type)
{
    CIwGameString   shape;
    CIwGameString   material;
    ObjectData      data;
    
    data = GAME->GetBox2dObjectData(type);
    
    return AssignBox2dPhysics(target, data.shape, data.material, true);
}

/**
 * \brief Assign the shape and material to a target actor.
 *
 * \param target The actor to be applied to.
 * \param shp the char* of the shape.
 * \param mat the char* of the material.
 * \param sensor set this as true or false.
 */
bool Box2dPhysicsUti::AssignBox2dPhysics(CollidableActor& target, const char* shp, const char* mat, bool sensor)
{
    CIwGameShape* shape = (CIwGameShape*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource(shp, CIwGameXomlNames::Shape_Hash);
    
    CIwGameBox2dMaterial* material = (CIwGameBox2dMaterial*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource(mat, CIwGameXomlNames::Box2dMaterial_Hash);
    
    if(shape == NULL || material == NULL)
    {
        CIwGameError::LogError("Error:: Could not find shape and/or material to assign to Actor..", target.getName().c_str());
        return false;
    }
    
    return AssignBox2dPhysics(target, shape, material, sensor);
}

/**
 * \brief Assign the shape and material to a target actor.
 *
 * \param target The actor to be applied to.
 * \param shape the pointer to the shape.
 * \param material the pointer to the material.
 * \param sensor set this as true or false.
 */
bool Box2dPhysicsUti::AssignBox2dPhysics(CollidableActor& target, CIwGameShape* shape, CIwGameBox2dMaterial* material, bool sensor)
{
    float angle     = target.getAngle();
    CIwFVec2 pos    = target.getPosition();
    
    CIwGameBox2dBody* body = new CIwGameBox2dBody();
    
    target.setAngle(-angle);
    target.setBox2dBody(body);
    target.getBox2dBody()->setBodyShape(shape);
    target.getBox2dBody()->InitBody(target.getScene()->getBox2dWorld(), shape, material, &pos, angle, 0, 0);
    target.setVelocityDamping(0, 0);
    target.setAngularVelocityDamping(0);
    target.getBox2dBody()->setCollisionFlags(1,1,0);
    target.getBox2dBody()->setUserData(&target);
    target.getBox2dBody()->setAsSensor(sensor);
    target.setCollidable(true);
    
    return true;
}

/**
 * \brief Show the visual collision of the specified actor.
 *
 * \param actor The actor to apply this visual to.
 *
 *  TODO: rewrite function to use Box2d debug drawings instead
 */
void Box2dPhysicsUti::ShowVisualCollision(CollidableActor& actor)
{
    if(actor.getBox2dBody() == NULL)
    {
        CIwGameError::LogError("Calling CollidableActor::ShowVisualCollision..");
        CIwGameError::LogError("..actor does not have Box2dMaterial attached to it to set its visual shape..");
        return;
    }
    
    int shape_type = actor.getBox2dBody()->getBodyShape()->getShape()->ShapeType;
    
    int width = 1;
    int height = 1;
    
    if(shape_type == S_POLYGON)
    {
        //Setting Polygon Shape
        CIwGameGeomShapePolygon* polygon = (CIwGameGeomShapePolygon*)actor.getBox2dBody()->getBodyShape()->getShape();
        
        //find the size of the fixture
        CIwFVec2 smallest = GAME->FindSmallestVertex(polygon);
        CIwFVec2 largest = GAME->FindLargestVertex(polygon);
        
        width = largest.x - smallest.x;
        height = largest.y - smallest.y;
        
        if(width < 0) width *= -1.0f;
        if(height < 0) height *= -1.0f;
    }
    else
    if(shape_type ==  S_BOX)
    {
        CIwGameGeomShapeBox* box = (CIwGameGeomShapeBox*)actor.getBox2dBody()->getBodyShape()->getShape();
        
        width = box->Width;
        height = box->Height;
    }
    else
    if(shape_type ==  S_CIRCLE)
    {
        CIwGameGeomShapeCircle* circle = (CIwGameGeomShapeCircle*)actor.getBox2dBody()->getBodyShape()->getShape();
        
        width = circle->Radius * 2;
        height = circle->Radius * 2 ;
    }
    
    //set the render size
    if(width < 0) width *= -1;
    if(height < 0) height *= -1;
    
    actor.getVisual()->setDestSize(width, height);
    actor.getVisual()->setAlphaMode(CIwMaterial::ALPHA_HALF);
    
}

//--------------------------------------------------------------------------------------------------------
// Implementation of BodyPart class
//--------------------------------------------------------------------------------------------------------

BodyPart::BodyPart(CollidableActor* actor, const char* n, float x, float y) :
m_actor(actor), m_name(n), m_offSetPos(CIwFVec2(x,y))
{}

/**
 * \brief Get the name of the body parts
 *
 * \return m_name The name of the BodyPart object
 */
const char* BodyPart::GetName()
{
    return m_name;
}

/**
 * \brief The position of the BodyPart object relative to the attached actor's center position.
 *
 * \return The offset position of this BodyPart object
 */
CIwFVec2 BodyPart::GetPos() const
{
    CIwFVec2 ret_pos(0,0);
    
    // current actor's position and facing
    CIwFVec2 actor_pos  = m_actor->getPosition();
    int facing          = ((BaseActor*)m_actor)->GetFacing();
    
    // get the body part's offset position
    ret_pos.x = actor_pos.x + (m_offSetPos.x) * facing;
    ret_pos.y = actor_pos.y + (m_offSetPos.y) * facing;
    
    return ret_pos;
}

//--------------------------------------------------------------------------------------------------------
// Implementation of BurnableActor class
//--------------------------------------------------------------------------------------------------------

BurnableActor::BurnableActor() : m_burnLevel(0) {}

/**
 * \brief Init the max burn level that an actor can take before it is set to be destroyed.
 */
void BurnableActor::Init(int max_burn_level)
{
    m_burnLevel = max_burn_level;
}

/**
 * \brief Returns the max burn level of an actor.
 */
int BurnableActor::GetBurnLevel() const
{
    return m_burnLevel;
}

/**
 * \brief Reduce 1 point from the max burn level of an actor.
 */
void BurnableActor::ApplyBurn()
{
    if(m_burnLevel < 1)
        return;
    
    m_burnLevel--;
}

/**
 * \brief
 */
bool BurnableActor::CanCrumble()
{
    if(m_burnLevel > 0)
        return false;
    
    return true;
}

//--------------------------------------------------------------------------------------------------------
// Implementation of CollidableActor class
//--------------------------------------------------------------------------------------------------------

CollidableActor::CollidableActor() : CIwGameActorImage(), m_resetOnLevelStart(false)
{
    if(GAME->GetUserOptions()->IsDebugging() && getName().getHash() != CIwGameString("WorldMapActor").getHash())
    {
        setDraggable(true);
    }
}

bool CollidableActor::UpdateVisual()
{
    //if(GAME->GetStateID() != STATE_PAUSE)
        return CIwGameActorImage::UpdateVisual();
    
    //return false;
}

/**
 * \brief Sets some basic attributes for this actor.
 *
 * \param name The name of this actor.
 * \param pos The position of this actor.
 * \param type Actor type.
 */
void CollidableActor::SetAttributes(const char* name, CIwFVec2 pos, int type)
{
    setName(name);
    setPosition(pos.x, pos.y);
    setDraggable(false);
    setType(type);
    setLayer(L_ACTORS);
    //setActualClassType("baseactor");
}

/**
 * \brief Get the timer of the object.
 */
GameTimer*  CollidableActor::GetObjectTimer()
{
    return NULL;
}

bool CollidableActor::LoadFromXoml(IIwGameXomlResource* parent, bool load_children, CIwGameXmlNode* node)
{
    if(!CIwGameActorImage::LoadFromXoml(parent, load_children, node))
        return false;
    
    for(CIwGameXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); ++it)
    {
        unsigned int attrib_name_hash = (*it)->getName().getHash();
        if(attrib_name_hash == CIwGameXomlNames::Layer_Hash)
        {
            unsigned int layer_hash = (*it)->GetValue().getHash();
            
            if(layer_hash == CIwGameString("actor").getHash())
            {
                setLayer(L_ACTORS);
            }
        }
        else
        if(attrib_name_hash == CIwGameXomlNames::Type_Hash)
        {
            unsigned int type_hash = (*it)->GetValue().getHash();
            
            if(type_hash == CIwGameString("constrictor").getHash())
            {
                setType(CONSTRICTOR);
            }
            else
            if(type_hash == CIwGameString("obstacles").getHash())
            {
                setType(OBSTACLES);
            }
            else
            if(type_hash == CIwGameString("stone").getHash())
            {
                setType(DROP_STONE);
            }
            else
            if(type_hash == CIwGameString("enemy").getHash())
            {
                setType(ENEMY);
            }
        }
        
    }
    
    return true;
}

/**
 * \brief Notifies the other actor that it has collided with this actor.
 *
 */
void CollidableActor::ResolveCollisions()
{
    if(Destroyed)
        return;
    
    ResolveBox2dCollisions();
}

/**
 * \brief Resolve the collisions of this actor's sprites with an entity of a certain type.
 *
 * \param scene The scene that the collision is taking place.
 * \param entity_type The type of the entity to check for.
 */
void CollidableActor::ResolveSpriteCollisions(CIwGameScene* scene, int entity_type)
{
    for(CIwGameScene::_Iterator it = scene->begin(); it != scene->end(); ++it)
    {
        if( (*it)->getType() == entity_type )
        {
            if(CheckCollisionRect(*it))
                (*it)->NotifyCollision(this);
        }
    }
}

void CollidableActor::ResolveBox2dCollisions()
{
    if(Box2dBody != NULL)
    {
        //a list of collidables
        CIwGameSlotArray<CIwGameBox2dCollidable*>& started = Box2dBody->getCollisionsStarted();
        
        for(int t = 0; t < started.getSize(); t++)
        {
            CIwGameBox2dCollidable* collision = started.element_at(t);
            if(collision != NULL)
            {
                CollidableActor* actor = (CollidableActor*)collision->getUserData();
                if(actor != NULL)
                    actor->NotifyCollision(this);
            }
        }
    }
}

/**
 * \brief Notify this actor that it has collided with a bomb.
 * \param bomb_actor The bomb that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithBomb(CollidableActor* bomb_actor)
{}

/**
 * \brief Notify this actor that it has collided with an explosion.
 * \param exp_actor The explosion that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithExplosion(CollidableActor* exp_actor)
{}

/**
 * \brief Notify this actor that it has collided with a stone.
 * \param stone_actor The stone that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithStone(CollidableActor* stone_actor)
{}

/**
 * \brief Notify this actor that it has collided with a trampoline.
 * \param trampoline_actor The trampoline that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithTrampoline(CollidableActor* trampoline_actor)
{}

/**
 * \brief Notify this actor that it has collided with a candle.
 * \param candle_actor The candle that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithCandle(CollidableActor* candle_actor)
{}

/**
 * \brief Notify this actor that it has collided with a platform.
 * \param platform_actor The platform that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithPlatform(CollidableActor* platform_actor)
{}

/**
 * \brief Notify this actor that it has collided with the player
 * \param player The player that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithPlayer(CollidableActor* player)
{}

/**
 * \brief Notify this actor that it has collided with an enemy
 * \param enemy The enemy that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithEnemy(CollidableActor* actor)
{}

/**
 * \brief Notify this actor that it has collided with a collectable object
 * \param collectable_actor The collectable that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithCollectable(CollidableActor* collectable_actor)
{}

/**
 * \brief Notify this actor that it has collided with the goal
 * \param goal The goal that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithGoal(CollidableActor* goal)
{}

/**
 * \brief Notify this actor that it has collided with a fixture
 * \param fixture The fixture that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithFixture(CollidableActor* fixture)
{}

/**
 * \brief Notify this actor that it has collided with a powerup
 * \param power The powerup that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithPowerup(PowerUpActor* power)
{}

/**
 * \brief Notify this actor that it has collided with a heavy object
 * \param object The object that this actor collided with.
 */
void CollidableActor::NotifyCollisionWithHeavyObject(CollidableActor* object)
{}

void CollidableActor::NotifyCollisionWithParticles(CollidableParticles* particles)
{}

void CollidableActor::NotifyCollisionWithSpikedBall(CollidableActor* ball)
{}

void CollidableActor::NotifyCollisionWithSword(CIwGameActor* sword)
{}

bool CollidableActor::Update(float dt)
{
    if(!CIwGameActorImage::Update(dt) || isDestroyed())
        return false;
    
    return true;
}

/*
void CollidableActor::SetAttributes(const char* name, CIwFVec2 pos, int type)
{
    setName(name);
    setPosition(pos.x, pos.y);
    setDraggable(false);
    setType(type);
    setLayer(L_ACTORS);
    //setActualClassType("baseactor");
}



bool CollidableActor::AssignBox2dPhysics(int type)
{
    CIwGameString   shape;
    CIwGameString   material;
    ObjectData      data;
    
    data = GAME->GetBox2dObjectData(type);
    
    return AssignBox2dPhysics(data.shape, data.material, true);
}

bool CollidableActor::AssignBox2dPhysics(const char* shp, const char* mat, bool sensor)
{
    CIwGameShape* shape = (CIwGameShape*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource(shp, CIwGameXomlNames::Shape_Hash);
    
    CIwGameBox2dMaterial* material = (CIwGameBox2dMaterial*)IW_GAME_GLOBAL_RESOURCES->getResourceManager()->findResource(mat, CIwGameXomlNames::Box2dMaterial_Hash);
    
    if(shape == NULL || material == NULL)
    {
        CIwGameError::LogError("Error:: Could not find shape and/or material to assign to Actor..", getName().c_str());
        return false;
    }
    
    Angle = -Angle;
    Box2dBody = new CIwGameBox2dBody();
    Box2dBody->setBodyShape(shape);
    Box2dBody->InitBody(Scene->getBox2dWorld(), shape, material, &Position, Angle, 0, 0);
    VelocityDamping.x = 0;
    VelocityDamping.y = 0;
    AngularVelocityDamping = 0;
    setVelocityDamping(VelocityDamping.x, VelocityDamping.y);
    setAngularVelocityDamping(AngularVelocityDamping);
    Box2dBody->setCollisionFlags(1,1,0);
    Box2dBody->setUserData(this);
    Box2dBody->setAsSensor(sensor);
    setCollidable(true);
    
    return true;
}

void CollidableActor::ShowVisualCollision()
{
    if(Box2dBody == NULL)
    {
        CIwGameError::LogError("Calling CollidableActor::ShowVisualCollision..");
        CIwGameError::LogError("..actor does not have Box2dMaterial attached to it to set its visual shape..");
        return;
    }
    
    int shape_type = Box2dBody->getBodyShape()->getShape()->ShapeType;
    
    int width = 1;
    int height = 1;
    
    if(shape_type == S_POLYGON)
    {
        //Setting Polygon Shape
        CIwGameGeomShapePolygon* polygon = (CIwGameGeomShapePolygon*)Box2dBody->getBodyShape()->getShape();
        
        //find the size of the fixture
        CIwFVec2 smallest = GAME->FindSmallestVertex(polygon);
        CIwFVec2 largest = GAME->FindLargestVertex(polygon);
        
        width = largest.x - smallest.x;
        height = largest.y - smallest.y;
        
        if(width < 0) width *= -1.0f;
        if(height < 0) height *= -1.0f;
    }
    else
    if(shape_type ==  S_BOX)
    {
        CIwGameGeomShapeBox* box = (CIwGameGeomShapeBox*)Box2dBody->getBodyShape()->getShape();
        
        width = box->Width;
        height = box->Height;
    }
    else
    if(shape_type ==  S_CIRCLE)
    {
        CIwGameGeomShapeCircle* circle = (CIwGameGeomShapeCircle*)Box2dBody->getBodyShape()->getShape();
        
        width = circle->Radius * 2;
        height = circle->Radius * 2 ;
    }
    
    //set the render size
    if(width < 0) width *= -1;
    if(height < 0) height *= -1;
    
    getVisual()->setDestSize(width, height);
    getVisual()->setAlphaMode(CIwMaterial::ALPHA_HALF);
    
}

 */

void CollidableActor::UpdateAnimFrameVec2(CIwGameActor* target, const char* anim_name, CIwFVec2& from_pos, CIwFVec2& to_pos, float duration)
{
    CIwGameAnimTimeline* timeline = target->getTimeline();
    
    if(timeline != NULL)
    {
        //get the instance of the animation
        CIwGameAnimInstance* instance = timeline->findAnimation(anim_name);
        CIwGameAnim* anim = instance->getAnimation();
        int count = anim->getFrameCount() - 1;
        
        //return if frame count is 0 or greater than 2
        if(count < 0 || count > 2)
            return;
        
        //update data on to-position
        CIwGameAnimFrameVec2* frame = (CIwGameAnimFrameVec2*)anim->getFrame(1);
        frame->data.x   = to_pos.x;
        frame->data.y   = to_pos.y;
        frame->Time     = duration;
        
        //update data on from-position
        frame = (CIwGameAnimFrameVec2*)anim->getFrame(0);
        frame->data.x = from_pos.x;
        frame->data.y = from_pos.y;
        
        //update the animation durtaion
        anim->setDuration(duration);
        
        //restart timeline
        timeline->restart();
    }
}

/**
 * \brief Returns the value of m_isResttable, which will determine if the actor can be reset to its original state at levelrestart
 */
bool CollidableActor::ResetOnLevelStart() const
{
    return m_resetOnLevelStart;
}

/**
 * \brief Calls all functions that will reset the actor's original state
 */
bool CollidableActor::Respawn()
{
    if(!ResetOnLevelStart())
        return false;
    
    ScaleX		= OriginalScale.x;
    ScaleY		= OriginalScale.y;
    
    if(!isActive())
        setActive(true);
    
    if(Box2dBody != NULL)
    {
        Box2dBody->getBody()->SetFixedRotation(true);
        Box2dBody->setCollisionFlags(1,1,0);
    }
    setCollidable(true);
    ResetPosition();
    ResetAngle();
    
    if(Timeline != NULL)
        Timeline->restart();
    
    return true;
}

/**
 * \brief Only reset the actor's position. For levelend or levelrestart, call Respawn()
 */
void CollidableActor::ResetPosition()
{
    float pos_x = OriginalPosition.x;
    float pos_y = OriginalPosition.y - 2.0f;
    
    setPosition(pos_x, pos_y);
    setVelocity(0, -0.01f);
}

/**
 * \brief Only reset the actor's angle. For levelend or levelrestart, call Respawn()
 */
void CollidableActor::ResetAngle()
{
    setAngle(OriginalAngle);
    setAngularVelocity(0);
}

/**
 * \brief Find the actor's orientation based on the position of the other's Y-axis actor.
 */
int CollidableActor::FindActorOrientationTo(CollidableActor* other, ActorOrientation::eAxis axis)
{
    ActorOrientation orient(this);
    
    return orient.FindOrientation(other, axis);
}

//--------------------------------------------------------------------------------------------------------
// CollidableParticles class implementations
//--------------------------------------------------------------------------------------------------------
bool CollidableParticles::UpdateVisual()
{
    //if(GAME->GetStateID() != STATE_PAUSE)
        return CIwGameActorParticles::UpdateVisual();
    
    //return false;
}

CollidableParticles::CollidableParticles()
{
    setCollidable(true);
}

void CollidableParticles::ResolveCollisions()
{
    
    // currently only check collision with player
    CIwGameActor* player = Scene->findActor("Player");
    
    if(player == NULL && !player->isActive())
        return;
    
    if(CheckCollision(player))
    {
        static_cast<PlayerActor*>(player)->NotifyCollisionWithParticles(this);
    }
    
}



//--------------------------------------------------------------------------------------------------------
// ActorOrientation class implementations
//--------------------------------------------------------------------------------------------------------

/**
 * \brief The constructor of the ActorOrientation class. Determines an actor's orientation in relation to another actor's position.
 * TODO: using actor size does not work to determine m_length. maybe use image size instead
 *
 */
ActorOrientation::ActorOrientation(CIwGameActor* actor) :
    m_currentActor(actor),
    m_orientation(UNDETERMINED),
    m_length(actor->getSize())
{
    
}

/**
 * \brief Determines where the other actor's position is on the y-axis and return the current actor's orientation based on that.
 * \param other_actor The pointer to the other actor to be compared to.
 */
int ActorOrientation::FindOrientation(CIwGameActor* other_actor, ActorOrientation::eAxis axis)
{
    int actor_orientation = ActorOrientation::UNDETERMINED;
    
    switch (axis)
    {
        case X_AXIS:
        {
            float other_pos_x = other_actor->getPosition().x;
            actor_orientation = FindOrientationFromX(other_pos_x);
        }
        break;
            
        case Y_AXIS:
        {
            float other_pos_y = other_actor->getPosition().y;
            actor_orientation = FindOrientationFromY(other_pos_y);
        }
        break;
    }
    
    return actor_orientation;
}

/**
 * \brief Determines the current actor's orientation from a point on the x-axis
 * \param x Where the point is located on the x-axis.
 */
int ActorOrientation::FindOrientationFromX(const float x)
{
    m_orientation           = UNDETERMINED;
    
    float curr_pos_x        = m_currentActor->getPosition().x;
    float other_pos_x       = x;
    
    if(other_pos_x <= curr_pos_x)
        m_orientation = RIGHT;
    else
    if(other_pos_x >= curr_pos_x)
        m_orientation = LEFT;
    
    return m_orientation;
}

/**
 * \brief Determines the current actor's orientation from a point on the y-axis
 * \param y Where the point is located on the y-axis.
 */
int ActorOrientation::FindOrientationFromY(const float y)
{
    m_orientation           = UNDETERMINED;
    
    float curr_pos_y        = m_currentActor->getPosition().y;
    float other_pos_y       = y;
    
    if(other_pos_y <= curr_pos_y)
        m_orientation = BELOW;
    else
        if(other_pos_y >= curr_pos_y)
            m_orientation = ABOVE;
    
    return m_orientation;
}
/**
 * \brief Determines if the other actor's position falls directly within the size of the current actor
 * \param other_actor The pointer to the other actor.
 * TODO: m_length currently holds nothing
 */
bool ActorOrientation::IsWithinActorSize(CIwGameActor* other_actor) const
{
    bool within = false;
    
    int     length          = m_length.x / 2;
    float   op_x            = other_actor->getPosition().x;
    float   min_length_x    = m_currentActor->getPosition().x - length;
    float   max_length_x    = m_currentActor->getPosition().x + length;
    
    if(op_x >= min_length_x || op_x <= max_length_x)
        within = true;
    
    return within;
}
