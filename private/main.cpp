//--------------------------------------------------------------------------------------------------------
// Dragon Drop 
// Programmed by: Vanny Sou
// 
//--------------------------------------------------------------------------------------------------------

#include "s3e.h"
#include "Game.h"
#include "IwResManager.h"
#include "Marm/IwGamePlatformMarm.h"

int main()
{
	IwGamePlatformMarm_Create();
    IwResManagerInit();
    
	CIwGameError::LogError("BOOT: Memory used: ", CIwGameString(s3eMemoryGetInt(S3E_MEMORY_USED)).c_str());

	Game::Create();
	GAME->Init(true);

	CIwGameError::LogError("POST_INIT: Memory used: ", CIwGameString(s3eMemoryGetInt(S3E_MEMORY_USED)).c_str());

	while (!s3eDeviceCheckQuitRequest())
	{
		if (!GAME->Update())
			break;

		GAME->Draw();

	}

	GAME->Release();
	Game::Destroy();
	
    IwResManagerTerminate();
	IwGamePlatformMarm_Destroy();
	return 0;
}





