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

	// �ٴ�
	std::vector<cVector3>	m_vecLineVertex;

	// �����Ʈ���� ���ڵ�..
	cVector3				m_vPosition;
	cVector3				m_vDirection;
	float					m_fSpeed;
	float					m_fAngle;

	// �� ��Ʈ���� ���ڵ�..
	cVector3				m_vEye;
	cVector3				m_vLookAt;
	cVector3				m_vUp;

	// ������ ������ ���� ��Ʈ������
	cMatrix					m_matWorld;
	cMatrix					m_matView;
	cMatrix					m_matProjection;
	cMatrix					m_matViewport;


	// ���콺 ȸ�� ����
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

