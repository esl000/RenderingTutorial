#pragma once
#include "CoreInclude.h"
class Geometry :
	public Object
{
public:
	Vector4* _vertice;
	Vector2* _uv;

	int* _indice;

	HBITMAP _tex;
	int _verticeCount;
	int _indiceCount;

public:
	Geometry(Vector4* vertice, Vector2* uv, int verticeCount, int* indice, int indiceCount, HBITMAP tex);
	~Geometry();
};

