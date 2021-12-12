#include "ResourceMesh.h"

#include "Application.h"

#include "Random.h"

Resource::Resource(ResourceType type) :
type(type),
uid(Random::GetRandomUint()),
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

const char* Resource::GetTypeAsString() const
{
	switch (type)
	{
	case ResourceType::MODEL: { return "MODEL"; }				break;
	case ResourceType::MESH: { return "MESH"; }				break;
	case ResourceType::MATERIAL: { return "MATERIAL"; }			break;
	case ResourceType::TEXTURE: { return "TEXTURE"; }			break;
	case ResourceType::ANIMATION: { return "ANIMATION"; }			break;
	case ResourceType::SHADER: { return "SHADER"; }			break;
	case ResourceType::PARTICLE_SYSTEM: { return "PARTICLE_SYSTEM"; }	break;
	case ResourceType::SCRIPT: { return "SCRIPT"; }			break;
	case ResourceType::NAVMESH: { return "NAVMESH"; }			break;
	}

	return "NONE";
}