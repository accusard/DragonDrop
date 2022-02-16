//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// debug_actor_text.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#if !defined(_DEBUG_ACTOR_TEXT_H_)
#define _DEBUG_ACTOR_TEXT_H_

#include "IwGame.h"
#include "IwGameActor.h"

#include "game.h"

class HUDTextDisplay : public CIwGameActorImage
{
protected:
	CIwFVec2				PointerPosition;
	CIwGameActorText*       TextDisplay;
    CIwGameString           DebugText;
    unsigned int            OldTextHash;
    unsigned int            TextHash;
    float                   Fade;
	bool                    RestartFading;
    CIwGameAnimTimeline*    FadeInBlack;
    CIwGameAnimTimeline*    FadeOutBlack;
    
    CIwGameTimer            FadeTimer;

public:
	HUDTextDisplay() : CIwGameActorImage(), PointerPosition(0,0), TextDisplay(NULL), DebugText(""), OldTextHash(0), TextHash(0), Fade(1.0), RestartFading(false), FadeInBlack(NULL), FadeOutBlack(NULL), FadeTimer(0) {}
    
	~HUDTextDisplay()
    {
    };

    bool            Update(float dt);
    bool            UpdateVisual();
    
    bool                InitTextDisplay(CIwGameFont* font, CIwGameString& text);
    void                SetTextDisplay(const char* text);
    CIwGameActorText*   GetTextDisplay() const { return TextDisplay; }
    unsigned int        GetText() { return TextHash; }
	void                ReadPointerPosition();									// read and set the mouse position
	CIwFVec2            GetPointerPosition() const{ return PointerPosition; }	// get MousePosition
    void                SetPointerPosition(float x, float y);
    //void                DisplayTextInHUD(CIwFVec2 pos);
    CIwGameTimer        GetFadeTimer() { return FadeTimer; }
    
    void            SetFade(float fade);
    float           GetFade() const {return Fade;}

};

 

#endif
