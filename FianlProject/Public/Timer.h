#pragma once
#include <chrono>

class Timer
{
private:
    // C++의 시간 측정 전용 타입들
    std::chrono::high_resolution_clock::time_point m_PrevTime;
    float m_DeltaTime;

public:
    Timer();
    ~Timer() = default;

    // 매 프레임마다 호출되어 dt를 갱신해주는 함수
    void Tick();

    // 계산된 dt를 반환하는 함수
    float Get_DeltaTime() const { return m_DeltaTime; }
};