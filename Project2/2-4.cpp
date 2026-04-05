#include "Random_Engine.hpp"
#include "Utilities.hpp"
#include <vector>

enum class Mode { EX_2_4, EX_2_4_2 };
Mode g_currentMode = Mode::EX_2_4;

// 실습 2-4 데이터
struct { int dan, colIdx; } g_data24;
// 실습 2-4-2 데이터
struct { float x, y, w, h; std::wstring pattern; COLORREF color; } g_data242;

void MakeData24() {
	MyRandom* rand = MyRandom::GetInstance();
	g_data24.dan = rand->GetInt(2, 20);
	g_data24.colIdx = rand->GetInt(0, g_data24.dan - 1);
}

void MakeData242() {
	MyRandom* rand = MyRandom::GetInstance();
	g_data242.x = rand->GetFloatRange(0.0f, 0.5f);
	g_data242.y = rand->GetFloatRange(0.5f, 1.0f);
	g_data242.w = rand->GetFloatRange(0.15f, 0.5f);
	g_data242.h = rand->GetFloatRange(0.15f, 0.5f);
	g_data242.color = MyRGB(rand->GetFloat(), rand->GetFloat(), rand->GetFloat());
	g_data242.pattern = L"";
	for (int i = 0; i < 50; ++i) g_data242.pattern += (wchar_t)rand->GetInt('A', 'Z');
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nShow) {
	LPCTSTR szClass = L"GugudanApp";
	MSG msg;
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInst,
					  LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
					  (HBRUSH)GetStockObject(WHITE_BRUSH), NULL, szClass, NULL };
	RegisterClassEx(&wc);
	HWND hWnd = CreateWindow(szClass, L"2장 실습 통합", WS_OVERLAPPEDWINDOW, 0, 0, 1280, 960, NULL, NULL, hInst, NULL);
	ShowWindow(hWnd, nShow);
	while ( GetMessage(&msg, 0, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps; HDC hDC;
	MyRandom* rand = MyRandom::GetInstance();

	switch (uMsg) {
	case WM_CREATE: MakeData24(); MakeData242(); break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		if (g_currentMode == Mode::EX_2_4) {
			float colWidth = 1.0f / g_data24.dan;
			float startX = g_data24.colIdx * colWidth;
			for (int i = 1; i <= 9; ++i) {
				std::wstring indent(3 * (i - 1), L' ');
				std::wstring line = indent + std::to_wstring(g_data24.dan) + L" * " + std::to_wstring(i) + L" = " + std::to_wstring(g_data24.dan * i);
				MyTextOut(hDC, startX, 0.9f - (i * 0.05f), line, MyRGB(1, 1, 1), MyRGB(rand->GetFloat(), rand->GetFloat(), rand->GetFloat()));
			}
		}
		else {
			// 사각형 채우기: 문자열 반복 및 자동 줄바꿈 [cite: 534, 1135]
			std::wstring fullPattern = L"";
			for (int i = 0; i < 10; ++i) fullPattern += g_data242.pattern;
			MyDrawText(hDC, g_data242.x, g_data242.y, g_data242.x + g_data242.w, g_data242.y - g_data242.h,
				fullPattern, MyRGB(1, 1, 1), g_data242.color, DT_WORDBREAK | DT_EDITCONTROL);
		}
		EndPaint(hWnd, &ps); break;
	case WM_KEYDOWN:
		if (wParam == '1') g_currentMode = Mode::EX_2_4;
		else if (wParam == '2') g_currentMode = Mode::EX_2_4_2;
		else if (wParam == 'R') { MakeData24(); MakeData242(); }
		else if (wParam == VK_ESCAPE) PostQuitMessage(0);
		InvalidateRect(hWnd, NULL, TRUE); break;
	case WM_DESTROY: PostQuitMessage(0); break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}