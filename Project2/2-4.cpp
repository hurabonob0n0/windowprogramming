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
	while (GetMessage(&msg, 0, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
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
				MyTextOut(hDC, startX > 0.85 ? 0.85 : startX, 0.9f - (i * 0.05f), line, MyRGB(1, 1, 1), MyRGB(rand->GetFloat(), rand->GetFloat(), rand->GetFloat()));
			}
		}
		else {
			// [실습 2-4-2] GetTextExtentPoint32를 이용한 테두리 그리기
			SIZE size;
			int len = (int)g_data242.pattern.length();

			// 1. 현재 폰트 기준으로 문자열의 실제 픽셀 너비(cx)와 높이(cy) 측정 [cite: 1031, 1078]
			GetTextExtentPoint32(hDC, g_data242.pattern.c_str(), len, &size); 

			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			int winW = clientRect.right;
			int winH = clientRect.bottom;

			// 시작점 및 끝점 픽셀 좌표 계산
			int px = RatioToX(g_data242.x, winW);
			int py = RatioToY(g_data242.y, winH);
			int pW = RatioToX(g_data242.w, winW);
			int pH = RatioToX(g_data242.h, winH); // 높이도 너비 비율 기준으로 계산하거나 h 사용

			SetBkColor(hDC, MyRGB(1, 1, 1));
			SetTextColor(hDC, g_data242.color);

			// 2. 가로 선분 (윗변 & 아랫변) 출력
			// 측정된 폭(size.cx)에 상관없이 pattern 전체를 출력 [cite: 1052]
			TextOut(hDC, px, py, g_data242.pattern.c_str(), len);
			TextOut(hDC, px, py + pH, g_data242.pattern.c_str(), len);

			// 3. 세로 선분 (좌변 & 우변) 출력
			// 문자열의 실제 높이(size.cy)를 기준으로 간격을 조절하여 겹침 방지 [cite: 1043, 1053]
			int numVerticalChars = pH / (size.cy > 0 ? size.cy : 15); // 높이를 문자 높이로 나눔

			for (int i = 0; i <= numVerticalChars; ++i) {
				// 패턴 내의 문자를 순환하며 한 글자씩 세로로 출력
				wchar_t ch = g_data242.pattern[i % len];
				int drawY = py + (i * size.cy);

				// 사각형의 높이(pH)를 넘지 않도록 제어
				if (drawY > py + pH) break;

				TextOut(hDC, px, drawY, &ch, 1);           // 좌변
				TextOut(hDC, px + pW, drawY, &ch, 1);      // 우변
			}
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