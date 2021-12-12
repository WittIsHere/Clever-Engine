#include "ResourceMesh.h"
#include "Application.h"

ResourceMesh::ResourceMesh() : Resource(ResourceType::MESH)
{

}

ResourceMesh:: ~ResourceMesh()
{

}

bool ResourceMesh::CleanUp()
{
	return false;
}

