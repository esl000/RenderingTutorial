#pragma once
#include "Component.h"
class Transform : public Component
{
public:
	Transform();
	~Transform();

	Vector3 Position;
	Vector3 Scale;
};

