#pragma once

#include "Globals.h"
#include "Component.h"
#include "c_Mesh.h"

#include <vector>


class GameObject
{
public:
	GameObject(char* name, GameObject* parent);
	GameObject(char* name, GameObject* parent, Component* mesh);
	~GameObject();

	// Methods

	void Init();
	void Update();

	Component* CreateComponent(ComponentType type); //empty component with type 

	void AddComponent(Component* copyCmp); //copy an existing "component"

	void PrepareDraw();
	void Draw();  // ??

public:

	char* name = nullptr;
	bool isActive = true;
	bool toDestroy = false;

private:

	GameObject* parent;
	std::vector<GameObject*> myChilds;
	
	std::vector<Component*> myComponents;
};