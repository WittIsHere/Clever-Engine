#ifndef __EMITTER_H__
#define __EMITTER_H__

#include <vector>

#include "ParticleModule.h"

//class R_Texture;
class ParsonNode;
class ResourceBase;
class ResourceTexture;

class Emitter
{
public:
	Emitter();
	Emitter(const char* name);

	//Loop through all modules, loop through particles, update them     
	void Update(float dt);
	void CleanUp();

	//Save and load?
	void Save(ParsonNode& node);
	void Load(ParsonNode& node);

	void SetAsDefault();
	bool AddModuleFromType(ParticleModule::Type type);
	bool DeleteModuleFromType(ParticleModule::Type type);

	void SetParticleCount(int particleCount);

	void SetTexture(ResourceTexture* newTexture);
	void SetTexture(ResourceBase newTexture);

public:
	std::vector<ParticleModule*> modules;
	
	std::string name = "Particle Emitter";
	ResourceTexture* emitterTexture = nullptr;

	std::string path = "None";

	int maxParticleCount = 10;

	bool toDelete = false;
};

#endif // !__EMITTER_H__
