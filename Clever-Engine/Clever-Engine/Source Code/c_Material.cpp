#include "c_Material.h"
#include "ModuleScene.h"
#include "JSONParser.h"
#include "ResourceTexture.h"


c_Material::c_Material(GameObject* parent, COMPONENT_TYPE type) : Component(parent, type)
{
	textureData = nullptr;
}

c_Material::c_Material(GameObject* parent, ResourceTexture* data) : Component(parent, COMPONENT_TYPE::MATERIAL)
{
	textureData = data;
}

c_Material::~c_Material()
{
}

bool c_Material::Enable()
{
	return true;
}

bool c_Material::Update(float dt)
{
	return true;
}

bool c_Material::Disable()
{
	return true;
}

bool c_Material::SaveState(ParsonNode& root) const
{
	root.SetNumber("Type", (uint)type);

	if (textureData != nullptr)
	{
		root.SetNumber("UID", textureData->GetUID());
		root.SetString("assetsPath", textureData->GetAssetsPath());
		root.SetString("libraryPath", textureData->GetLibraryPath());
	}
	return true;
}

bool c_Material::LoadState(ParsonNode& root)
{
	bool ret = true;

	textureData = nullptr;

	std::string assetsPath = std::string(root.GetString("assetsPath"));

	//get the corresponding resorce and allocate it into memory
	textureData = (ResourceTexture*)App->resources->GetResource((uint32)root.GetNumber("UID"));

	if (textureData == nullptr)
	{
		LOG("[ERROR] Loading Scene: Could not find Mesh %s with UID: %u! Try reimporting the model.", root.GetString("File"), (uint32)root.GetNumber("UID"));
	}

	return true;
}

const char* c_Material::getPath()
{
	return textureData->GetAssetsPath();
}

const uint c_Material::getTextureID()
{
	return textureData->GetTextureID();
}

void c_Material::setPath(const char* path)
{
	textureData->assetsPath = path;
}

void c_Material::setTextureID(uint id) //does this method make any sense? shouldn't we just change the texturedata instead of the id?
{
	textureData->SetTextureID(id);
}

bool c_Material::AssignNewData(ResourceTexture* data)
{
	bool ret = true;

	if (this->isEmpty == true && this->textureData == nullptr)
	{
		textureData = data;
	}
	else
	{
		RELEASE(data);
		textureData = data;
	}
	return ret;
}





