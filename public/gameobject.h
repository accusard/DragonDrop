//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// gameobject.h
//	Programmed by: Vanny Sou Copyright 2018-2022
// 
//--------------------------------------------------------------------------------------------------------

// GameObjects is used to keep track of available inventory per level

struct GameObjects
{
	//objects
	int Treasure;
	int	Stone;
	int	Platform;
	int Dynamite;
	int Trampoline;
	int Candle;

	//tools
	bool hasSword;
	bool hasBubble;
	bool hasUmbrella;
	bool hasBoots;

	GameObjects() : Treasure(0), Stone(0), Platform(0), Dynamite(0), Trampoline(0), Candle(0), 
					hasSword(false), hasBubble(false), hasUmbrella(false), hasBoots(false)
	{}
};

