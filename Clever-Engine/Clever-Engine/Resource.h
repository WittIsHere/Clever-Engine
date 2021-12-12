#pragma once

#include "Globals.h"
#include <string>

enum class ResourceType					
{
	MODEL,
	MESH,
	MATERIAL,
	TEXTURE,
	NONE
};

class Resource
{
public:

	Resource(ResourceType type);
	virtual ~Resource();

	virtual bool CleanUp();

public:
	const char* GetTypeAsString() const; 

public: //TODO: pasar a private pofavo
	bool hasForcedUID;

	ResourceType		type;

	uint32				uid;
	uint				references;

	std::string			assetsPath;
	std::string			libraryPath;
};