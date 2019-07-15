#pragma once

#include "cVector3.h"

class cMatrix;

class cVector4
{
public:
	cVector4(void);
	cVector4(float _x, float _y, float _z, float _w);
	~cVector4(void);

	float x, y, z, w;
	cVector4 operator*(cMatrix& mat);
	cVector3 ToVector3();
};

