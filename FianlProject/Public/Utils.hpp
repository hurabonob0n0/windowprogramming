#pragma once
// 메모리 릭 검출을 위한 매크로 및 헤더 (디버그 모드에서만 동작)
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <Windows.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <math.h>
#include <memory>
#include <algorithm>
using namespace std;

#include <DirectXMath.h>
using namespace DirectX;

struct WinInfo {
    HWND hWnd;
    int WinCX;
    int WinCY;
};

// static variables
extern WinInfo g_WinInfo;
class RawInput;
extern unique_ptr<RawInput> g_RawInput;
class MyRandom;
extern MyRandom* g_Random;

enum class eLayerType {
    Background,
    Default,
    Monster,
    Player,
    UI,
    Mouse,
    End
};