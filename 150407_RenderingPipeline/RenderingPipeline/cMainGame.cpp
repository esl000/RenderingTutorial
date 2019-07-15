#include "StdAfx.h"
#include "cMainGame.h"
#include "cVector4.h"

cMainGame::cMainGame(void)
	: m_MemDC(NULL)
	, m_hOldBitmap(NULL)
	, m_hBitmap(NULL)
	, m_vEye(0.f, 0.f, -1.f)
	, m_vLookAt(0.f, 0.f, 0.f)
	, m_vUp(0.f, 1.f, 0.f)
	, m_vPosition(0.f, 0.f, 0.f)
	, m_vDirection(0.f, 0.f, 1.f)
	, m_fSpeed(0.1f)
	, m_fAngle(0.0f)
	, m_isLButtonDown(false)
	, m_fRotAngleX(0.f)
	, m_fRotAngleY(0.f)
	, m_fDistance(15.f)
{
	m_ptPrev.x = 0;
	m_ptPrev.y = 0;
}


cMainGame::~cMainGame(void)
{
	SelectObject(m_MemDC, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_MemDC);
}

void cMainGame::Setup()
{
	HDC hdc = GetDC(g_hWnd);

	m_MemDC = CreateCompatibleDC(hdc);

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);

	// SelectObject : 리턴값으로 이전의 객체를 반환
	m_hOldBitmap = (HBITMAP)SelectObject(m_MemDC, m_hBitmap);

	ReleaseDC(g_hWnd, hdc);

	// 정육면체 버텍스 데이터 생성
	m_vecVertex.push_back(cVector3(-1.f, -1.f, -1.f));
	m_vecVertex.push_back(cVector3(-1.f,  1.f, -1.f));
	m_vecVertex.push_back(cVector3( 1.f,  1.f, -1.f));
	m_vecVertex.push_back(cVector3( 1.f, -1.f, -1.f));
	m_vecVertex.push_back(cVector3(-1.f, -1.f,  1.f));
	m_vecVertex.push_back(cVector3(-1.f,  1.f,  1.f));
	m_vecVertex.push_back(cVector3( 1.f,  1.f,  1.f));
	m_vecVertex.push_back(cVector3( 1.f, -1.f,  1.f));

	m_vecIndex.push_back(0); m_vecIndex.push_back(1); m_vecIndex.push_back(2);
	m_vecIndex.push_back(0); m_vecIndex.push_back(2); m_vecIndex.push_back(3);
	m_vecIndex.push_back(4); m_vecIndex.push_back(6); m_vecIndex.push_back(5);
	m_vecIndex.push_back(4); m_vecIndex.push_back(7); m_vecIndex.push_back(6);
	m_vecIndex.push_back(4); m_vecIndex.push_back(5); m_vecIndex.push_back(1);
	m_vecIndex.push_back(4); m_vecIndex.push_back(1); m_vecIndex.push_back(0);
	m_vecIndex.push_back(3); m_vecIndex.push_back(2); m_vecIndex.push_back(6);
	m_vecIndex.push_back(3); m_vecIndex.push_back(6); m_vecIndex.push_back(7);
	m_vecIndex.push_back(1); m_vecIndex.push_back(5); m_vecIndex.push_back(6);
	m_vecIndex.push_back(1); m_vecIndex.push_back(6); m_vecIndex.push_back(2);
	m_vecIndex.push_back(4); m_vecIndex.push_back(0); m_vecIndex.push_back(3);
	m_vecIndex.push_back(4); m_vecIndex.push_back(3); m_vecIndex.push_back(7);

	// 바닥 버텍스 생성
	int nHalf = 5;
	float fInterval = 1.0f;
	float fMax = nHalf * fInterval;
	float fMin = -fMax;

	for (int i = 1; i <= nHalf; ++i)
	{
		m_vecLineVertex.push_back(cVector3(fMin, 0.f, fInterval * i));
		m_vecLineVertex.push_back(cVector3(fMax, 0.f, fInterval * i));

		m_vecLineVertex.push_back(cVector3(fMin, 0.f, -fInterval * i));
		m_vecLineVertex.push_back(cVector3(fMax, 0.f, -fInterval * i));

		m_vecLineVertex.push_back(cVector3(fInterval * i, 0.f, fMin));
		m_vecLineVertex.push_back(cVector3(fInterval * i, 0.f, fMax));

		m_vecLineVertex.push_back(cVector3(-fInterval * i, 0.f, fMin));
		m_vecLineVertex.push_back(cVector3(-fInterval * i, 0.f, fMax));
	}

	m_vecLineVertex.push_back(cVector3(fMin, 0.f, 0.f));
	m_vecLineVertex.push_back(cVector3(fMax, 0.f, 0.f));

	m_vecLineVertex.push_back(cVector3(0.f, 0.f, fMin));
	m_vecLineVertex.push_back(cVector3(0.f, 0.f, fMax));

	// 변환행렬 생성
	m_matWorld = cMatrix::Translate(m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matView = cMatrix::View(&m_vEye, &m_vLookAt, &m_vUp);
	m_matProjection = cMatrix::Projection((float)PI / 4.f, rc.right / (float)rc.bottom, 1.f, 100.f);
	m_matViewport = cMatrix::Viewport(0.f, 0.f, (float)rc.right, (float)rc.bottom, 0.f, 1.f);
}

void cMainGame::Update()
{
	// 회전 각도 구하기
	if(GetKeyState('A') & 0x8000)
	{
		m_fAngle -= 0.1f;
	}
	if(GetKeyState('D') & 0x8000)
	{
		m_fAngle += 0.1f;
	}

	// 각도 업데이트 후 회전 행렬 구하기
	cMatrix matR = cMatrix::RotateY(m_fAngle);
	
	// 회전 행렬에 의한 방향 벡터 구하기
	cVector4 v(0.f, 0.f, 1.f, 0.f);
	
	v = v * matR;

	m_vDirection.x = v.x;
	m_vDirection.y = v.y;
	m_vDirection.z = v.z;

	// 방향에 따른 전 후 이동 좌표 구하기
	if(GetKeyState('W') & 0x8000)
	{
		m_vPosition = m_vPosition + m_vDirection * m_fSpeed;
	}
	if(GetKeyState('S') & 0x8000)
	{
		m_vPosition = m_vPosition - m_vDirection * m_fSpeed;
	}

	// 좌표에 따른 이동 행렬 구하기
	cMatrix matT = cMatrix::Translate(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	// 월드 행렬 구하기.
	m_matWorld = matR * matT;

	// 카메라 회전 및 큐브의 위치에 따른 뷰 매트릭스 갱신
	cMatrix matRotX = cMatrix::RotateX(m_fRotAngleX);
	cMatrix matRotY = cMatrix::RotateY(m_fRotAngleY);
	cMatrix matCamRot = matRotX * matRotY;
	m_vEye = cVector3(0.f, 0.f, -m_fDistance);
	m_vEye = m_vEye.TransformCoord(matCamRot);
	m_vEye = m_vEye + m_vPosition;
	m_vLookAt = m_vPosition;
	m_matView = cMatrix::View(&m_vEye, &m_vLookAt, &m_vUp);
}

void cMainGame::Render( HDC hdc )
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	// 도화지를 흰색으로
	PatBlt(m_MemDC, rc.left, rc.top, rc.right, rc.bottom, WHITENESS);

	// 박스 점들 변환
	cMatrix matWVP = m_matWorld * m_matView * m_matProjection;
	std::vector<cVector4> vecScreenVertex(m_vecVertex.size());
	for (size_t i = 0; i < m_vecVertex.size(); ++i)
	{
		vecScreenVertex[i].x = m_vecVertex[i].x;
		vecScreenVertex[i].y = m_vecVertex[i].y;
		vecScreenVertex[i].z = m_vecVertex[i].z;
		vecScreenVertex[i].w = 1.f;
		vecScreenVertex[i] = vecScreenVertex[i] * matWVP;
	}

	// 박스 그리기
	for (size_t i = 0; i < m_vecIndex.size(); i += 3)
	{
		cVector4 v0 = vecScreenVertex[m_vecIndex[i]];
		cVector4 v1 = vecScreenVertex[m_vecIndex[i + 1]];
		cVector4 v2 = vecScreenVertex[m_vecIndex[i + 2]];

		cVector3 v01 = v1.ToVector3() - v0.ToVector3();
		cVector3 v02 = v2.ToVector3() - v0.ToVector3();
		cVector3 n = cVector3::Cross(v01, v02);
		float f = cVector3::Dot(n, cVector3(0.f, 0.f, 1.f));

		if(f > 0)
			continue;

		v0 = v0 * m_matViewport;
		v1 = v1 * m_matViewport;
		v2 = v2 * m_matViewport;

		MoveToEx(m_MemDC, (int)v0.x, (int)v0.y, NULL);
		LineTo(m_MemDC, (int)v1.x, (int)v1.y);
		LineTo(m_MemDC, (int)v2.x, (int)v2.y);
		LineTo(m_MemDC, (int)v0.x, (int)v0.y);
	}

	// 바닥 점들 변환
	cMatrix matVPVp = m_matView * m_matProjection * m_matViewport;

	std::vector<cVector4> vecLineVertex(m_vecLineVertex.size());
	for (size_t i = 0; i < m_vecLineVertex.size(); ++i)
	{
		vecLineVertex[i].x = m_vecLineVertex[i].x;
		vecLineVertex[i].y = m_vecLineVertex[i].y;
		vecLineVertex[i].z = m_vecLineVertex[i].z;
		vecLineVertex[i].w = 1.f;
		vecLineVertex[i] = vecLineVertex[i] * matVPVp;
	}

	// 바닥 그리기
	for (size_t i = 0; i < vecLineVertex.size(); i += 2)
	{
		cVector4 v0 = vecLineVertex[i];
		cVector4 v1 = vecLineVertex[i + 1];

		MoveToEx(m_MemDC, (int)v0.x, (int)v0.y, NULL);
		LineTo(m_MemDC, (int)v1.x, (int)v1.y);
	}

	// 도화지에 그려진 내용을 화면에 표시
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, m_MemDC, 0, 0, SRCCOPY);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		{
			m_isLButtonDown = true;
			m_ptPrev.x = LOWORD(lParam);
			m_ptPrev.y = HIWORD(lParam);
		}
		break;
	case WM_LBUTTONUP:
		{
			m_isLButtonDown = false;
		}
		break;
	case WM_MOUSEMOVE:
		{
			if (!m_isLButtonDown)
				break;

			POINT ptCurr;
			ptCurr.x = LOWORD(lParam);
			ptCurr.y = HIWORD(lParam);

			int nDeltaX = ptCurr.x - m_ptPrev.x;
			int nDeltaY = ptCurr.y - m_ptPrev.y;

			m_fRotAngleY += (nDeltaX * 0.01f);
			m_fRotAngleX += (nDeltaY * 0.01f);
			if(m_fRotAngleX < -PI / 2.0f + 0.0001f)
				m_fRotAngleX = -PI / 2.0f + 0.0001f;
			if(m_fRotAngleX > PI / 2.0f - 0.0001f)
				m_fRotAngleX =  PI / 2.0f - 0.0001f;

			m_ptPrev = ptCurr;
		}
		break;
	case WM_MOUSEWHEEL:
		{
			m_fDistance -= (GET_WHEEL_DELTA_WPARAM(wParam) * 0.01f);
		}
		break;
	}
}
