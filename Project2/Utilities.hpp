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

// 특정 좌표에 점 찍기 [cite: 1155]
inline void MySetPixel(HDC hDC, float x, float y, COLORREF color) {
    HWND hWnd = WindowFromDC(hDC);
    RECT rect; GetClientRect(hWnd, &rect);
    SetPixel(hDC, RatioToX(x, rect.right), RatioToY(y, rect.bottom), color);
}

// 직선 그리기 
inline void MyDrawLine(HDC hDC, float x1, float y1, float x2, float y2,
    COLORREF color = MyRGB(0, 0, 0), int width = 1, int style = PS_SOLID) {

    HWND hWnd = WindowFromDC(hDC);
    RECT rect; GetClientRect(hWnd, &rect);

    HPEN hPen = CreatePen(style, width, color); // 펜 생성 [cite: 1394]
    HPEN oldPen = (HPEN)SelectObject(hDC, hPen); // 펜 선택 [cite: 1403]

    MoveToEx(hDC, RatioToX(x1, rect.right), RatioToY(y1, rect.bottom), NULL);
    LineTo(hDC, RatioToX(x2, rect.right), RatioToY(y2, rect.bottom));

    SelectObject(hDC, oldPen); // 복구 [cite: 1532]
    DeleteObject(hPen); // 삭제 [cite: 1405]
}

// 사각형 그리기 [cite: 1206]
inline void MyDrawRect(HDC hDC, float L, float T, float R, float B,
    COLORREF lineColor = MyRGB(0, 0, 0), int lineWidth = 1,
    COLORREF fillColor = MyRGB(1, 1, 1), bool useFill = true) {

    HWND hWnd = WindowFromDC(hDC);
    RECT rect; GetClientRect(hWnd, &rect);

    HPEN hPen = CreatePen(PS_SOLID, lineWidth, lineColor);
    HPEN oldPen = (HPEN)SelectObject(hDC, hPen);

    // 내부를 채울 브러시 설정 [cite: 1436]
    HBRUSH hBrush = useFill ? CreateSolidBrush(fillColor) : (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

    Rectangle(hDC, RatioToX(L, rect.right), RatioToY(T, rect.bottom),
        RatioToX(R, rect.right), RatioToY(B, rect.bottom));

    SelectObject(hDC, oldPen);
    SelectObject(hDC, oldBrush);
    DeleteObject(hPen);
    if (useFill) DeleteObject(hBrush); // 생성한 객체만 삭제 [cite: 1490]
}

// 원/타원 그리기 [cite: 1193]
inline void MyDrawEllipse(HDC hDC, float L, float T, float R, float B,
    COLORREF lineColor = MyRGB(0, 0, 0), int lineWidth = 1,
    COLORREF fillColor = MyRGB(1, 1, 1), bool useFill = true) {

    HWND hWnd = WindowFromDC(hDC);
    RECT rect; GetClientRect(hWnd, &rect);

    HPEN hPen = CreatePen(PS_SOLID, lineWidth, lineColor);
    HPEN oldPen = (HPEN)SelectObject(hDC, hPen);
    HBRUSH hBrush = useFill ? CreateSolidBrush(fillColor) : (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

    Ellipse(hDC, RatioToX(L, rect.right), RatioToY(T, rect.bottom),
        RatioToX(R, rect.right), RatioToY(B, rect.bottom));

    SelectObject(hDC, oldPen);
    SelectObject(hDC, oldBrush);
    DeleteObject(hPen);
    if (useFill) DeleteObject(hBrush);
}

// 둥근 사각형 그리기 
inline void MyDrawRoundRect(HDC hDC, float L, float T, float R, float B,
    float cornerWidthRatio, float cornerHeightRatio,
    COLORREF lineColor = MyRGB(0, 0, 0), int lineWidth = 1,
    COLORREF fillColor = MyRGB(1, 1, 1)) {

    HWND hWnd = WindowFromDC(hDC);
    RECT rect; GetClientRect(hWnd, &rect);

    HPEN hPen = CreatePen(PS_SOLID, lineWidth, lineColor);
    HPEN oldPen = (HPEN)SelectObject(hDC, hPen);
    HBRUSH hBrush = CreateSolidBrush(fillColor);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

    RoundRect(hDC, RatioToX(L, rect.right), RatioToY(T, rect.bottom),
        RatioToX(R, rect.right), RatioToY(B, rect.bottom),
        RatioToX(cornerWidthRatio, rect.right), RatioToY(cornerHeightRatio, rect.bottom));

    SelectObject(hDC, oldPen);
    SelectObject(hDC, oldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

// 삼각형 그리기 (Polygon 함수 활용)
inline void MyDrawTriangle(HDC hDC, float x1, float y1, float x2, float y2, float x3, float y3,
    COLORREF lineColor = MyRGB(0, 0, 0), int lineWidth = 1,
    COLORREF fillColor = MyRGB(1, 1, 1), bool useFill = true) {

    HWND hWnd = WindowFromDC(hDC);
    RECT rect;
    GetClientRect(hWnd, &rect); // 윈도우의 현재 크기 가져오기 [cite: 1507]

    // 1. 펜과 브러시 생성 및 선택 [cite: 1403, 1437]
    HPEN hPen = CreatePen(PS_SOLID, lineWidth, lineColor);
    HPEN oldPen = (HPEN)SelectObject(hDC, hPen);

    HBRUSH hBrush = useFill ? CreateSolidBrush(fillColor) : (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

    // 2. 비율 좌표를 픽셀 좌표로 변환하여 POINT 배열에 저장 
    POINT pts[3];
    pts[0].x = RatioToX(x1, rect.right);  pts[0].y = RatioToY(y1, rect.bottom);
    pts[1].x = RatioToX(x2, rect.right);  pts[1].y = RatioToY(y2, rect.bottom);
    pts[2].x = RatioToX(x3, rect.right);  pts[2].y = RatioToY(y3, rect.bottom);

    // 3. Polygon 함수로 삼각형 그리기 [cite: 1266, 1275]
    Polygon(hDC, pts, 3);

    // 4. GDI 객체 복구 및 삭제 [cite: 1405, 1406, 1439]
    SelectObject(hDC, oldPen);
    SelectObject(hDC, oldBrush);
    DeleteObject(hPen);
    if (useFill) DeleteObject(hBrush);
}