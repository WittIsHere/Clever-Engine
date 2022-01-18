#include "c_Material.h"
#include "ModuleScene.h"
#include "JSONParser.h"

c_Material::c_Material(GameObject* parent, COMPONENT_TYPE type) : Component(parent, type)
{
	textureData = nullptr;
}

c_Material::c_Material(GameObject* parent, ComponentData* data) : Component(parent, data->type)
{
	textureData = (TextureData*)data;
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

	if (isEmpty == false && textureData != nullptr)
	{
		ParsonNode material = root.SetNode("TextureData");

		material.SetString("Path", textureData->path.c_str());
		material.SetInteger("TextureID", textureData->textureID); 
	}
	return true;
}

bool c_Material::LoadState(ParsonNode& root)
{
	textureData = nullptr;

	ParsonNode textureNode = root.GetNode("TextureData", false);

	if (textureNode.NodeIsValid()) //load data
	{
		TextureData* data = new TextureData;
		data->path = textureNode.GetString("Path");
		data->textureID = textureNode.GetInteger("TextureID");

		//assign data
		this->AssignNewData(data);
	}

	return true;
}

const char* c_Material::getPath()
{
	return textureData->path.c_str();
}

const uint c_Material::getTextureID()
{
	return textureData->textureID;
}

void c_Material::setPath(const char* path)
{
	textureData->path = path;
}

void c_Material::setTextureID(uint id) //does this method make any sense? shouldn't we just change the texturedata instead of the id?
{
	textureData->textureID = id;
}

bool c_Material::AssignNewData(TextureData* data)
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





