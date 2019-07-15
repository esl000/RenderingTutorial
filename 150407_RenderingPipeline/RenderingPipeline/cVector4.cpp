#include "StdAfx.h"
#include "cVector4.h"
#include "cMatrix.h"

cVector4::cVector4(void)
{
}

cVector4::cVector4( float _x, float _y, float _z, float _w )
	: x(_x)
	, y(_y)
	, z(_z)
	, w(_w)
{

}


cVector4::~cVector4(void)
{
}

cVector4 cVector4::operator*( cMatrix& mat )
{
	cVector4 v;
	v.x = x * mat[0][0] + y * mat[1][0] + z * mat[2][0] + w * mat[3][0];
	v.y = x * mat[0][1] + y * mat[1][1] + z * mat[2][1] + w * mat[3][1];
	v.z = x * mat[0][2] + y * mat[1][2] + z * mat[2][2] + w * mat[3][2];
	v.w = x * mat[0][3] + y * mat[1][3] + z * mat[2][3] + w * mat[3][3];

	if(fabs(v.w) > 0.0001f)
	{
		v.x /= v.w;
		v.y /= v.w;
		v.z /= v.w;
		v.w /= v.w;
	}
	return v;
}

cVector3 cVector4::ToVector3()
{
	cVector3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}
