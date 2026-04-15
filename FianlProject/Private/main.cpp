#pragma once
#include "MyGame.h"

int main() {
    // MyGame 인스턴스 생성 (포인터 방식)
    MyGame* myGame = new MyGame();

    // 초기화 실패 시 종료
    if (!myGame->Initialize()) {
        delete myGame;
        return -1;
    }

    // 메시지 루프
    MSG msg = {};
    while (true) {
        // 메시지가 있으면 처리하고, 없으면 게임 루프 실행 (PeekMessage)
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // 게임 엔진 메인 루프
            myGame->Update();
            myGame->Late_Update();
            myGame->Draw();
        }
    }

    delete myGame;
    return (int)msg.wParam;
}