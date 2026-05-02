#pragma once
#include "MyGame.h"
#include "Timer.h"

int main() {
    // MyGame 인스턴스 생성 (포인터 방식)
    MyGame* myGame = MyGame::Get_Instance();

    Timer gameTimer;
    float minFrameSec = 0.016f;
    float framesec = 0;

    // 초기화 실패 시 종료
    if (!myGame->Initialize()) {
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
            // 1. 매 프레임 가장 먼저 타이머를 갱신합니다.
            gameTimer.Tick();

            // 2. 갱신된 dt를 가져옵니다.
            framesec += gameTimer.Get_DeltaTime();

            if (framesec < minFrameSec)
                continue;

            // 3. 엔진 로직에 dt를 넘겨줍니다.
            myGame->Update(framesec);
            myGame->Late_Update(framesec);
            myGame->Draw();
            framesec = 0;
        }
    }

    myGame->Destroy_Instance();

    return (int)msg.wParam;
}