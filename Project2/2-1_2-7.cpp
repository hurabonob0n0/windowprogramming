#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <stdio.h>

// 실습 관리를 위한 전역 변수
int g_modes[] = { 21, 22, 23, 24, 242, 25, 26, 27 };
int g_currentIdx = 0;

// 실습 2-5, 2-7 공용 데이터 (입력 문자열)
TCHAR g_str[10][31];
int g_curRow = 0, g_curCol = 0;
COLORREF g_inputColor = RGB(0, 0, 0);

// 실습 2-6용 구조체 및 데이터
struct DrawingData {
    int x, y, n, count;
    COLORREF textCol, backCol;
};
DrawingData g_drawList[10];
int g_drawCount = 0;
TCHAR g_inputBuf[100] = _T(""); // 2-6 입력용 버퍼

// 모드 전환 시 데이터 초기화 함수
void ResetData() {
    for (int i = 0; i < 10; i++) g_str[i][0] = _T('\0');
    g_curRow = g_curCol = 0;
    g_drawCount = 0;
    g_inputBuf[0] = _T('\0');
    g_inputColor = RGB(rand() % 256, rand() % 256, rand() % 256);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    srand((unsigned int)time(NULL));
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInst,
                      LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
                      (HBRUSH)GetStockObject(WHITE_BRUSH), NULL, L"WinProg_Lab2", NULL };
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow(L"WinProg_Lab2", L"윈도우 프로그래밍 2장 통합 실습 (F11:이전, F12:다음)",
        WS_OVERLAPPEDWINDOW, 100, 50, 800, 600, NULL, NULL, hInst, NULL);
    ShowWindow(hWnd, nShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    TCHAR buf[256];
    SIZE sz;

    switch (iMsg) {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 2, 18); // [cite: 1972]
        ShowCaret(hWnd); // [cite: 1976]
        break;

    case WM_KEYDOWN:
        if (wParam == VK_F11) { // 다음 문제 [cite: 1675]
            g_currentIdx = (g_currentIdx + 1) % 8;
            ResetData();
            InvalidateRect(hWnd, NULL, TRUE); // 
        }
        else if (wParam == VK_F10) { // 이전 문제
            g_currentIdx = (g_currentIdx - 1 + 8) % 8;
            ResetData();
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wParam == VK_ESCAPE) { // ESC: 종료 또는 리셋 [cite: 1570, 1600]
            if (g_modes[g_currentIdx] == 25) PostQuitMessage(0);
            else { ResetData(); InvalidateRect(hWnd, NULL, TRUE); }
        }
        break;

    case WM_CHAR:
    {
        int mode = g_modes[g_currentIdx];
        if (mode >= 25) { // 입력이 필요한 모드들
            if (wParam == VK_BACK) { // 백스페이스 처리 [cite: 1675, 1781]
                if (g_curCol > 0) g_curCol--;
                g_str[g_curRow][g_curCol] = _T('\0');
            }
            else if (wParam == VK_RETURN) { // 엔터 처리 [cite: 1675, 1810]
                if (mode == 26) { // 2-6: 입력값 파싱
                    if (g_drawCount < 10) {
                        _stscanf(g_str[0], _T("%d %d %d %d"), &g_drawList[g_drawCount].x, &g_drawList[g_drawCount].y,
                            &g_drawList[g_drawCount].n, &g_drawList[g_drawCount].count);
                        g_drawList[g_drawCount].textCol = RGB(rand() % 256, rand() % 256, rand() % 256);
                        g_drawList[g_drawCount].backCol = RGB(rand() % 256, rand() % 256, rand() % 256);
                        g_drawCount++;
                    }
                    g_curCol = 0; g_str[0][0] = _T('\0');
                }
                else {
                    g_curRow = (g_curRow + 1) % 10;
                    g_curCol = 0;
                }
            }
            else { // 문자 입력
                if (g_curCol < 30) {
                    g_str[g_curRow][g_curCol++] = (TCHAR)wParam;
                    g_str[g_curRow][g_curCol] = _T('\0');
                }
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    }
        
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps); // [cite: 1090]
        int curMode = g_modes[g_currentIdx];

        // 상단에 현재 모드 표시
        wsprintf(buf, L"현재 실습: %d-%d (F11/F12로 이동)", 2, (curMode > 100 ? 4 : curMode % 10));
        SetTextColor(hdc, RGB(0, 0, 255));
        TextOut(hdc, 10, 10, buf, _tcslen(buf));
        SetTextColor(hdc, RGB(0, 0, 0));

        if (curMode == 21) { // [실습 2-1] 랜덤 좌표 [cite: 1470]
            for (int i = 1; i <= 15; i++) {
                int rx = rand() % 600, ry = rand() % 400 + 50;
                wsprintf(buf, L"%d: (%d, %d)", i, rx, ry);
                TextOut(hdc, rx, ry, buf, _tcslen(buf));
            }
            TextOut(hdc, 400, 300, L"0: (400, 300)", 13);
        }
        else if (curMode == 22 || curMode == 23) { // [실습 2-2, 2-3] 구구단 [cite: 1488, 1501]
            int nDiv = rand() % 15 + 2;
            int cellW = 800 / nDiv;
            int isOdd = (rand() % 2 != 0);
            for (int d = 2; d <= nDiv + 1; d++) {
                if (isOdd) SetTextColor(hdc, RGB(d * 20, 0, 0)); // 홀수: 단별 색상 
                for (int i = 1; i <= 9; i++) {
                    if (!isOdd) SetTextColor(hdc, RGB(0, i * 25, 0)); // 짝수: 줄별 색상 
                    wsprintf(buf, L"%d*%d=%d", d, i, d * i);
                    TextOut(hdc, (d - 2) * cellW, 50 + (i * 20), buf, _tcslen(buf));
                }
            }
        }
        else if (curMode == 24) { // [실습 2-4] 한 개의 단 출력 [cite: 1512]
            int dan = rand() % 19 + 2;
            int pos = rand() % dan;
            SetTextColor(hdc, RGB(255, 0, 0));
            for (int i = 1; i <= 9; i++) {
                wsprintf(buf, L"%*s%d*%d=%d", (i - 1) * 3, L"", dan, i, dan * i); // 들여쓰기 [cite: 1517]
                TextOut(hdc, (800 / dan) * pos, 50 + (i * 20), buf, _tcslen(buf));
            }
        }
        else if (curMode == 242) { // [실습 2-4-2] 문자 사각형 [cite: 1534]
            RECT r = { rand() % 400, rand() % 300, 0, 0 };
            r.right = r.left + (rand() % 300 + 100); r.bottom = r.top + (rand() % 200 + 100);
            SetTextColor(hdc, RGB(rand() % 256, rand() % 256, rand() % 256));
            for (int y = r.top; y < r.bottom; y += 20)
                for (int x = r.left; x < r.right; x += 15) TextOut(hdc, x, y, L"A", 1);
        }
        else if (curMode == 26) { // [실습 2-6] 숫자 그리기 [cite: 1582]
            TextOut(hdc, 10, 500, L"입력(x y n count):", 14);
            TextOut(hdc, 150, 500, g_str[0], _tcslen(g_str[0]));
            for (int i = 0; i < g_drawCount; i++) {
                SetTextColor(hdc, g_drawList[i].textCol); SetBkColor(hdc, g_drawList[i].backCol);
                for (int r = 0; r < g_drawList[i].count; r++) {
                    for (int c = 0; c < g_drawList[i].count; c++) {
                        wsprintf(buf, L"%d", g_drawList[i].n);
                        TextOut(hdc, g_drawList[i].x + (c * 20), g_drawList[i].y + (r * 20), buf, _tcslen(buf));
                    }
                }
            }
        }
        else if (curMode == 27 || curMode == 25) { // [실습 2-5, 2-7] 메모장 [cite: 1638]
            SetTextColor(hdc, g_inputColor);
            for (int i = 0; i < 10; i++) TextOut(hdc, 10, 50 + (i * 20), g_str[i], _tcslen(g_str[i]));
            GetTextExtentPoint32(hdc, g_str[g_curRow], _tcslen(g_str[g_curRow]), &sz); // [cite: 2004]
            SetCaretPos(10 + sz.cx, 50 + (g_curRow * 20)); // [cite: 1979]
        }

        EndPaint(hWnd, &ps); // [cite: 1094]
    }
    break;

    case WM_DESTROY:
        HideCaret(hWnd); DestroyCaret(); // [cite: 1988, 1991]
        PostQuitMessage(0); // [cite: 1226]
        break;
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}