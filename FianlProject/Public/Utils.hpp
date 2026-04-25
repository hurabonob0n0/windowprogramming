#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <math.h>
#include <memory>
using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

struct WinInfo {
    HWND hWnd;
    int WinCX;
    int WinCY;
};

extern WinInfo g_WinInfo;

inline COLORREF MyRGB(float r, float g, float b) {
    return RGB(static_cast<BYTE>(r * 255), static_cast<BYTE>(g * 255), static_cast<BYTE>(b * 255));
}

// 헬퍼: 비율 좌표를 픽셀로 변환 (Y축 반전 포함)
inline int RatioToX(float ratio, int width) { return static_cast<int>(ratio * width); }
inline int RatioToY(float ratio, int height) { return static_cast<int>((1.0f - ratio) * height); }