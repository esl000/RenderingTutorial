#include "GameObject.h"
#include "Transform.h"


GameObject::GameObject(string name)
	: _isPendingKill(false)
	, _transform(AddComponent<Transform>())
{
	_name = name;
	Init();
}

GameObject::~GameObject()
{

}

Transform * GameObject::GetTransform()
{
	return _transform;
}

void GameObject::Init()
{
	for (vector<Component*>::iterator iter = _attachedComponents.begin(); iter != _attachedComponents.end(); ++iter)
	{
		(*iter)->Init();
	}
}

void GameObject::Update()
{
	for (vector<Component*>::iterator iter = _attachedComponents.begin(); iter != _attachedComponents.end(); ++iter)
	{
		(*iter)->Update();
	}
}

void GameObject::Render()
{
	for (vector<Component*>::iterator iter = _attachedComponents.begin(); iter != _attachedComponents.end(); ++iter)
	{
		(*iter)->Render();
	}
}

bool GameObject::IsPendingKill() const
{
	return _isPendingKill;
}

void GameObject::Destroy()
{
	_isPendingKill = true;

	for (vector<Component*>::iterator iter = _attachedComponents.begin(); iter != _attachedComponents.end();)
	{
		delete (*iter);
	}

	_attachedComponents.clear();
}
