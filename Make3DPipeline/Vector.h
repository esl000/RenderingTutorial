#pragma once
#include <cmath>

struct Vector2
{
	float x, y;
	Vector2() {}
	Vector2(float x, float y) : x(x), y(y) {}

	static float DotProduct(const Vector2& v1, const Vector2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	float Distance()
	{
		return sqrt(x * x + y * y);
	}

	Vector2 Normalized()
	{
		float denominatorDistance = 1.f / Distance();
		return Vector2(x * denominatorDistance,
			y * denominatorDistance);
	}

	void Normalize()
	{
		float denominatorDistance = 1.f / Distance();
		x *= denominatorDistance;
		y *= denominatorDistance;
	}

};

struct Vector3
{
	float x, y, z;
	Vector3() {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}


	const static Vector3 zero;
	const static Vector3 one;

	static float DotProduct(const Vector3& v1, const Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	Vector3 Normalized()
	{
		float denominatorDistance = 1.f / Distance();
		return Vector3(x * denominatorDistance,
			y * denominatorDistance,
			z * denominatorDistance);
	}

	void Normalize()
	{
		float denominatorDistance = 1.f / Distance();
		x *= denominatorDistance;
		y *= denominatorDistance;
		z *= denominatorDistance;
	}

	float Distance()
	{
		return sqrt(x * x + y * y + z * z);
	}

	static Vector3 CrossProduct(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.y * v2.z - v1.z * v2.y
			, v1.z * v2.x - v1.x * v2.z
			, v1.x * v2.y - v1.y * v2.x);
	}
};

struct Vector4
{
	float x, y, z, w;
	Vector4() {}
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Vector4(const Vector3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

	const static Vector4 zero;
	const static Vector4 one;

	//easy call 3d vector operators
	void Normalize()
	{
		(*((Vector3*)this)).Normalize();
	}

	//easy call 3d vector operators
	Vector4 Normalized()
	{
		float denominatorDistance = 1.f / Distance();
		return Vector4(x * denominatorDistance,
			y * denominatorDistance,
			z * denominatorDistance, w);
	}

	//easy call 3d vector operators
	float Distance()
	{
		return (*((Vector3*)this)).Distance();
	}

	Vector3 ToVector3() const
	{
		return Vector3(x, y, z);
	}

	//easy call 3d vector operators
	static float DotProduct(const Vector4& v1, const Vector4& v2)
	{
		return Vector3::DotProduct(*(Vector3*)&v1, *(Vector3*)&v2);
	}

	//easy call 3d vector operators
	//w follow v1 w value
	static Vector4 CrossProduct(const Vector4& v1, const Vector4& v2)
	{
		return  Vector4(v1.y * v2.z - v1.z * v2.y
			, v1.z * v2.x - v1.x * v2.z
			, v1.x * v2.y - v1.y * v2.x, v1.w);
	}
};

inline Vector2 operator +(const Vector2 a, const Vector2 b)
{
	return Vector2(a.x + b.x, a.y + b.y);
}

inline Vector2 operator *(const Vector2 a, const float b)
{
	return Vector2(b * a.x, b * a.y);
}

inline Vector2 operator *(const float a, const Vector2 b)
{
	return Vector2(a * b.x, a * b.y);
}

inline Vector2 operator -(const Vector2 a, const Vector2 b)
{
	return Vector2(a.x - b.x, a.y - b.y);
}

inline Vector2 operator /(const Vector2 a, const float b)
{
	float denominator = 1.f / b;
	return Vector2(denominator * a.x, denominator * a.y);
}

inline Vector2 operator /(const float a, const Vector2 b)
{
	float denominator = 1.f / a;
	return Vector2(denominator * b.x, denominator * b.y);
}


inline Vector3 operator +(const Vector3 a, const Vector3 b)
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector3 operator *(const Vector3 a, const float b)
{
	return Vector3(b * a.x, b * a.y, b * a.z);
}

inline Vector3 operator *(const float a, const Vector3 b)
{
	return Vector3(a * b.x, a * b.y, a * b.z);
}

inline Vector3 operator -(const Vector3 a, const Vector3 b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector3 operator /(const Vector3 a, const float b)
{
	float denominator = 1.f / b;
	return Vector3(denominator * a.x, denominator * a.y, denominator * a.z);
}

inline Vector3 operator /(const float a, const Vector3 b)
{
	float denominator = 1.f / a;
	return Vector3(denominator * b.x, denominator * b.y, denominator * b.z);
}

inline Vector4 operator +(const Vector4 a, const Vector4 b)
{
	return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline Vector4 operator *(const Vector4 a, const float b)
{
	return Vector4(b * a.x, b * a.y, b * a.z, a.w * b);
}

inline Vector4 operator *(const float a, const Vector4 b)
{
	return Vector4(a * b.x, a * b.y, a * b.z, a * b.w);
}

inline Vector4 operator -(const Vector4 a, const Vector4 b)
{
	return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline Vector4 operator /(const Vector4 a, const float b)
{
	float denominator = 1.f / b;
	return Vector4(denominator * a.x, denominator * a.y, denominator * a.z, denominator * a.w);
}

inline Vector4 operator /(const float a, const Vector4 b)
{
	float denominator = 1.f / a;
	return Vector4(denominator * b.x, denominator * b.y, denominator * b.z, denominator * b.w);
}
