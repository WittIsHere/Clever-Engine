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
	ResourceType GetType() const;
	const char* GetTypeAsString() const; 
	bool HasForcedUID() const;
	uint32 GetUID() const;
	uint GetReferenceCount() const;

	void ForceUID(uint32 uid);
	void AddReference(int numberOfReferences = 1);

public: //TODO: pasar a private pofavo
	std::string			assetsPath;
	std::string			libraryPath;

private:
	bool hasForcedUID;
	ResourceType		type;
	uint32				UID;
	uint				references;

};