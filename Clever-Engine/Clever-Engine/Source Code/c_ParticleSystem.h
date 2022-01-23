#ifndef __C_PARTICLESYSTEM_H__
#define __C_PARTICLESYSTEM_H__

#include "Component.h"
#include "EmitterInstance.h"

class ResourceParticleSystem;
class ResourceBase;

class  c_ParticleSystem : public Component
{
public:
	c_ParticleSystem(GameObject* owner);
	~c_ParticleSystem();

	bool Update(float dt) override;
	bool CleanUp();
	
	bool SaveState(ParsonNode& root) const override;
	bool LoadState(ParsonNode& root) override;

	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::PARTICLE_SYSTEM; }				// Required for templated methods such as GetComponent().

public:
	void Reset();

	void SetParticleSystem(ResourceParticleSystem* newParticleSystem);
	void SetParticleSystem(ResourceBase newParticleSystem);

	void RefreshEmitterInstances();

	void GenerateResourcePS(const char* name);
	uint SaveParticleSystem() const;
	bool LoadParticleSystem(ResourceParticleSystem* rParticles, const char* buffer) const;

	bool SetAsDefaultComponent();					//Reset the component, add an emitterInstance to the list and assign it the default emitter
	void AddNewEmitter();							//Create a default emitter

	void EnginePreview(bool previewEnabled);

	void StopSpawn();
	void ResumeSpawn();

	void StopAndDelete();


private:
	void ClearEmitters();
	void InternalStopAndDelete();

public:
	std::vector<EmitterInstance*> emitterInstances; // don't save/load
	
	ResourceParticleSystem* resource = nullptr;			// save/load

	bool stopSpawn = false;
	bool previewEnabled = false;
	bool tempDelete = false;

private:
	bool stopAndDeleteCheck = false;

};

#endif //!__C_PARTICLES_H__
