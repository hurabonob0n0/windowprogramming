#include "Random_Engine.hpp"
#include "Utilities.hpp"
#include <vector>
#include <string>

// 전역 변수
int g_numDiv = 0; // 가로 등분 수 (2~16) 

// 데이터를 생성하는 함수
void make_gugudan_data() {
    MyRandom* rand = MyRandom::GetInstance();
    g_numDiv = rand->GetInt(2, 16); // 2에서 16 사이의 랜덤 등분 설정 
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
        make_gugudan_data();
        break;

    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps); //
        {
            // 1. 상단 정보 출력
            wstring info = L"현재 등분: " + to_wstring(g_numDiv);
            MyTextOut(hDC, 0.02f, 0.98f, info, MyRGB(1.0f, 1.0f, 1.0f), MyRGB(0.0f, 0.0f, 1.0f));

            float colWidth = 1.0f / g_numDiv; // 한 칸의 가로 폭 비율
            float margin = 0.005f;           // 단 사이의 간격 (0.5%)

            // 2. 윗줄 출력 (2단 -> 등분까지)
            for (int i = 0; i < g_numDiv; i++) {
                int dan = i + 2;
                wstring content = L"";
                for (int j = 1; j <= 9; j++) {
                    content += to_wstring(dan) + L" * " + to_wstring(j) + L" = " + to_wstring(dan * j) + L"\n";
                }

                // [수정] L에 마진을 더하고, R에서 마진을 뺍니다.
                float L = (i * colWidth) + margin;
                float R = ((i + 1) * colWidth) - margin;

                // 줄바꿈을 위해 DT_SINGLELINE 제외 [cite: 534]
                MyDrawText(hDC, L, 0.9f, R, 0.5f, content, MyRGB(1, 1, 1), MyRGB(0, 0, 0), DT_CENTER | DT_TOP);
            }

            // 3. 아랫줄 출력 (최대 단 -> 2단까지 역순)
            for (int i = 0; i < g_numDiv; i++) {
                int dan = (g_numDiv + 1) - i;
                wstring content = L"";
                for (int j = 1; j <= 9; j++) {
                    content += to_wstring(dan) + L" * " + to_wstring(j) + L" = " + to_wstring(dan * j) + L"\n";
                }

                // [수정] 동일하게 마진 적용
                float L = (i * colWidth) + margin;
                float R = ((i + 1) * colWidth) - margin;

                MyDrawText(hDC, L, 0.45f, R, 0.05f, content, MyRGB(1, 1, 1), MyRGB(0.1f, 0.4f, 0.1f), DT_CENTER | DT_TOP);
            }
        }
        EndPaint(hWnd, &ps); //
        break;

    case WM_KEYDOWN:
        if (wParam == 'R') {
            make_gugudan_data(); // 새로운 랜덤 등분 생성 
            InvalidateRect(hWnd, NULL, TRUE); // 화면 갱신
        }
        if (wParam == VK_ESCAPE) PostQuitMessage(0);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}