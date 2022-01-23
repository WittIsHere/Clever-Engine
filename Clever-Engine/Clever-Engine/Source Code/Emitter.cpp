#include "JSONParser.h"
#include "Application.h"

#include "ResourceBase.h"
#include "ModuleResources.h"
#include "ResourceTexture.h"

#include "ParticleModule.h"

#include "Resource.h"

#include "Emitter.h"


Emitter::Emitter()
{
	std::vector<ResourceBase> textures;
	App->resources->GetResourceBases<ResourceTexture>(textures);
	emitterTexture = (ResourceTexture*)App->resources->GetResource(textures.begin()->UID);
	textures.clear();
}

Emitter::Emitter(const char* name)
{
	std::vector<ResourceBase> textures;
	App->resources->GetResourceBases<ResourceTexture>(textures);
	emitterTexture = (ResourceTexture*)App->resources->GetResource(textures.begin()->UID);
	this->name = name;
}

void Emitter::Update(float dt)
{

	
}

void Emitter::CleanUp()
{
	LOG("EMITTER CLEANUP");

	for (auto mod = modules.begin(); mod != modules.end(); ++mod)
	{
		RELEASE((*mod));
	}

	modules.clear();
	
	if (emitterTexture != nullptr) 
	{
		App->resources->DeleteResource(emitterTexture->GetUID());
		emitterTexture = nullptr;
	}
}

void Emitter::Save(ParsonNode& node)
{
	node.SetString("name", name.c_str());

	uint32 textureUID = (emitterTexture != nullptr) ? emitterTexture->GetUID() : 0;
	node.SetInteger("textureUID", textureUID);
	
	if(emitterTexture != nullptr)
		node.SetString("texturePath", emitterTexture->GetAssetsPath());
	else
		node.SetString("texturePath", path.c_str());
	

	node.SetInteger("maxParticleCount", maxParticleCount);

	ParsonArray modulesArray = node.SetArray("modules");

	for (auto mod = modules.begin(); mod != modules.end(); ++mod)
	{
		ParsonNode arrayNode = modulesArray.SetNode("module");
		(*mod)->Save(arrayNode);
	}
}

void Emitter::Load(ParsonNode& node)
{
	name = node.GetString("name");
	
	uint32 textureUID = node.GetInteger("textureUID");
	emitterTexture = (ResourceTexture*)App->resources->GetResource(textureUID);

	maxParticleCount = node.GetInteger("maxParticleCount");

	ParsonArray modulesArray = node.GetArray("modules");

	for (uint i = 0 ; i < modulesArray.size ; ++i)
	{
		//(*mod)->Save(modulesArray.SetNode("module"));
		ParsonNode mod = modulesArray.GetNode(i);

		ParticleModule* particleModule = nullptr;

		ParticleModule::Type a = (ParticleModule::Type)mod.GetInteger("Type");

		switch (a)
		{
			case ParticleModule::Type::EMITTER_BASE: particleModule = new EmitterBase(); break;
			case ParticleModule::Type::EMITTER_SPAWN: particleModule = new EmitterSpawn(); break;
			case ParticleModule::Type::PARTICLE_LIFETIME: particleModule = new ParticleLifetime(); break;
			case ParticleModule::Type::PARTICLE_MOVEMENT: particleModule = new ParticleMovement(); break;
			case ParticleModule::Type::PARTICLE_COLOR: particleModule = new ParticleColor(); break;
			case ParticleModule::Type::EMITTER_AREA: particleModule = new EmitterArea(); break;
			case ParticleModule::Type::PARTICLE_SIZE: particleModule = new ParticleSize(); break;
			case ParticleModule::Type::PARTICLE_BILLBOARDING: particleModule = new ParticleBillboarding(); break;
		}

		if (particleModule != nullptr)
		{
			particleModule->Load(mod);
			modules.push_back(particleModule);
		}
		else
		{
			LOG("Could not load particle module with type: %d", (int)a);
		}
	}
}

void Emitter::SetAsDefault()
{
	AddModuleFromType(ParticleModule::Type::EMITTER_BASE);
	AddModuleFromType(ParticleModule::Type::EMITTER_SPAWN);
	AddModuleFromType(ParticleModule::Type::PARTICLE_LIFETIME);
	AddModuleFromType(ParticleModule::Type::PARTICLE_MOVEMENT);
	/*
	AddModuleFromType(ParticleModule::Type::EMITTER_AREA);
	AddModuleFromType(ParticleModule::Type::PARTICLE_COLOR);
	AddModuleFromType(ParticleModule::Type::PARTICLE_SIZE);
	AddModuleFromType(ParticleModule::Type::PARTICLE_BILLBOARDING);
	*/
}

bool Emitter::AddModuleFromType(ParticleModule::Type type)
{
	//switch (type) and in each case a type of particlemodule to add to the emitter.
	for (int i = 0; i < modules.size(); i++)
	{
		if (modules[i]->type == type)
		{
			return false;
		}
	}

	switch (type)
	{
		case (ParticleModule::Type::EMITTER_BASE):
			modules.push_back(new EmitterBase); 
			break;
		case (ParticleModule::Type::EMITTER_SPAWN):
			modules.push_back(new EmitterSpawn);
			break;
		case (ParticleModule::Type::EMITTER_AREA):
			modules.push_back(new EmitterArea);
			break;
		case(ParticleModule::Type::PARTICLE_COLOR):
			modules.push_back(new ParticleColor);
			break;	
		case(ParticleModule::Type::PARTICLE_LIFETIME):
			modules.push_back(new ParticleLifetime);
			break;
		case(ParticleModule::Type::PARTICLE_MOVEMENT):
			modules.push_back(new ParticleMovement);
			break;
		case(ParticleModule::Type::PARTICLE_SIZE):
			modules.push_back(new ParticleSize);
			break;
		case(ParticleModule::Type::PARTICLE_BILLBOARDING):
			modules.push_back(new ParticleBillboarding);
			break;
		case(ParticleModule::Type::NONE):
			return false;
	}
	return true;

}

bool Emitter::DeleteModuleFromType(ParticleModule::Type type)
{
	//switch (type) and in each case a type of particlemodule to add to the emitter.
	for (int i = 0; i < modules.size(); i++)
	{
		if (modules[i]->type == type)
		{
			modules.erase(modules.begin() + i);
			modules.shrink_to_fit();
		}
	}
	return true;
}

void Emitter::SetParticleCount(int particleCount)
{
	maxParticleCount = particleCount;
}

void Emitter::SetTexture(ResourceTexture* newTexture)
{
	if (newTexture != nullptr)
	{
		App->resources->DeleteResource(emitterTexture->GetUID());
		emitterTexture = newTexture;
	}
	else
	{
		LOG("COuld not find Texture %s for emitter", newTexture->assetsPath.c_str());
	}
}

void Emitter::SetTexture(ResourceBase newTexture)
{
	
	ResourceTexture* a = (ResourceTexture*)App->resources->GetResource(newTexture.GetUID());

	if (a != nullptr)
	{
		if(emitterTexture !=nullptr)
			App->resources->DeleteResource(emitterTexture->GetUID());

		emitterTexture = a;
	}
	else
	{
		LOG("COuld not find Texture %s for emitter", newTexture.assetsPath.c_str());
	}

	
}
