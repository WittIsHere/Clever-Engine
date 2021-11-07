#pragma once

#include "Globals.h"
#include "Component.h"

struct meshData;

class c_Mesh : public Component
{
public:
	c_Mesh();
	~c_Mesh();

	bool Enable();
	bool Update();
	bool Disable();


public:
	meshData* resourceMesh;
};