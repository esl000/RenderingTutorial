#include "RenderingCore.h"
#include "Geometry.h"

RenderingCore::~RenderingCore()
{
	SelectObject(_memDC, _oldBitmap);
	SelectObject(_texDC, _texOldBitmap);
	DeleteObject(_bitmap);
	DeleteDC(_memDC);
	DeleteDC(_texDC);
}

void RenderingCore::Init()
{
	HDC hdc = GetDC(g_hWnd);

	_memDC = CreateCompatibleDC(hdc);
	_texDC = CreateCompatibleDC(hdc);

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	_bitmap = CreateCompatibleBitmap(_memDC, rc.right, rc.bottom);

	_oldBitmap = (HBITMAP)SelectObject(_memDC, _bitmap);

	ReleaseDC(g_hWnd, hdc);

	_depthBuffer = new float*[rc.right];
	float* depthBuffer = new float[rc.right * rc.bottom];

	for (int i = 0; i < rc.right; ++i)
	{
		_depthBuffer[i] = &depthBuffer[i * rc.bottom];
	}

	_eyePos = Vector3(0.f, 0.f, -10.f);
}

void RenderingCore::Present()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	for (int i = 0; i < rc.right * rc.bottom; ++i)
	{
		_depthBuffer[0][i] = 1.f;
	}
	PatBlt(_memDC, rc.left, rc.top, rc.right, rc.bottom, WHITENESS);
}

void RenderingCore::DrawGeometry(Geometry* geo, Matrix4x4& srt)
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	Matrix4x4 viewMat = GetViewMatrix(Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f), _eyePos)
		, projectionMat = GetPerspectiveMatrix(3.141592f * 0.25f, (float)rc.right / (float)rc.bottom, 1.f, 100.f)
		, viewportMatrix = GetViewPortMatrix(0.f, 0.f, (float)rc.right, (float)rc.bottom, 0.f, 1.f);

	Matrix4x4 renderingMat = srt * viewMat * projectionMat;

	for (int i = 0; i < geo->_indiceCount; i += 3)
	{
		Vector4 v0 = geo->_vertice[geo->_indice[i]] * renderingMat
			, v1 = geo->_vertice[geo->_indice[i + 1]] * renderingMat
			, v2 = geo->_vertice[geo->_indice[i + 2]] * renderingMat;

		v0 = (v0 / v0.w) * viewportMatrix;
		v1 = (v1 / v1.w) * viewportMatrix;
		v2 = (v2 / v2.w) * viewportMatrix;

		Vector4 v01 = v1 - v0;
		Vector4 v02 = v2 - v0;
		Vector4 n = Vector4::CrossProduct(v01, v02);
		float f = Vector4::DotProduct(n, Vector4(0.f, 0.f, 1.f, 0.f));

		if (f < 0)
			continue;

		MoveToEx(_memDC, (int)v0.x, (int)v0.y, NULL);
		LineTo(_memDC, (int)v1.x, (int)v1.y);
		LineTo(_memDC, (int)v2.x, (int)v2.y);
		LineTo(_memDC, (int)v0.x, (int)v0.y);

		Vector4 range = GetCircumscribedRectangle(v0, v1, v2);

		_texOldBitmap = (HBITMAP)SelectObject(_texDC, geo->_tex);

		Vector2 u = Vector2(v01.x, v01.y);
		Vector2 v = Vector2(v02.x, v02.y);

		Vector2 uv0 = geo->_uv[geo->_indice[i]];
		Vector2 uv1 = geo->_uv[geo->_indice[i + 1]];
		Vector2 uv2 = geo->_uv[geo->_indice[i + 2]];

		BITMAP texInfo;
		GetObject(geo->_tex, sizeof(texInfo), &texInfo);

		float vdotv = Vector2::DotProduct(v, v), udotv = Vector2::DotProduct(v, u)
			, udotu = Vector2::DotProduct(u, u);

		float denominator = 1.f / (udotu * vdotv - udotv * udotv);
		
		for (int x = (int)range.x; x <= range.z; ++x)
		{
			for (int y = (int)range.y; y <= range.w; ++y)
			{
				Vector2 w = Vector2(x - v0.x, y - v0.y);

				float wdotu = Vector2::DotProduct(w, u), wdotv = Vector2::DotProduct(w, v);

				// 컨벡스 성질을 이용 삼각형의 무게중심을 구함(뎁스 무시)
				float s = (wdotu * vdotv - wdotv * udotv) * denominator;
				float t = (wdotv * udotu - wdotu * udotv) * denominator;

				if (s >= 0.f && s <= 1.f && t >= 0.f && t <= 1.f && s + t <= 1.f)
				{
					//무게중심의 투영 보정
					float collectedS = (s / v1.z) / ((1.f - s) / v0.z + s / v1.z)
						, collectedT = (t / v2.z) / ((1.f - t) / v0.z + t / v2.z);

					Vector4 pixelPos = (1.f - collectedS - collectedT)
						* v0 + collectedS
						* v1 + collectedT * v2;

					Vector2 textureSamplingCoordnation = (1.f - collectedS - collectedT) 
						* uv0 + collectedS 
						* uv1 + collectedT * uv2;

					int pixelX = (int)(textureSamplingCoordnation.x * texInfo.bmWidth)
						, pixelY = (int)((1.f - textureSamplingCoordnation.y) * texInfo.bmHeight);

					if (pixelX < 0 || pixelX >= texInfo.bmWidth || pixelY < 0 || pixelY >= texInfo.bmHeight)
					{
						int a = 0;
					}

					COLORREF c = GetPixel(_texDC, pixelX, pixelY);
					DepthTesting(pixelPos, c);	
				}
			}
		}

		SelectObject(_texDC, _texOldBitmap);
	}
}

void RenderingCore::DepthTesting(Vector4& pixel, COLORREF & c)
{
	if (_depthBuffer[(int)pixel.x][(int)pixel.y] > pixel.z)
		SetPixel(_memDC, (int)pixel.x, (int)pixel.y, c);
	else
		int a = 0;
}

void RenderingCore::Draw()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	HDC hdc = GetDC(g_hWnd);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, _memDC, 0, 0, SRCCOPY);
	ReleaseDC(g_hWnd, hdc);
}

