#include "JSONParser.h"

#include "ParticleModule.h"

#include "ResourceParticleSystem.h"

ResourceParticleSystem::ResourceParticleSystem() : Resource(ResourceType::PARTICLE_SYSTEM)
{

}

ResourceParticleSystem::~ResourceParticleSystem()
{

}

bool ResourceParticleSystem::CleanUp()
{
	LOG("R_PARTICLESYSTEM CLEANUP");
	
	for (auto emitter = emitters.begin(); emitter != emitters.end(); ++emitter)
	{
		(*emitter).CleanUp();
	}

	emitters.clear();

	return true;
}

bool ResourceParticleSystem::SaveMeta(ParsonNode& metaRoot) const
{
	ParsonArray contained_array = metaRoot.SetArray("ContainedResources");


	return true;
}

bool ResourceParticleSystem::LoadMeta(const ParsonNode& metaRoot)
{
	return true;
}

void ResourceParticleSystem::AddNewEmitter()
{
	//OPTICK_CATEGORY("C_Particle AddNewEmitter()", Optick::Category::Debug);

	emitters.push_back(Emitter());

	emitters.back().modules.push_back(new EmitterBase());
	emitters.back().modules.push_back(new EmitterSpawn());
	//emitters.back().modules.push_back(new ParticleColor());
	emitters.back().modules.push_back(new ParticleLifetime());
}

void ResourceParticleSystem::AddNewEmitter(const char* name)
{
	//OPTICK_CATEGORY("C_Particle AddNewEmitter()", Optick::Category::Debug);
	
	emitters.push_back(Emitter(name));

	emitters.back().modules.push_back(new EmitterBase());
	emitters.back().modules.push_back(new EmitterSpawn());
	//emitters.back().modules.push_back(new ParticleColor());
	emitters.back().modules.push_back(new ParticleLifetime());
}

//void R_ParticleSystem::DeleteEmitter()
//{
//
//}
