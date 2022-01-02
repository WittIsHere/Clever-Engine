#include "Application.h"
#include "ResourceBase.h"
#include "ModuleResources.h"


ResourceBase::ResourceBase(uint32 UID, const std::string& assetsPath, const std::string& libraryPath, const ResourceType& type)
{
	this->UID = UID;
	this->assetsPath = assetsPath;
	this->libraryPath = libraryPath;
	this->type = type;

}

//ResourceBase::ResourceBase(const Resource* resource)
//{
//}

ResourceBase::~ResourceBase()
{
}

bool ResourceBase::CleanUp()
{
	return false;
}

ResourceType ResourceBase::GetType() const
{
	return type;
}

const char* ResourceBase::GetTypeAsString() const
{
	switch (type)
	{
	case ResourceType::MODEL: { return "MODEL"; } break;
	case ResourceType::MESH: { return "MESH"; }	break;
	case ResourceType::MATERIAL: { return "MATERIAL"; }	break;
	case ResourceType::TEXTURE: { return "TEXTURE"; } break;
	}

	return "NONE";
}

uint32 ResourceBase::GetUID() const
{
	return UID;
}

const char* ResourceBase::GetAssetsPath() const
{
	return assetsPath.c_str();
}

const char* ResourceBase::GetLibraryPath() const
{
	return libraryPath.c_str();
}
