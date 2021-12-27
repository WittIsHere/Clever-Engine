#include "Resource.h"

#include "Application.h"

#include "Random.h"

Resource::Resource(ResourceType type) :
type(type),
UID(Random::GetRandomUint()),
references(0),
assetsPath("[NONE]"),
libraryPath("[NONE]"),
hasForcedUID(false)
{
		
}

Resource::~Resource()
{

}

bool Resource::CleanUp()
{
	bool ret = true;

	assetsPath.clear();
	libraryPath.clear();

	assetsPath.shrink_to_fit();
	libraryPath.shrink_to_fit();

	return ret;
}

ResourceType Resource::GetType() const
{
	return type;
}

const char* Resource::GetTypeAsString() const
{
	switch (type)
	{
	case ResourceType::MODEL:		{ return "MODEL"; } break;
	case ResourceType::MESH:		{ return "MESH"; }	break;
	case ResourceType::MATERIAL:	{ return "MATERIAL"; }	break;
	case ResourceType::TEXTURE:		{ return "TEXTURE"; } break;
	}

	return "NONE";
}

bool Resource::HasForcedUID() const
{
	return hasForcedUID;
}

uint32 Resource::GetUID() const
{
	return UID;
}

uint Resource::GetReferenceCount() const
{
	return references;
}

void Resource::ForceUID(uint32 uid)
{
	UID = uid;
	hasForcedUID = true;
}

void Resource::AddReference(int numberOfReferences)
{
	references += numberOfReferences;
}
