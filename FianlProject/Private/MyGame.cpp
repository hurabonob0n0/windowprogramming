#include "MyGame.h"
#include <iostream>

WinInfo g_WinInfo = { nullptr, 800, 600 };

std::unique_ptr<MyGame> MyGame::m_Instance = nullptr;
unique_ptr<RenderManager> RenderManager::m_Instance = nullptr;

MyGame::MyGame() {}
MyGame::~MyGame() {}

float Rot = 0.f;
float PieAngle = 270.f;

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

    Create_BackBuffer();

    Shape::Build_Geometrys();


    //=========================RM========================
    m_RM = RenderManager::Get_Instance();

    //=======================Camera======================
    m_Camera = new Camera();
    m_Camera->Initialize();

    //=======================BaseObject=================
    m_Shape = new BaseObject();
    m_Shape->Initialize();

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

void MyGame::Update(float dt) {
    m_Shape->Update(dt);
}

void MyGame::Late_Update(float dt) {
    m_Shape->Late_Update(dt);
    m_Camera->Late_Update(dt);
}

void MyGame::Draw() {
    if (!m_hMemDC) return;

    // 1. [가짜 도화지]를 배경색으로 깨끗하게 지웁니다.
    RECT rect = { 0, 0, g_WinInfo.WinCX, g_WinInfo.WinCY };
    FillRect(m_hMemDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    //2. 여기에 그립니다.
    m_Shape->Draw(m_hMemDC,m_RM->Get_ViewMatrix(),m_RM->Get_ProjMatrix());

    // 3. 완성된 [가짜 도화지]를 [실제 화면]으로 순식간에 복사합니다. (BitBlt)
    HDC hDC = GetDC(g_WinInfo.hWnd);
    BitBlt(hDC, 0, 0, g_WinInfo.WinCX, g_WinInfo.WinCY, m_hMemDC, 0, 0, SRCCOPY);
    ReleaseDC(g_WinInfo.hWnd, hDC);
}

// 윈도우 메시지 처리기
LRESULT CALLBACK MyGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_SIZE: // 윈도우 크기가 변할 때 전역 변수 업데이트
        g_WinInfo.WinCX = LOWORD(lParam);
        g_WinInfo.WinCY = HIWORD(lParam);

        cout << "가로 : " << g_WinInfo.WinCX << "세로 : " << g_WinInfo.WinCY << endl;
        MyGame::Get_Instance()->Create_BackBuffer();
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void MyGame::Create_BackBuffer()
{
    {
        HDC hDC = GetDC(g_WinInfo.hWnd);

        // 1. 기존에 쓰던 게 있다면 삭제
        if (m_hMemDC) {
            SelectObject(m_hMemDC, m_hOldBitmap);
            DeleteObject(m_hBackBitmap);
            DeleteDC(m_hMemDC);
        }

        // 2. 새로운 메모리 DC 생성
        m_hMemDC = CreateCompatibleDC(hDC);
        // 3. 현재 화면과 똑같은 설정의 비트맵 생성
        m_hBackBitmap = CreateCompatibleBitmap(hDC, g_WinInfo.WinCX, g_WinInfo.WinCY);
        // 4. 메모리 DC에 비트맵 연결
        m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBackBitmap);

        ReleaseDC(g_WinInfo.hWnd, hDC);
    }
}
