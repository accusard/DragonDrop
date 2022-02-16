//--------------------------------------------------------------------------------------------------------
// Dragon Drop
//	gamecamera.h
// Programmed by: Vanny Sou
// 
//--------------------------------------------------------------------------------------------------------

#include "gamecamera.h"
#include "player_actor.h"

GameCamera::~GameCamera()
{
	
}

void GameCamera::SetTargetPosition(CIwFVec2 pos)
{ 
	pos.x += CameraXOffset;
	pos.y += CameraYOffset;
	TargetPosition = pos;
}

bool GameCamera::IsInEasingRange(int r)
{
	bool in_range = false;

	switch(r)
	{
	case HORIZONTAL:
		{
			if(Position.x > TargetPosition.x - EasingRange.x && Position.x < TargetPosition.x + EasingRange.x)
				in_range = true;
		}
		break;
	case VERTICAL:
		{
			if(Position.y > TargetPosition.y - EasingRange.y && Position.y < TargetPosition.y + EasingRange.y)
				in_range = true;
		}
		break;
	}
	return in_range;
}

bool GameCamera::CameraInTargetPosition(float offset)
{
	Panning = false;
	if(Position.x < TargetPosition.x + offset && Position.x > TargetPosition.x - offset)
		return true;
	else
		return false;
}

void GameCamera::PanCamera()
{

	//if(!IsInEasingRange(VERTICAL))
	//{
		if(TargetPosition.y < Position.y)
			Velocity.y += (float)-0.01;
		else
		if(TargetPosition.y > Position.y)
			Velocity.y += (float)0.01;
	//}

	//if(!IsInEasingRange(HORIZONTAL))
	//{
		if(TargetPosition.x < Position.x)
			Velocity.x += (float)-0.01;
		else
		if(TargetPosition.x > Position.x)
			Velocity.x += (float)0.01;
	//}
}

bool GameCamera::OutOfBoundX()
{
	if(Position.x <= Boundary.x)
		return true;

    return false;
}

bool GameCamera::OutOfBoundY()
{
    if(Position.y >= Boundary.y)
        return true;

    return false;
}

bool GameCamera::OutOfBoundW()
{
    if(Position.x >= Boundary.w)
        return true;

    return false;
}

bool GameCamera::OutOfBoundH()
{
    if(Position.y <= Boundary.h)
        return true;
    
    return false;
}

void GameCamera::SetTarget(CIwGameActor* target)
{
	GameCamera::CurrentTarget = target;
	Position = target->getPosition();
	SetTargetPosition(target->getPosition());
}

void GameCamera::UpdateThroughUI()
{
	Position.x = CurrentTarget->getPosition().x * Scale;
	Position.y = CurrentTarget->getPosition().y * Scale;
    
	Position.y += CameraYOffset;
    
	if(OutOfBoundY() && Boundary.y != 0)
		Position.y = Boundary.y;
    
    if(OutOfBoundX() && Boundary.x != 0)
        Position.x = Boundary.x;
    
    if(OutOfBoundW() && Boundary.w != 0)
        Position.x = Boundary.w;
    
    if(OutOfBoundH() && Boundary.h != 0)
        Position.y = Boundary.h;
}

void GameCamera::Update(float dt)
{
	//if(CurrentTarget != NULL)
	UpdateThroughUI();

	PanCamera();

	CIwGameCamera::Update(dt);
}

void GameCamera::SetBoundary(CIwRect bound) 
{ 
	Boundary = bound;

	Boundary.x /= Scale;
	Boundary.y /= Scale;
	Boundary.w /= Scale;
	Boundary.h /= Scale;
}