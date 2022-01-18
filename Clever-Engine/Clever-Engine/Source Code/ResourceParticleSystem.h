#ifndef __R_PARTICLE_SYSTEM_H__
#define __R_PARTICLE_SYSTEM_H__
 
#include "Resource.h"
#include "Emitter.h"
#include "ComponentData.h"

class ResourceParticleSystem : public Resource
{
public:
	ResourceParticleSystem();
	~ResourceParticleSystem();

	bool CleanUp() override;

	bool SaveMeta(ParsonNode& metaRoot) const;
	bool LoadMeta(const ParsonNode& metaRoot);

	static inline ResourceType GetType() { return ResourceType::PARTICLE_SYSTEM; }

public:
	void AddNewEmitter();																			//default emitter creation
	void AddNewEmitter(const char* name);

public:
	std::vector<Emitter> emitters;
	std::string name = "Default Name";
};

#endif //!__R_PARTICLE_SYSTEM_H__
