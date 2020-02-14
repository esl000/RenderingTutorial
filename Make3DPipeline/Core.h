#pragma once
#include "CoreInclude.h"

class Core : public Object
{
	SINGLETON(Core)
public:
	~Core();

	void Init();
	void Update();
	void Render();
};

