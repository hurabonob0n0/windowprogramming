#pragma once
#include "Utils.hpp"
#include "SceneManager.h"
#include "RenderManager.h"


class MyGame {
public:
    MyGame();
    ~MyGame();

public:
    bool Initialize(); // 윈도우 생성 및 초기 설정
    void Update(float dt);     // 로직 업데이트
    void Late_Update(float dt); // 보정 및 충돌 체크 등
    void Draw();       // 렌더링 (그리기)

private:
    void Register_Window(HINSTANCE hInstance);
    void Create_MyWindow(HINSTANCE hInstance);
    void CenterWindowAndMouse(HWND hwnd);

private:
    // 윈도우 프로시저 (메시지 처리)
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void Create_BackBuffer();

private: // <unique_ptr>
    RenderManager* m_RM = nullptr;
    SceneManager* m_SceneManager = nullptr;

public:
    HDC     m_hMemDC{ nullptr };     // 가짜 도화지 (메모리 DC)
    HBITMAP m_hBackBitmap{ nullptr }; // 가짜 도화지용 비트맵
    HBITMAP m_hOldBitmap{ nullptr };  // 원래 비트맵 저장용

private:
    static unique_ptr<MyGame> m_Instance;

public:
    static MyGame* Get_Instance() {
        if (!m_Instance) {
            m_Instance = std::unique_ptr<MyGame>(new MyGame());
        }
        return m_Instance.get();
    }

    static void Destroy_Instance() {
        m_Instance.reset(); // 이때 소멸자가 즉시 호출됨
    }   
};
