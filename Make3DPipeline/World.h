#pragma once
#include "CoreInclude.h"

class GameObject;

class World : public Object
{
protected:
	vector<GameObject*> _worldObjects;
public:
	World();
	~World();

	virtual void AddObjectToWorld(GameObject* object);

	virtual void UpdateWorldObjects();
	virtual void RenderWorldObjects();
};

