#pragma once
#include "CoreInclude.h"

class InputManager : public Object
{
	SINGLETON(InputManager)
public:
	~InputManager();



	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

