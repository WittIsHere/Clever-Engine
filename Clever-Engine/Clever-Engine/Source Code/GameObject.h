#pragma once

#include "Globals.h"
#include "Component.h"

#include <vector>


class GameObject
{
public:
	GameObject(char* name, GameObject* parent);
	~GameObject();

	// Methods

	void Init();
	void Update();

	Component* CreateComponent(ComponentType type, GameObject* parent);

	void PrepareDraw();
	void Draw();  // ??

public:

	//GameObject* parent;

	char* name = nullptr;
	bool isActive = true;
	bool toDestroy = false;

private:

	std::vector<Component*> myComponents;
	std::vector<GameObject*> myChilds;
};