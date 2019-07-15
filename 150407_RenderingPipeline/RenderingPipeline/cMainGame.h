#pragma once

#include "cVector3.h"
#include "cMatrix.h"

class cMainGame
{
private:
	HDC			m_MemDC; 
	HBITMAP		m_hOldBitmap, m_hBitmap;

	std::vector<cVector3>	m_vecVertex;
	std::vector<int>		m_vecIndex;

	// 바닥
	std::vector<cVector3>	m_vecLineVertex;

	// 월드매트릭스 인자들..
	cVector3				m_vPosition;
	cVector3				m_vDirection;
	float					m_fSpeed;
	float					m_fAngle;

	// 뷰 매트릭스 인자들..
	cVector3				m_vEye;
	cVector3				m_vLookAt;
	cVector3				m_vUp;

	// 랜더링 파이프 라인 매트릭스들
	cMatrix					m_matWorld;
	cMatrix					m_matView;
	cMatrix					m_matProjection;
	cMatrix					m_matViewport;


	// 마우스 회전 관련
	POINT					m_ptPrev;
	bool					m_isLButtonDown;
	float					m_fRotAngleX;
	float					m_fRotAngleY;
	float					m_fDistance;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render(HDC hdc);
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

