#include "Application.h"
#include "ResourceBase.h"
#include "ModuleResources.h"


ResourceBase::ResourceBase(uint32 UID, const std::string& assetsPath, const std::string& libraryPath, const ResourceTypes& type)
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
