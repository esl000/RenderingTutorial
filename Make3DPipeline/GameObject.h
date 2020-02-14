#pragma once
#include "CoreInclude.h"
#include "Transform.h"

class GameObject : public Object
{
protected:
	bool _isPendingKill;
	Transform* _transform;

	vector<Component*> _attachedComponents;
public:
	GameObject(string name);
	~GameObject();

	virtual Transform* GetTransform();

	template<typename ComponentSubClass>
	ComponentSubClass* AddComponent();

	virtual void Init();
	virtual void Update();
	virtual void Render();

	virtual bool IsPendingKill() const;
	virtual void Destroy();
};

template<typename ComponentSubClass>
inline ComponentSubClass* GameObject::AddComponent()
{
	ComponentSubClass* comp = new ComponentSubClass();
	if (CastCheck<Component>(comp))
	{
		_attachedComponents.push_back(comp);
		comp->Init();
		return comp;
	}

	return nullptr;
}
