//--------------------------------------------------------------------------------------------------------
// Dragon Drop
//	scenemanagement.h
// Programmed by: Vanny Sou
// 
// Manage the current scene of the game which include the parallax backgrouds, the camera, scene states, etc.. 
//--------------------------------------------------------------------------------------------------------

#if !defined(_SCENEMANAGEMENT_H_)
#define _SCENEMANAGEMENT_H_

#include "IwGame.h"

class SceneManagement
{
private:
	CIwGameCamera*			SceneCamera;
	CIwList<CIwGameActor*>	Backgrounds;

public:
	SceneManagement();
	~SceneManagement();
};

#endif