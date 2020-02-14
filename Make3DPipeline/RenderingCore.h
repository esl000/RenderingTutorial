#pragma once
#include "CoreInclude.h"

class RenderingCore : public Object
{
	SINGLETON(RenderingCore)
private:
	HDC _memDC, _texDC;
	HBITMAP _bitmap, _oldBitmap, _texOldBitmap;
	float** _depthBuffer;

	Vector3 _eyePos;
public:
	~RenderingCore();

	void Init();
	void Present();

	void DrawGeometry(class Geometry* geo, Matrix4x4& srt);
	void DepthTesting(Vector4& pixel, COLORREF& c);
	void Draw();
};

inline Vector4 GetCircumscribedRectangle(Vector4& v0, Vector4& v1, Vector4& v2)
{
	return Vector4(min(min(v0.x, v1.x), v2.x), min(min(v0.y, v1.y), v2.y),
		max(max(v0.x, v1.x), v2.x), max(max(v0.y, v1.y), v2.y));
}

