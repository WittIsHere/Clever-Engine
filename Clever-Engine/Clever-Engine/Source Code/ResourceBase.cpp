#include "ResourceBase.h"

ResourceBase::ResourceBase(uint32 UID, const std::string& assetsPath, const std::string& libraryPath, const ResourceTypes& type)
{
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
