//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// debug_actor_text.cpp
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#include "actors/debug_actor_text.h"
#include "options.h"

void HUDTextDisplay::SetFade(float fade)
{
    Fade = fade;
}

void HUDTextDisplay::ReadPointerPosition()
{
	if(!IW_GAME_INPUT->isPointerAvailable())
	{
		CIwGameError::LogError("Pointer System unavailable!");
		return;
	}
	
	int numTouches = IW_GAME_INPUT->getTouchCount();

	//Start the reading
	if(numTouches != 0)
	{
		//get screen size
		CIwVec2 screen = GAME->getCurrentScene()->getScreenSize();
		screen.x = screen.x /2;
		screen.y = screen.y /2;
        
		//get pointer position
		CIwVec2 pointerpos;
		CIwVec2 touchpos = IW_GAME_INPUT->getTouchedPos(0);
        
		pointerpos.x = (touchpos.x - screen.x + (int32)GAME->getCurrentScene()->getCamera()->getPosition().x);
		pointerpos.y = (touchpos.y - screen.y + (int32)GAME->getCurrentScene()->getCamera()->getPosition().y);
		
        //set in PointerPosition
        SetPointerPosition(pointerpos.x, pointerpos.y);
	}	
}

void HUDTextDisplay::SetPointerPosition(float x, float y)
{
    if(Position.x == x && Position.y == y)
        return;
    
    PointerPosition.x = x;
    PointerPosition.y = y;
}

bool HUDTextDisplay::InitTextDisplay(CIwGameFont* font, CIwGameString& text)
{
    int flags = IW_GX_FONT_DEFAULT_F;
    CIwVec2 screen_size = GAME->getCurrentScene()->getScreenSize();
    CIwRect rect = CIwRect(0,0,screen_size.x,100);
    CIwGameString string;
    
    //load the timelines
    FadeOutBlack = IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->findTimeline("black_text_fade_out");
    FadeInBlack = IW_GAME_GLOBAL_RESOURCES->getTimelinesManager()->findTimeline("black_text_fade_in");
    
    TextDisplay = new CIwGameActorText();
    TextDisplay->setLinkedTo(this);
    
    Scene->addActor(TextDisplay);
    
    TextDisplay->setMarginIsPadding(true);
    TextDisplay->setAutoHeight(true);
    TextDisplay->setMargin(0,0,-10,0);
    TextDisplay->setDocking(DK_Top);
    TextDisplay->Init(font, rect, text, flags);
    TextDisplay->setColour(0,0,0,0);
    TextDisplay->setDepth(0);
    TextDisplay->setLayer(L_TEXT);
    TextDisplay->setIgnoreCamera(false);
    TextDisplay->setTimeline(FadeInBlack);
    
    return true;
}

void HUDTextDisplay::SetTextDisplay(const char* text)
{
    if(text == NULL)
        return;
    
    if(CIwGameString::CalculateHash(text) != TextHash)
    {
        TextHash = CIwGameString(text).getHash();
        TextDisplay->setText(text);
    }
    if(FadeTimer.getDuration() == 0)
    {
        TextDisplay->setTimeline(FadeInBlack);
        if(FadeInBlack != NULL)
            FadeInBlack->restart();
    }
    
    if(GAME->GetUserOptions()->IsDebugging())
    {
        FadeTimer.setDuration(100000);
        return;
    }
    
    FadeTimer.setDuration(3500);
}

bool HUDTextDisplay::Update(float dt)
{
    ReadPointerPosition();
    
    //Pointer world position
    if(IW_GAME_INPUT->isKeyDown(s3eKeyF1))
    {
        DebugText = "Pointer world position: ";
        DebugText += CIwGameString((int)PointerPosition.x).c_str();
        DebugText += ", ";
        DebugText += CIwGameString((int)PointerPosition.y).c_str();
    }
    //Current memory being used
    if(IW_GAME_INPUT->isKeyDown(s3eKeyF2))
    {
        DebugText = "Memory used: ";
        //DebugText += CIwGameString(s3eMemoryGetInt(S3E_MEMORY_USED)).c_str();
    }
    
    if(IW_GAME_INPUT->getTouchCount() == 1 && IW_GAME_INPUT->getLastKey() <= (s3eKeyF10)
       && IW_GAME_INPUT->getLastKey() >= s3eKeyF1)
        SetTextDisplay(DebugText.c_str());
    
    return CIwGameActorImage::Update(dt);
}

bool HUDTextDisplay::UpdateVisual()
{
    
    if(FadeTimer.HasTimedOut() && (FadeOutBlack != NULL))
    {
        TextDisplay->setTimeline(FadeOutBlack);
        FadeTimer.setDuration(0, false);
        FadeOutBlack->restart();
    }

    return CIwGameActorImage::UpdateVisual();
}
/*
void HUDTextDisplay::DisplayTextInHUD(CIwFVec2 pos)
{
    TextDisplay->setPosition(pos);
}
*/
