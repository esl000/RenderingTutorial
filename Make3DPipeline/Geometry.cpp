#include "Geometry.h"

Geometry::Geometry(Vector4 * vertice, Vector2 * uv, int verticeCount, int * indice, int indiceCount, HBITMAP tex)
	: _vertice(vertice)
	, _uv(uv)
	, _verticeCount(verticeCount)
	, _indice(indice)
	, _indiceCount(indiceCount)
	, _tex(tex)

{
}

Geometry::~Geometry()
{
	delete[] _vertice;
	delete[] _uv;
	delete[] _indice;
	DeleteObject(_tex);
}
