#include "JSONParser.h"
#include "Emitter.h"
#include "GameObject.h"

#include "Application.h"
#include "ModuleResources.h"

#include "ResourceParticleSystem.h"

#include "c_ParticleSystem.h"


c_ParticleSystem::c_ParticleSystem(GameObject* owner) : Component(owner, COMPONENT_TYPE::PARTICLE_SYSTEM)
{
	//OPTICK_CATEGORY("C_Particle Constructor", Optick::Category::Debug)
	SetAsDefaultComponent();
}

c_ParticleSystem::~c_ParticleSystem()
{
	emitterInstances.clear();
}

bool c_ParticleSystem::SaveState(ParsonNode& root) const
{
	SaveParticleSystem();

	root.SetNumber("Type", (double)GetType());

	root.SetInteger("particleSystemUID",resource->GetUID());
	root.SetString("particleSystemAssetsPath", resource->GetAssetsPath());

	root.SetBool("stopSpawn", stopSpawn);
	root.SetBool("tempDelete", tempDelete);
	root.SetBool("previewEnabled", previewEnabled);
	root.SetBool("stopAndDeleteCheck", stopAndDeleteCheck);

	return true;
}

bool c_ParticleSystem::LoadState(ParsonNode& root)
{
	uint32 resourceUID = root.GetNumber("particleSystemUID");

	std::string path = root.GetString("particleSystemAssetsPath");
	resource = (ResourceParticleSystem*)App->resources->GetResource(resourceUID);

	stopSpawn			= root.GetBool("stopSpawn");
	tempDelete			= root.GetBool("tempDelete"); 
	previewEnabled		= root.GetBool("previewEnabled");
	stopAndDeleteCheck	= root.GetBool("stopAndDeleteCheck");

	RefreshEmitterInstances();

	return true;
}

bool c_ParticleSystem::Update(float dt)
{
	//OPTICK_CATEGORY("c_ParticleSystem Update", Optick::Category::Update);
	
	for (unsigned int i = 0; i < emitterInstances.size(); ++i)
	{
		emitterInstances[i]->Update(dt);
	}

	for (int i = 0; i < resource->emitters.size(); ++i)
	{
		if (resource->emitters[i].toDelete == true)
		{
			resource->emitters[i].CleanUp();
			resource->emitters.erase(resource->emitters.begin()+i);
			RefreshEmitterInstances();
		}
	}

	if (stopAndDeleteCheck == true)
	{
		InternalStopAndDelete();
	}

	return true;
}

bool c_ParticleSystem::CleanUp()
{
	App->resources->DeleteResource(resource->GetUID());
	resource = nullptr;

	//Clean Emitter Instances
	for (auto emitter = emitterInstances.begin(); emitter != emitterInstances.end(); ++emitter)
	{
		RELEASE((*emitter));
	}

	emitterInstances.clear();

	return true;
}

void c_ParticleSystem::SetParticleSystem(ResourceParticleSystem* newParticleSystem)
{
	CleanUp();
	resource = newParticleSystem;
	RefreshEmitterInstances();
}

void c_ParticleSystem::SetParticleSystem(ResourceBase newParticleSystem)
{
	ResourceParticleSystem* a = (ResourceParticleSystem*)App->resources->GetResource(newParticleSystem.GetUID());

	if (a != nullptr)
	{
		//CleanUp();
		resource = a;
		RefreshEmitterInstances();
	}
	else
	{
		LOG("COuld not find Texture %s for emitter", newParticleSystem.assetsPath.c_str());
	}
}

void c_ParticleSystem::RefreshEmitterInstances()
{
	//OPTICK_CATEGORY("C_Particle RefreshEmitterInstances()", Optick::Category::Debug)

	for (auto emitter = emitterInstances.begin(); emitter != emitterInstances.end(); ++emitter)
	{
		RELEASE (*emitter);
	}

	emitterInstances.clear();

	for (auto emit = resource->emitters.begin(); emit != resource->emitters.end(); ++emit)
	{
		EmitterInstance* emitter = new EmitterInstance(&(*emit),this);
		emitterInstances.push_back(emitter);
	}
}

void c_ParticleSystem::GenerateResourcePS(const char* name) 
{
	//OPTICK_CATEGORY("C_Particle AddParticleSystem()", Optick::Category::Debug)
	std::string assetsPath = ASSETS_PARTICLESYSTEMS_PATH + std::string(name) + PARTICLESYSTEMS_AST_EXTENSION;
	resource = (ResourceParticleSystem*)App->resources->CreateResource(ResourceType::PARTICLE_SYSTEM, assetsPath.c_str(), 0);

	resource->AddNewEmitter();
	RefreshEmitterInstances();
}

void c_ParticleSystem::SaveParticleSystem() const
{
	//App->resources->SaveResourceToLibrary(resource);
	
	//char* buffer = nullptr;
	//Importer::Particles::Save(resource,&buffer);
}

bool c_ParticleSystem::SetAsDefaultComponent()
{
	bool ret = false;
	//OPTICK_CATEGORY("C_Particle SetAsDefaultComponent()", Optick::Category::Debug)
	GenerateResourcePS("Default Particle System");

	//RefreshEmitterInstances(); (it is already done inside AddParticleSystem())

	return ret;
}

void c_ParticleSystem::EnginePreview(bool previewEnabled)
{
	if (this->previewEnabled == previewEnabled)	//if the state of the particle system is different than the requested, clear all and switch mode.
	{
		return;
	}

	ClearEmitters();
	this->previewEnabled = previewEnabled;
}

void c_ParticleSystem::StopSpawn()
{
	for (int i = 0; i < emitterInstances.size(); i++)
	{
		emitterInstances[i]->stopSpawn = true;
	}
}

void c_ParticleSystem::ResumeSpawn()
{
	for (int i = 0; i < emitterInstances.size(); i++)
	{
		emitterInstances[i]->stopSpawn = false;
	}
}

void c_ParticleSystem::StopAndDelete()
{
	stopAndDeleteCheck = true;
	StopSpawn();
}

void c_ParticleSystem::InternalStopAndDelete()
{
	bool stopFinished = true;
	for (int i = 0; i < emitterInstances.size(); i++)
	{
		if (emitterInstances[i]->activeParticles > 0)
		{
			stopFinished = false;	//if all particles have died, just check next component
			break;
		}
	}

	if (stopFinished == true)
	{
		GameObject* owner = this->GetOwner();
		owner->DeleteComponent(this);
	}
}

void c_ParticleSystem::ClearEmitters()
{
	for(int i = 0; i < emitterInstances.size(); i++)
	{
		emitterInstances[i]->KillAll();
	}
}

void c_ParticleSystem::Reset()
{
	//loop all emitterInstances and trigger reset() method
	for (unsigned int i = 0; i < emitterInstances.size(); i++)
	{
		emitterInstances[i]->ResetEmitter();
	}
}



