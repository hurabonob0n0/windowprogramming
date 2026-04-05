#include <Windows.h>
#include <string>
using namespace std;

inline COLORREF MyRGB(float r, float g, float b) {
    return RGB(static_cast<BYTE>(r * 255), static_cast<BYTE>(g * 255), static_cast<BYTE>(b * 255));
}

// 헬퍼: 비율 좌표를 픽셀로 변환 (Y축 반전 포함)
inline int RatioToX(float ratio, int width) { return static_cast<int>(ratio * width); }
inline int RatioToY(float ratio, int height) { return static_cast<int>((1.0f - ratio) * height); }

// 배경색 default: 흰색, 글자색 default: 검은색 [cite: 605, 608]
inline void MyTextOut(HDC hDC, float x_ratio, float y_ratio, const std::wstring& text,
    COLORREF bgColor = MyRGB(1.0f, 1.0f, 1.0f),
    COLORREF textColor = MyRGB(0.0f, 0.0f, 0.0f))
{
    HWND hWnd = WindowFromDC(hDC);
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int width = clientRect.right;
    int height = clientRect.bottom;

    int px = RatioToX(x_ratio, width);
    int py = RatioToY(y_ratio, height);

    SetBkColor(hDC, bgColor);
    SetTextColor(hDC, textColor);

    TextOut(hDC, px, py, text.c_str(), static_cast<int>(text.length()));
}

// MyDrawText: 비율 좌표(0.0~1.0)를 사용하여 사각형 영역 내에 텍스트 출력
inline void MyDrawText(HDC hDC, float L, float T, float R, float B, const std::wstring& text,
    COLORREF bgColor = MyRGB(1.0f, 1.0f, 1.0f),
    COLORREF textColor = MyRGB(0.0f, 0.0f, 0.0f),
    UINT flags = DT_CENTER | DT_VCENTER | DT_SINGLELINE)    // ‒ DT_LEFT: 박스 영역 내에서 왼쪽 정렬
                                                            //‒ DT_CENTER : 박스 영역 내에서 가운데 정렬
                                                            //‒ DT_RIGHT : 박스 영역 내에서 오른쪽 정렬
                                                            //‒ DT_TOP : 박스 영역의 상하에서 위쪽에 출력
                                                            //‒ DT_BOTTOM : 박스 영역의 상하에서 아래쪽에 출력
                                                            //‒ DT_VCENTER : 박스 영역의 상하에서 가운데 출력(DT_SINGLELINE 과 함께 사용)
                                                            //‒ DT_SINGLELINE : 박스 영역 안에 한 줄로 출력(세로에 해당하는 플래그 사용 시 함께 사용)
                                                            //‒ DT_WORDBREAK : 단어가 박스 영역의 오른쪽 끝에 닿으면 단어 단위로 줄바꿈
                                                            //‒ DT_EDITCONTROL : DT_WORDBREAK에 OR해주면 영문, 한글, 숫자 모두 글자 단위로 개행
{
    HWND hWnd = WindowFromDC(hDC);
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int width = clientRect.right;
    int height = clientRect.bottom;

    RECT drawRect;
    drawRect.left = static_cast<int>(L * width);
    drawRect.right = static_cast<int>(R * width);
    drawRect.top = static_cast<int>((1.0f - T) * height);
    drawRect.bottom = static_cast<int>((1.0f - B) * height);

    if (drawRect.top > drawRect.bottom) std::swap(drawRect.top, drawRect.bottom);

    SetBkColor(hDC, bgColor);
    SetTextColor(hDC, textColor);

    DrawText(hDC, text.c_str(), static_cast<int>(text.length()), &drawRect, flags);
}