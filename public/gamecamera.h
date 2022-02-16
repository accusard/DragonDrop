//--------------------------------------------------------------------------------------------------------
// Dragon Drop
//	gamecamera.h
// Programmed by: Vanny Sou
// 
//--------------------------------------------------------------------------------------------------------

#if !defined(_GAME_CAMERA_H_)
#define _GAME_CAMERA_H_

#include "game.h"


#define CAM_OFFSET_Y	0
#define CAM_OFFSET_X	0

enum EasingRange { HORIZONTAL, VERTICAL };

class GameCamera : public CIwGameCamera
{
private:
	CIwFVec2		StartingPosition;	//The starting position of the camera
	CIwFVec2		TargetPosition;		//the desired position of the camera
	float			TargetScale;		//the desired zoom level of the camera
	CIwFVec2		EasingRange;		//the target range position of the camera where its velocity should ease to a stop
	float			CameraYOffset;		//offset y position of the camera
	float			CameraXOffset;		//offset x position of the camera
	CIwRect         Boundary;			//boundary range of camera
	CIwGameActor*	CurrentTarget;		//Current camera target
	bool			inPosition;
	bool			Panning;			//state of the camera
	CIwGameTimer	DelayTimer;			//sets wait timer before panning begins
    CIwVec2         VirtualScreenSize;

public:
	//default constructor
	GameCamera() : StartingPosition(0,0), TargetPosition(0,0), TargetScale(0), EasingRange(0,0), 
		CameraYOffset(0), CameraXOffset(0), Boundary(0,0,0,0), CurrentTarget(NULL), inPosition(false), Panning(false), DelayTimer(), VirtualScreenSize(0,0) {};

	~GameCamera();
	
	
	CIwFVec2		GetStartingPostiion() const { return StartingPosition; }
	void			SetTargetPosition(CIwFVec2 pos);				//sets the desired target position
	void			SetTargetScale(float scale) {TargetScale = scale;}
	void			SetEasingRange(CIwFVec2 range) { EasingRange = range; }
	void			SetCameraYOffset(float offset) {CameraYOffset = offset; TargetPosition.y += offset;}
	void			SetCameraXOffset(float offset) {CameraXOffset = offset; TargetPosition.x += offset;}

	void            SetBoundary(CIwRect bound) ;					//the boundary of the camera
	bool			IsInEasingRange(int r);							//checks if camera is in range of EasingRange
	bool			CameraInTargetPosition(float offset);			//checks to see if current position hits target position
	void			PanCamera();									// pan camera to TargetPosition
	bool			OutOfBoundX();
    bool            OutOfBoundY();
    bool            OutOfBoundW();
    bool            OutOfBoundH();
	void			SetTarget(CIwGameActor* target);
	void			UpdateThroughUI();
	bool			IsInPosition() { return inPosition; }
	CIwGameTimer	GetDelayTimer() { return DelayTimer; }
	void			Update(float dt);
    void            SetVirtualScreenSize(CIwVec2 screen_size) {VirtualScreenSize = screen_size;}
};



#endif