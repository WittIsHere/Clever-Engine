#pragma once
//Base class for classifying component data 

#include "Globals.h"

enum COMPONENT_TYPE
{
	NONE,
	TRANSFORM,	//necessary?
	MESH,
	MATERIAL,
};

class ComponentData
{
//public:
//	ComponentData();
//	ComponentData(COMPONENT_TYPE cType);
//	~ComponentData();
public:
	COMPONENT_TYPE type;
	//UID?
};