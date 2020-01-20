// Make3DPipeline.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Make3DPipeline.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;

Vector3 Vertice[] =
{
	Vector3(-1.f, -1.f, -1.f),
	Vector3(-1.f,  1.f, -1.f),
	Vector3(1.f,  1.f, -1.f),
	Vector3(1.f, -1.f, -1.f),
	Vector3(-1.f, -1.f,  1.f),
	Vector3(-1.f,  1.f,  1.f),
	Vector3(1.f,  1.f,  1.f),
	Vector3(1.f, -1.f,  1.f)
};

int Indice[] =
{
	0, 1, 2,
	0, 2, 3,
	4, 6, 5,
	4, 7, 6,
	4, 5, 1,
	4, 1, 0,
	3, 2, 6,
	3, 6, 7,
	1, 5, 6,
	1, 6, 2,
	4, 0, 3,
	4, 3, 7
};

Vector3 Eye(0.f, 0.f, 3.f);
Vector3 Position(0.f, 0.f, 0.f);

float angleX = 0.f, angleY = 0.f, Distance = 15.f;

void Render(HDC hdc);

HDC MemDC;

HBITMAP hBitmap;

HBITMAP hOldBitmap;

bool Initialize = false;


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MAKE3DPIPELINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAKE3DPIPELINE));

    MSG msg;

	HDC hdc = GetDC(g_hWnd);

	MemDC = CreateCompatibleDC(hdc);

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);

	// SelectObject : 리턴값으로 이전의 객체를 반환
	hOldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap);

	ReleaseDC(g_hWnd, hdc);

	Initialize = true;

	SetTimer(g_hWnd, 1000, 10, NULL);

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	SelectObject(MemDC, hOldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(MemDC);


    return (int) msg.wParam;
}



void Render(HDC hdc)
{
	Matrix4x4 matRotX = Matrix4x4RotationX(angleX);
	Matrix4x4 matRotY = Matrix4x4RotationY(angleY);
	Matrix4x4 matCamRot = matRotX * matRotY;
	Eye = Vector3(0.f, 0.f, -Distance);
	Eye = (matCamRot * Vector4(Eye, 1.f)).ToVector3();
	Eye = Eye + Position;


	RECT rc;
	GetClientRect(g_hWnd, &rc);
	PatBlt(MemDC, rc.left, rc.top, rc.right, rc.bottom, WHITENESS);

	Matrix4x4 worldMat = GetTranslationMatrix(Position.x, Position.y, Position.z)
		, ViewMatrix = GetViewMatrix(Vector3(0.f, 1.f, 0.f), (Position - Eye).Normalized(), Eye)
		, projectionMatrix = GetPerspectiveMatrix(3.141592f * 0.25f, (float)rc.right / (float)rc.bottom, 1.f, 100.f)
		, viewportMatrix = GetViewPortMatrix(0.f, 0.f, (float)rc.right, (float)rc.bottom, 0.f, 1.f);


	Matrix4x4 renderingMatrix = ViewMatrix * projectionMatrix;

	Vector4 vertice[8];

	for (int i = 0; i < 8; ++i)
	{
		vertice[i] = Vector4(Vertice[i], 1.f);
		vertice[i] = renderingMatrix * vertice[i];

		vertice[i] = vertice[i] * (1.f / vertice[i].w);
	}

	for (int i = 0; i < 36; i += 3)
	{
		Vector4 v0 = vertice[Indice[i]]
			, v1 = vertice[Indice[i + 1]]
			, v2 = vertice[Indice[i + 2]];

		Vector3 v01 = v1.ToVector3() - v0.ToVector3();
		Vector3 v02 = v2.ToVector3() - v0.ToVector3();
		Vector3 n = Vector3::CrossProduct(v01, v02);
		float f = Vector3::DotProduct(n, Vector3(0.f, 0.f, 1.f));

		if (f > 0)
			continue;

		v0 = viewportMatrix * v0;
		v1 = viewportMatrix * v1;
		v2 = viewportMatrix * v2;

		MoveToEx(MemDC, (int)v0.x, (int)v0.y, NULL);
		LineTo(MemDC, (int)v1.x, (int)v1.y);
		LineTo(MemDC, (int)v2.x, (int)v2.y);
		LineTo(MemDC, (int)v0.x, (int)v0.y);
	}

	BitBlt(hdc, 0, 0, rc.right, rc.bottom, MemDC, 0, 0, SRCCOPY);

}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAKE3DPIPELINE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MAKE3DPIPELINE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
bool LButtonDown = false;
POINT Prev;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_TIMER:
		{
			InvalidateRect(hWnd, NULL, false);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

			if(Initialize)
				Render(hdc);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_LBUTTONDOWN:
		{
			LButtonDown = true;
			Prev.x = LOWORD(lParam);
			Prev.y = HIWORD(lParam);
		}
		break;
	case WM_LBUTTONUP:
		{
			LButtonDown = false;
		}
		break;
	case WM_MOUSEMOVE:
		{
			if (!LButtonDown)
				break;

			POINT ptCurr;
			ptCurr.x = LOWORD(lParam);
			ptCurr.y = HIWORD(lParam);

			int nDeltaX = ptCurr.x - Prev.x;
			int nDeltaY = ptCurr.y - Prev.y;

			angleY += (nDeltaX * 0.01f);
			angleX += (nDeltaY * 0.01f);
			if (angleX < -PI / 2.0f + 0.0001f)
				angleX = -PI / 2.0f + 0.0001f;
			if (angleX > PI / 2.0f - 0.0001f)
				angleX = PI / 2.0f - 0.0001f;

			Prev = ptCurr;
		}
		break;
	case WM_MOUSEWHEEL:
		{
			Distance -= (GET_WHEEL_DELTA_WPARAM(wParam) * 0.01f);
		}
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
