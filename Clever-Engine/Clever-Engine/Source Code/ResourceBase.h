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

class ResourceBase
{
public:
    ResourceBase(uint32 UID, const std::string& assetsPath, const std::string& libraryPath, const ResourceType& type);
    /*ResourceBase(const Resource* resource);*/
    ~ResourceBase();

    bool CleanUp();

    ResourceType GetType() const;
    const char* GetTypeAsString() const;
    uint32 GetUID() const;
    const char* GetAssetsPath() const;
    const char* GetLibraryPath() const;

public:
    uint32 UID;

    std::string assetsPath;
    std::string libraryPath;

    ResourceType type;
};