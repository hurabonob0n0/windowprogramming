#pragma once
#include <random>

class MyRandom {
private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist01;

private:
    MyRandom() : gen(std::random_device{}()), dist01(0.0f, 1.0f) {}

private:
    // 복사 생성자와 대입 연산자 삭제
    MyRandom(const MyRandom&) = delete;
    MyRandom& operator=(const MyRandom&) = delete;
    static MyRandom* Instance;


public:
    // 0.0 ~ 1.0 사이의 uniform한 실수 리턴
    float GetFloat() {
        return dist01(gen);
    }

    // 지정된 범위 [min, max] 사이의 정수 리턴
    int GetInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }

    // 지정된 범위 [min, max] 사이의 실수 리턴
    float GetFloatRange(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

public:
    static MyRandom* GetInstance() {
        // 지역 정적 변수는 프로그램 종료 시 자동으로 파괴되며, 
        // 처음 호출될 때 단 한 번만 생성됩니다 (C++11부터 쓰레드 세이프).
        static MyRandom instance;
        return &instance;
    }
};
