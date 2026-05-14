#include "MyGame.h"
#include <iostream>
#include "RawInput.hpp"
#include "BaseSpriteObject.h"
#include "VertexBuffer.h"
#include "Mouse.h"

WinInfo g_WinInfo = { nullptr, 1600, 920};

std::unique_ptr<MyGame> MyGame::m_Instance = nullptr;
unique_ptr<RenderManager> RenderManager::m_Instance = nullptr;

MyGame::MyGame() {}
MyGame::~MyGame() {
    delete m_Mouse;
    delete m_Camera;
    delete m_GameObject;
	delete m_BaseObject;
}

float Rot = 0.f;
float PieAngle = 270.f;

bool MyGame::Initialize() {
    // 1. 콘솔에서 크기 입력받기
    /*std::cout << "--- Window Setup ---" << std::endl;
    std::cout << "Width (pixel): ";
    std::cin >> g_WinInfo.WinCX;
    std::cout << "Height (pixel): ";
    std::cin >> g_WinInfo.WinCY;*/

    HINSTANCE hInstance = GetModuleHandle(NULL); // 현재 실행 파일의 hInstance 가져오기

    // 2. 윈도우 클래스 등록
    Register_Window(hInstance);

    // 3. 윈도우 생성
    Create_MyWindow(hInstance);

    if (!g_WinInfo.hWnd) return false;

    ShowCursor(false);

    CenterWindowAndMouse(g_WinInfo.hWnd);

    g_RawInput->Initialize(g_WinInfo.hWnd);

    ShowWindow(g_WinInfo.hWnd, SW_SHOW);
    UpdateWindow(g_WinInfo.hWnd);

    Create_BackBuffer();

    VertexBuffer::Build_Geometrys();
	//=========================Mouse======================
	m_Mouse = new Mouse();
	m_Mouse->Initialize();

    //=========================RM========================
    m_RM = RenderManager::Get_Instance();

    //=======================Camera======================
    m_Camera = new Camera();
    m_Camera->Initialize();

    //====================bo=============
    m_BaseObject = new BaseObject();
    m_BaseObject->Initialize();

    //====================bo=============
    m_GameObject = new BaseSpriteObject();
    m_GameObject->Initialize();

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

void MyGame::CenterWindowAndMouse(HWND hwnd)
{
    RECT rect;
    GetWindowRect(hwnd, &rect); // 현재 창의 크기를 가져옴

    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    // 1. 전체 화면 해상도 가져오기
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 2. 중앙 좌표 계산
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    // 3. 윈도우 위치 이동
    SetWindowPos(hwnd, NULL, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    // 4. 마우스 커서를 창의 정중앙으로 이동
    // 창의 중심점 = (시작 좌표 + 너비의 절반)
    SetCursorPos(posX + (windowWidth / 2), posY + (windowHeight / 2));
}

void MyGame::Update(float dt) {
    if(g_RawInput->Key_Down(VK_ESCAPE)) {
        PostQuitMessage(0);
	}
	m_Mouse->Update(dt);
    m_BaseObject->Update(dt);
    m_GameObject->Update(dt);
}

void MyGame::Late_Update(float dt) {
	m_Mouse->Late_Update(dt);
    m_BaseObject->Late_Update(dt);
    m_GameObject->Late_Update(dt);
    m_Camera->Late_Update(dt);
}

void MyGame::Draw() {
    if (!m_hMemDC) return;

    // 1. [가짜 도화지]를 배경색으로 깨끗하게 지웁니다.
    RECT rect = { 0, 0, g_WinInfo.WinCX, g_WinInfo.WinCY };
    FillRect(m_hMemDC, &rect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

    // 2. 여기에 그립니다.
	FXMMATRIX view = m_RM->Get_ViewMatrix();
	CXMMATRIX proj = m_RM->Get_ProjMatrix();
    m_BaseObject->Draw(m_hMemDC, view,proj);
    m_GameObject->Draw(m_hMemDC, view, proj);
	m_Mouse->Draw(m_hMemDC,  view,proj);

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
        MyGame::Get_Instance()->Create_BackBuffer();

        break;

    case WM_INPUT:
        g_RawInput->Update_InputDev(lParam);
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
