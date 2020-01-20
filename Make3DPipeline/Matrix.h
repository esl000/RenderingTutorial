#pragma once
#include <cmath>
#include "Vector.h"

struct Matrix4x4;

struct Matrix4x4
{
	float _11, _12, _13, _14,
		_21, _22, _23, _24,
		_31, _32, _33, _34,
		_41, _42, _43, _44;

	const static Matrix4x4 identity;

	Matrix4x4(){}

	Matrix4x4(float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44)
		: _11(_11), _12(_12), _13(_13), _14(_14),
		_21(_21), _22(_22), _23(_23), _24(_24),
		_31(_31), _32(_32), _33(_33), _34(_34),
		_41(_41), _42(_42), _43(_43), _44(_44) {}
};

inline Vector4 operator* (const Matrix4x4 m, const Vector4 v)
{
	return Vector4(
		m._11 * v.x + m._21 * v.y + m._31 * v.z + m._41 * v.w,
		m._12 * v.x + m._22 * v.y + m._32 * v.z + m._42 * v.w,
		m._13 * v.x + m._23 * v.y + m._33 * v.z + m._43 * v.w, 
		m._14 * v.x + m._24 * v.y + m._34 * v.z + m._44 * v.w);
}

inline Vector4 operator* (const Vector4 v, const Matrix4x4 m)
{
	return Vector4(
		m._11 * v.x + m._21 * v.y + m._31 * v.z + m._41 * v.w,
		m._12 * v.x + m._22 * v.y + m._32 * v.z + m._42 * v.w,
		m._13 * v.x + m._23 * v.y + m._33 * v.z + m._43 * v.w,
		m._14 * v.x + m._24 * v.y + m._34 * v.z + m._44 * v.w);
}

inline Matrix4x4 operator * (const Matrix4x4 m1, const Matrix4x4 m2)
{
	return Matrix4x4(
		m1._11 * m2._11 + m1._12 * m2._21 + m1._13 * m2._31 + m1._14 * m2._41,
		m1._11 * m2._12 + m1._12 * m2._22 + m1._13 * m2._32 + m1._14 * m2._42,
		m1._11 * m2._13 + m1._12 * m2._23 + m1._13 * m2._33 + m1._14 * m2._43,
		m1._11 * m2._14 + m1._12 * m2._24 + m1._13 * m2._34 + m1._14 * m2._44,

		m1._21 * m2._11 + m1._22 * m2._21 + m1._23 * m2._31 + m1._24 * m2._41,
		m1._21 * m2._12 + m1._22 * m2._22 + m1._23 * m2._32 + m1._24 * m2._42,
		m1._21 * m2._13 + m1._22 * m2._23 + m1._23 * m2._33 + m1._24 * m2._43,
		m1._21 * m2._14 + m1._22 * m2._24 + m1._23 * m2._34 + m1._24 * m2._44,

		m1._31 * m2._11 + m1._32 * m2._21 + m1._33 * m2._31 + m1._34 * m2._41,
		m1._31 * m2._12 + m1._32 * m2._22 + m1._33 * m2._32 + m1._34 * m2._42,
		m1._31 * m2._13 + m1._32 * m2._23 + m1._33 * m2._33 + m1._34 * m2._43,
		m1._31 * m2._14 + m1._32 * m2._24 + m1._33 * m2._34 + m1._34 * m2._44,

		m1._41 * m2._11 + m1._42 * m2._21 + m1._43 * m2._31 + m1._44 * m2._41,
		m1._41 * m2._12 + m1._42 * m2._22 + m1._43 * m2._32 + m1._44 * m2._42,
		m1._41 * m2._13 + m1._42 * m2._23 + m1._43 * m2._33 + m1._44 * m2._43,
		m1._41 * m2._14 + m1._42 * m2._24 + m1._43 * m2._34 + m1._44 * m2._44
		);
}

inline Matrix4x4 GetTranslationMatrix(float x, float y, float z)
{
	return Matrix4x4(1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		x, y, z, 1.f);
}

inline Matrix4x4 Matrix4x4RotationX(float angle)
{
	float cosVal = cos(angle), sinVal = sin(angle);

	return Matrix4x4(1.f, 0.f, 0.f, 0.f,
		0.f, cosVal, sinVal, 0.f,
		0.f, -sinVal, cosVal, 0.f,
		0.f, 0.f, 0.f, 1.f);
}

inline Matrix4x4 Matrix4x4RotationY(float angle)
{
	float cosVal = cos(angle), sinVal = sin(angle);

	return Matrix4x4(cosVal, 0.f, -sinVal, 0.f,
		0.f, 1.f, 0.f, 0.f,
		sinVal, 0.f, cosVal, 0.f,
		0.f, 0.f, 0.f, 1.f);
}

inline Matrix4x4 Matrix4x4RotationZ(float angle)
{
	float cosVal = cos(angle), sinVal = sin(angle);

	return Matrix4x4(cosVal, sinVal, 0.f, 0.f,
		-sinVal, cosVal, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);
}

//Use Radian Angle
inline Matrix4x4 GetRotationMatrix(float axisX, float axisY, float axisZ)
{
	return Matrix4x4RotationX(axisX)
		* Matrix4x4RotationY(axisY)
		* Matrix4x4RotationZ(axisZ);
}

inline Matrix4x4 GetScaleMatrix(float deltaX, float deltaY, float deltaZ)
{
	return Matrix4x4(deltaX, 0.f, 0.f, 0.f,
		0.f, deltaY, 0.f, 0.f,
		0.f, 0.f, deltaZ, 0.f,
		0.f, 0.f, 0.f, 1.f);
}

inline Matrix4x4 GetViewMatrix(Vector3 u, Vector3 f, Vector3 pos)
{
	u = u.Normalized();
	f = f.Normalized();

	Vector3 r = Vector3::CrossProduct(u, f);
	r = r.Normalized();
	u = Vector3::CrossProduct(f, r).Normalized();

	return Matrix4x4(r.x, u.x, f.x, 0.f,
		r.y, u.y, f.y, 0.f,
		r.z, u.z, f.z, 0.f,
		-Vector3::DotProduct(pos, r), -Vector3::DotProduct(pos, u)
		, -Vector3::DotProduct(pos, f), 1.f);

}

// -1 -1 0, 1 1 1
inline Matrix4x4 GetPerspectiveMatrix(float fovy, float Aspect,
	float zn, float zf)
{
	float yScale = 1.f / tan(fovy * 0.5f);
	float xScale = yScale / Aspect;
	return Matrix4x4(xScale, 0.f, 0.f, 0.f,
		0.f, yScale, 0.f, 0.f,
		0.f, 0.f, zf / (zf - zn), 1.f,
		0.f, 0.f, -(zn * zf) / (zf - zn), 0.f);
}

inline Matrix4x4 GetViewPortMatrix(float x, float y, float width, float height, float minZ, float farZ)
{
	return Matrix4x4(width * 0.5f, 0.f, 0.f, 0.f,
		0.f, -height * 0.5f, 0.f, 0.f,
		0.f, 0.f, farZ - minZ, 0.f,
		(width * 0.5f) + x, y + (height * 0.5f), minZ, 1.f);
}
