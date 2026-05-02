#include "Timer.h"

Timer::Timer() : m_DeltaTime(0.0f)
{
    // 생성될 때의 현재 시간을 초기값으로 세팅
    m_PrevTime = std::chrono::high_resolution_clock::now();
}

void Timer::Tick()
{
    // 1. 현재 프레임의 시간 측정
    auto currentTime = std::chrono::high_resolution_clock::now();

    // 2. 현재 시간 - 이전 시간 = 걸린 시간 (초 단위로 자동 변환)
    std::chrono::duration<float> elapsed = currentTime - m_PrevTime;

    m_DeltaTime = elapsed.count();
    m_PrevTime = currentTime;
    
}