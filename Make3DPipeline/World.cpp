#include "World.h"
#include "GameObject.h"

World::World()
{
}


World::~World()
{
}

void World::AddObjectToWorld(GameObject * object)
{
	_worldObjects.push_back(object);
}

void World::UpdateWorldObjects()
{
	for (vector<GameObject*>::iterator iter = _worldObjects.begin(); iter != _worldObjects.end();)
	{
		if ((*iter)->IsPendingKill())
			_worldObjects.erase(iter);
		else
		{
			(*iter)->Update();
			++iter;
		}
	}
}

void World::RenderWorldObjects()
{
	for (vector<GameObject*>::iterator iter = _worldObjects.begin(); iter != _worldObjects.end(); ++iter)
	{
		if (!(*iter)->IsPendingKill())
			(*iter)->Render();
	}
}
