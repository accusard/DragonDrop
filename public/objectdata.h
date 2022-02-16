//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// objectdata.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------

#if !defined(_OBJECT_DATA_H_)
#define _OBJECT_DATA_H_

struct ObjectData
{
	CIwGameString	name;
	CIwFVec2		position;
	int				type;
	int				layer;
	const char*		image;
	const char*		shape;
	const char*		material;
	const char*		timeline;
	bool			heavy;
	float			angle_vel;
    
	ObjectData() : name(NULL), position(0,0), type(-1), layer(3), image(NULL), shape(NULL), material(NULL), timeline(NULL), heavy(false), angle_vel(0) {}
};

#endif
