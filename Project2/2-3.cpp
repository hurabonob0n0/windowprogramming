#include "Random_Engine.hpp"
#include "Utilities.hpp"
#include <vector>
#include <string>

int g_numDiv = 0;    // 가로 등분 수 (2~16)
int g_colorMode = 0; // 랜덤 숫자 (홀수/짝수 판별용)

void make_2_3_data() {
	MyRandom* rand = MyRandom::GetInstance();
	g_numDiv = rand->GetInt(2, 16);
	g_colorMode = rand->GetInt(1, 100); // 1~100 사이의 랜덤 숫자 [cite: 684]
}
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"GugudanWindow";
LPCTSTR lpszWindowName = L"실습 2-2: 구구단 출력";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(0);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	// 800x600 크기로 윈도우 생성 [cite: 671]
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;


	switch (uMsg) {
	case WM_CREATE:
		make_2_3_data();
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		{
			MyRandom* rand = MyRandom::GetInstance();
			wstring info = L"등분: " + to_wstring(g_numDiv) + L" / 모드: " + (g_colorMode % 2 == 0 ? L"짝수(줄 별)" : L"홀수(단 별)");
			MyTextOut(hDC, 0.02f, 0.98f, info, MyRGB(1, 1, 1), MyRGB(0, 0, 1));

			float colWidth = 1.0f / g_numDiv;
			float margin = 0.005f;

			bool isOdd = (g_colorMode % 2 != 0); // 홀수 여부 판별 

			// 1. 윗줄 (2단 -> 등분)
			for (int i = 0; i < g_numDiv; i++) {
				int dan = i + 2;
				COLORREF danColor = MyRGB(rand->GetFloat(), rand->GetFloat(), rand->GetFloat()); // 홀수용 단 색상

				for (int j = 1; j <= 9; j++) {
					wstring line = to_wstring(dan) + L" * " + to_wstring(j) + L" = " + to_wstring(dan * j);

					// 홀수면 단 전체가 같은 색, 짝수면 줄마다 랜덤 색상 
					COLORREF textColor = isOdd ? danColor : MyRGB(rand->GetFloat(), rand->GetFloat(), rand->GetFloat());

					float L = (i * colWidth) + margin;
					float R = ((i + 1) * colWidth) - margin;

					// 각 줄의 Y 영역을 계산 (0.9 ~ 0.5 사이를 9등분)
					float rowHeight = 0.4f / 9.0f;
					float T = 0.9f - (rowHeight * (j - 1));
					float B = 0.9f - (rowHeight * j);

					MyDrawText(hDC, L, T, R, B, line, MyRGB(1, 1, 1), textColor);
				}
			}

			// 2. 아랫줄 (역순)
			for (int i = 0; i < g_numDiv; i++) {
				int dan = (g_numDiv + 1) - i;
				COLORREF danColor = MyRGB(rand->GetFloat(), rand->GetFloat(), rand->GetFloat());

				for (int j = 1; j <= 9; j++) {
					wstring line = to_wstring(dan) + L" * " + to_wstring(j) + L" = " + to_wstring(dan * j);
					COLORREF textColor = isOdd ? danColor : MyRGB(rand->GetFloat(), rand->GetFloat(), rand->GetFloat());

					float L = (i * colWidth) + margin;
					float R = ((i + 1) * colWidth) - margin;

					float rowHeight = 0.4f / 9.0f;
					float T = 0.45f - (rowHeight * (j - 1));
					float B = 0.45f - (rowHeight * j);

					MyDrawText(hDC, L, T, R, B, line, MyRGB(1, 1, 1), textColor);
				}
			}
		}
		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		if (wParam == 'R') {
			make_2_3_data(); // 데이터와 모드 재설정 
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}