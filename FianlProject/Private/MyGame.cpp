#include "MyGame.h"
#include <iostream>

WinInfo g_WinInfo = { nullptr, 800, 600 };

MyGame::MyGame() {}
MyGame::~MyGame() {}

float Rot = 0.f;

bool MyGame::Initialize() {
    // 1. 콘솔에서 크기 입력받기
    std::cout << "--- Window Setup ---" << std::endl;
    std::cout << "Width (pixel): ";
    std::cin >> g_WinInfo.WinCX;
    std::cout << "Height (pixel): ";
    std::cin >> g_WinInfo.WinCY;

    HINSTANCE hInstance = GetModuleHandle(NULL); // 현재 실행 파일의 hInstance 가져오기

    // 2. 윈도우 클래스 등록
    Register_Window(hInstance);

    // 3. 윈도우 생성
    Create_MyWindow(hInstance);

    if (!g_WinInfo.hWnd) return false;

    ShowWindow(g_WinInfo.hWnd, SW_SHOW);
    UpdateWindow(g_WinInfo.hWnd);

    m_Shape = Circle::Create()
        ->Set_PieAngle(270.0f)           // 270도만 그리기 (원의 3/4)
        ->Set_CenterPoint({ 0.5f, 0.5f })
        ->Set_Width_Height(0.4f, 0.4f)
        ->Set_Line(RGB(0, 0, 0), 2)
        ->Set_Fill(RGB(255, 255, 0))     // 노란색 팩맨!
        ->Set_Rot(45.0f)                 // 45도 회전시켜서 입 방향 조절
        ->Make_Points();

    return true;
}

void MyGame::Register_Window(HINSTANCE hInstance) {
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MyGame::WndProc; // 클래스 내부의 정적 함수 연결
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"MyGameEngine";

    RegisterClassExW(&wcex);
}

void MyGame::Create_MyWindow(HINSTANCE hInstance) {
    // 입력받은 크기를 실제 "작업 영역(Client Area)"으로 맞추기 위한 계산
    RECT rc = { 0, 0, g_WinInfo.WinCX, g_WinInfo.WinCY };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    g_WinInfo.hWnd = CreateWindowW(L"MyGameEngine", L"My WinAPI Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top, // 계산된 전체 창 크기
        nullptr, nullptr, hInstance, nullptr);
}

void MyGame::Update() {
    // 게임 로직 (예: 이동)
    Rot += 0.01f;
    m_Shape->Set_Rot(Rot)->Make_Points();
}

void MyGame::Late_Update() {
    // 로직 후처리
}

void MyGame::Draw() {
    HDC hDC = GetDC(g_WinInfo.hWnd);

    m_Shape->Draw(hDC);

    ReleaseDC(g_WinInfo.hWnd, hDC);
}

// 윈도우 메시지 처리기
LRESULT CALLBACK MyGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_SIZE: // 윈도우 크기가 변할 때 전역 변수 업데이트
        g_WinInfo.WinCX = LOWORD(lParam);
        g_WinInfo.WinCY = HIWORD(lParam);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}