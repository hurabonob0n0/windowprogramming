#pragma once
#include "Utils.hpp"
#include "shape.h"

class MyGame {
public:
    MyGame();
    ~MyGame();

public:
    bool Initialize(); // 윈도우 생성 및 초기 설정
    void Update();     // 로직 업데이트
    void Late_Update(); // 보정 및 충돌 체크 등
    void Draw();       // 렌더링 (그리기)

private:
    void Register_Window(HINSTANCE hInstance);
    void Create_MyWindow(HINSTANCE hInstance);

private:
    // 윈도우 프로시저 (메시지 처리)
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    Shape* m_Shape;
};