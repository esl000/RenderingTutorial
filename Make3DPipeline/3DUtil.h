#pragma once
#include <cmath>

struct Vector3
{
	float x, y, z;

	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}


	const static Vector3 zero;
	const static Vector3 one;

	Vector3 operator +(const Vector3 other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 operator *(const float constantVal) const
	{
		return Vector3(constantVal * x, constantVal * y, constantVal * z);
	}

	Vector3 operator -(const Vector3 other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	static float DotProduct(const Vector3 v1, const Vector3 v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	Vector3 Normalized()
	{
		return (*this) / Distance();
	}

	float Distance()
	{
		return sqrt(x * x + y * y + z * z);
	}

	static Vector3 CrossProduct(const Vector3 v1, const Vector3 v2)
	{
		return Vector3(v1.y * v2.z - v1.z * v2.y
			, v1.z * v2.x - v1.x * v2.z
			, v1.x * v2.y - v1.y * v2.x);
	}

	Vector3 operator/ (float a)
	{
		return (*this) * (1.f / a);
	}
};

static Vector3 operator * (float a, Vector3 b)
{
	return Vector3(a * b.x, a * b.y, a * b.z);
}

static Vector3 operator / (float a, Vector3 b)
{
	return b * a;
}

struct Vector4
{
	float x, y, z, w;

	Vector4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Vector4(Vector3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}


	const static Vector4 zero;
	const static Vector4 one;

	Vector4 operator +(const Vector4 other) const
	{
		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Vector4 operator *(const float constantVal) const
	{
		return Vector4(constantVal * x, constantVal * y, constantVal * z, constantVal * w);
	}

	Vector4 operator -(const Vector4 other) const
	{
		return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	Vector3 ToVector3()
	{
		return Vector3(x, y, z);
	}

};

static Vector4 operator * (float constantVal, Vector4 v)
{
	return Vector4(constantVal * v.x, constantVal * v.y, constantVal * v.z, constantVal * v.w);
}

struct Matrix4x4
{
	float _11, _12, _13, _14,
		_21, _22, _23, _24,
		_31, _32, _33, _34,
		_41, _42, _43, _44;

	const static Matrix4x4 identity;

	Matrix4x4() : _11(0.f), _12(0.f), _13(0.f), _14(0.f),
		_21(0.f), _22(0.f), _23(0.f), _24(0.f),
		_31(0.f), _32(0.f), _33(0.f), _34(0.f),
		_41(0.f), _42(0.f), _43(0.f), _44(0.f) {}

	Matrix4x4(float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44) 
		: _11(_11), _12(_12), _13(_13), _14(_14),
		_21(_21), _22(_22), _23(_23), _24(_24),
		_31(_31), _32(_32), _33(_33), _34(_34),
		_41(_41), _42(_42), _43(_43), _44(_44) {}

	Vector4 operator* (const Vector4 v) const
	{
		return Vector4(PartMul((float*)&v, ToPoint(), 0, 0),
			PartMul((float*)&v, ToPoint(), 0, 1), 
			PartMul((float*)&v, ToPoint(), 0, 2), 
			PartMul((float*)&v, ToPoint(), 0, 3));
	}

	Matrix4x4 operator * (const Matrix4x4 m) const
	{
		return Matrix4x4(PartMul(ToPoint(), m.ToPoint(), 0, 0)
			, PartMul(ToPoint(), m.ToPoint(), 0, 1)
			, PartMul(ToPoint(), m.ToPoint(), 0, 2)
			, PartMul(ToPoint(), m.ToPoint(), 0, 3)
			, PartMul(ToPoint(), m.ToPoint(), 1, 0)
			, PartMul(ToPoint(), m.ToPoint(), 1, 1)
			, PartMul(ToPoint(), m.ToPoint(), 1, 2)
			, PartMul(ToPoint(), m.ToPoint(), 1, 3)
			, PartMul(ToPoint(), m.ToPoint(), 2, 0)
			, PartMul(ToPoint(), m.ToPoint(), 2, 1)
			, PartMul(ToPoint(), m.ToPoint(), 2, 2)
			, PartMul(ToPoint(), m.ToPoint(), 2, 3)
			, PartMul(ToPoint(), m.ToPoint(), 3, 0)
			, PartMul(ToPoint(), m.ToPoint(), 3, 1)
			, PartMul(ToPoint(), m.ToPoint(), 3, 2)
			, PartMul(ToPoint(), m.ToPoint(), 3, 3));
	}


	static const Matrix4x4 GetTranslationMatrix(float x, float y, float z)
	{
		return Matrix4x4(1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			x, y, z, 1.f);
	}

	//Use Radian Angle
	static const Matrix4x4 GetRotationMatrix(float axisX, float axisY, float axisZ)
	{
		return Matrix4x4RotationX(axisX) 
			* Matrix4x4RotationY(axisY) 
			* Matrix4x4RotationZ(axisZ);
	}

	static const Matrix4x4 Matrix4x4RotationX(float angle)
	{
		float cosVal = cos(angle), sinVal = sin(angle);

		return Matrix4x4(1.f, 0.f, 0.f, 0.f,
			0.f, cosVal, sinVal, 0.f,
			0.f, -sinVal, cosVal, 0.f,
			0.f, 0.f, 0.f, 1.f);
	}

	static const Matrix4x4 Matrix4x4RotationY(float angle)
	{
		float cosVal = cos(angle), sinVal = sin(angle);

		return Matrix4x4(cosVal, 0.f, -sinVal, 0.f,
			0.f, 1.f, 0.f, 0.f,
			sinVal, 0.f, cosVal, 0.f,
			0.f, 0.f, 0.f, 1.f);
	}

	static const Matrix4x4 Matrix4x4RotationZ(float angle)
	{
		float cosVal = cos(angle), sinVal = sin(angle);

		return Matrix4x4(cosVal, sinVal, 0.f, 0.f,
			-sinVal, cosVal, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f);
	}

	static const Matrix4x4 GetScaleMatrix(float deltaX, float deltaY, float deltaZ)
	{
		return Matrix4x4(deltaX, 0.f, 0.f, 0.f,
			0.f, deltaY, 0.f, 0.f,
			0.f, 0.f, deltaZ, 0.f,
			0.f, 0.f, 0.f, 1.f);
	}

	static const Matrix4x4 GetViewMatrix(Vector3 u, Vector3 f, Vector3 pos)
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
	static Matrix4x4 GetPerspectiveMatrix(float fovy, float Aspect,
		float zn, float zf)
	{
		float yScale = 1.f / tan(fovy * 0.5f);
		float xScale = yScale / Aspect;
		return Matrix4x4(xScale, 0.f, 0.f, 0.f,
			0.f, yScale, 0.f, 0.f,
			0.f, 0.f, zf / (zf - zn), 1.f,
			0.f, 0.f, - (zn * zf) / (zf - zn), 0.f);
	}

	static Matrix4x4 GetViewPortMatrix(float x, float y, float width, float height, float minZ, float farZ)
	{
		return Matrix4x4(width * 0.5f, 0.f, 0.f, 0.f,
			0.f, -height * 0.5f, 0.f, 0.f,
			0.f, 0.f, farZ - minZ, 0.f,
			(width * 0.5f) + x, y + (height * 0.5f), minZ, 1.f);
	}

	// w/2 * x + w/2 , h/2 - h/2 * y, (fz- mz) * z + mz


private :
	float* ToPoint() const { return (float*)this; }

	inline const static float PartMul(float* part1, float* part2, int curPart1Row, int curPart2Column)
	{
		float retVal = 0;

		for (int i = 0; i < 4; ++i)
		{
			retVal += part1[4 * curPart1Row + i] * part2[curPart2Column + 4 * i];
		}
		return retVal;
	}
};

