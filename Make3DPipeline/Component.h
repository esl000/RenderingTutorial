#pragma once
#include "CoreInclude.h"

class Component : public Object
{
public:
	Component();
	~Component();

	virtual void Init();
	virtual void Update();
	virtual void Render();
};

