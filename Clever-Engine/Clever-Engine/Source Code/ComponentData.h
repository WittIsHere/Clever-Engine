#pragma once
//Base class for classifying component data 

#include "Globals.h"

enum DATA_TYPE
{
	NONE,
	TRANSFORM,	//necessary?
	MESH,
	MATERIAL,
};

class ComponentData
{
public:
	DATA_TYPE type = DATA_TYPE::NONE;
	//UID?
};