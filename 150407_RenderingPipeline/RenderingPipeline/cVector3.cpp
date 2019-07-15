#include "StdAfx.h"
#include "cVector3.h"
#include "cVector4.h"
#include "cMatrix.h"

cVector3::cVector3(void)
{
}

cVector3::cVector3( float _x, float _y, float _z )
	: x(_x)
	, y(_y)
	, z(_z)
{

}


cVector3::~cVector3(void)
{
}

bool cVector3::operator==( cVector3& v )
{
	float fEpsilon = 0.00001f;

	if(fabs(x - v.x) > fEpsilon)
		return false;

	if(fabs(y - v.y) > fEpsilon)
		return false;

	if(fabs(z - v.z) > fEpsilon)
		return false;

	return true;
}

bool cVector3::operator!=( cVector3& v )
{
	return !((*this) == v);
}

cVector3 cVector3::operator+( cVector3& v )
{
	return cVector3(x + v.x, y + v.y, z + v.z);
}

cVector3 cVector3::operator-( cVector3& v )
{
	return cVector3(x - v.x, y - v.y, z - v.z);
}

cVector3 cVector3::operator*( float fScalar )
{
	return cVector3(x * fScalar, y * fScalar, z * fScalar);
}

float cVector3::Length()
{
	return (float)sqrt(x * x + y * y + z * z);
}

cVector3 cVector3::Normalize()
{
	float l = Length();
	return cVector3(x / l, y / l, z / l);
}

float cVector3::Dot( cVector3& v1, cVector3& v2 )
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

cVector3 cVector3::Cross( cVector3& v1, cVector3& v2 )
{
	return cVector3(v1.y * v2.z - v2.y * v1.z, 
		v1.z * v2.x - v2.z * v1.x,
		v1.x * v2.y - v2.x * v1.y);
}

void cVector3::Print()
{
	printf("x : %.2f, y : %.2f, z : %.2f\n", x, y, z);
}

cVector3 cVector3::TransformCoord( cMatrix& mat )
{
	cVector4 v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = 1.0f;

	v = v * mat;
	return v.ToVector3();
}

