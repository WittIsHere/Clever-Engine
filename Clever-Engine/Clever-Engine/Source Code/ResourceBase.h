#pragma once
#include "Globals.h"

#include <string>

enum ResourceTypes
{
    R_EMPTY,
    R_MESH
};

class ResourceBase
{
public:
    ResourceBase(uint32 UID, const std::string& assetsPath, const std::string& libraryPath, const ResourceTypes& type);
    /*ResourceBase(const Resource* resource);*/
    ~ResourceBase();

    bool CleanUp();

public:
    uint32 UID;

    std::string assetsPath;
    std::string libraryPath;

    ResourceTypes type;
};