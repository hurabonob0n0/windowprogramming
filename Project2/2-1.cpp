#include "Random_Engine.hpp"
#include "Utilities.hpp"
#include <vector>

typedef struct tagMyData {
	float x, y;
	// 텍스트가 가로로 길기 때문에 X축 범위를 Y축보다 넓게 잡는 것이 좋습니다.
	bool isOverlap(const tagMyData& other) {
		if (abs(x - other.x) < 0.15f && abs(y - other.y) < 0.05f) return true;
		return false;
	}
	bool isOver(){ return (x > 0.9f || y < 0.1f); }
} MD;

vector<MD> g_dataList;

void make_data() {
	g_dataList.push_back({ 0.5f, 0.5f }); // 중앙 (0: (400, 300) 역할) [cite: 660]
	MyRandom* rand = MyRandom::GetInstance();

	// 15개가 채워질 때까지 반복 [cite: 656]
	while (g_dataList.size() < 15) {
		MD newData = { rand->GetFloat(), rand->GetFloat() };
		bool overlap = false;
		for (const auto& data : g_dataList) {
			if (newData.isOverlap(data) || newData.isOver()) {
				overlap = true;
				break;
			}
		}
		if (!overlap) g_dataList.push_back(newData);
	}
}


HINSTANCE g_hInst;
LPCTSTR lpszClass = L"My First Window";
LPCTSTR lpszWindowName = L"windows program1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(1);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1280, 960, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	switch (uMsg) {
	case WM_CREATE: {
		make_data();
		break;
	}
	case WM_PAINT: {
		hDC = BeginPaint(hWnd, &ps);

		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		int width = clientRect.right;
		int height = clientRect.bottom;

		for (int i = 0; i < g_dataList.size(); i++) {
			// 1. 픽셀 좌표 계산 (화면 표시용)
			int windowX = RatioToX(g_dataList[i].x, width);
			int windowY = RatioToY(g_dataList[i].y, height);

			// 2. 문자열 생성: "번호: (x, y)" [cite: 655, 658]
			wstring text = to_wstring(i) + L": (" + to_wstring(windowX) + L", " + to_wstring(windowY) + L")";

			// 3. 출력: MyTextOut에는 픽셀이 아닌 '비율(x, y)'을 그대로 전달
			// 배경 노란색(실습 코드 참고), 글자 검은색
			MyTextOut(hDC, g_dataList[i].x, g_dataList[i].y, text, MyRGB(1.0f, 1.0f, 0.0f), MyRGB(0.0f, 0.0f, 0.0f));
		}
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_KEYDOWN: {
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		if (wParam == 'R') {
			g_dataList.clear();
			make_data();
			InvalidateRect(hWnd, NULL, TRUE); //--- 화면 전체를 지우고 WM_PAINT 메시지를 보내도록 OS에 요청
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}