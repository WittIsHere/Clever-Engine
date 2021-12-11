#pragma once
//Base class for classifying component data 

#include "Globals.h"

enum COMPONENT_TYPE
{
	NONE,
	TRANSFORM,	//necessary?
	MESH,
	MATERIAL,
	CAMERA,
};

class ComponentData
{
public:
	ComponentData(COMPONENT_TYPE cType) : type(cType) {}
public:
	COMPONENT_TYPE type;
	//UID?

};