#pragma once
#include <windows.h>
#include <vector>

class RawInput
{
public:
    RawInput() = default;
    ~RawInput() = default;

public:
    HRESULT Initialize(HWND hWnd) {
        RAWINPUTDEVICE Rid[2] = {};

        // 키보드 등록
        Rid[0].usUsagePage = 0x01;
        Rid[0].usUsage = 0x06;
        Rid[0].dwFlags = RIDEV_INPUTSINK;
        Rid[0].hwndTarget = hWnd;

        // 마우스 등록
        Rid[1].usUsagePage = 0x01;
        Rid[1].usUsage = 0x02;
        Rid[1].dwFlags = RIDEV_INPUTSINK;
        Rid[1].hwndTarget = hWnd;

        if (!RegisterRawInputDevices(Rid, 2, sizeof(RAWINPUTDEVICE)))
            return E_FAIL;

        return S_OK;
    };

    // WndProc에서 WM_INPUT 메시지가 올 때마다 호출
    void Update_InputDev(LPARAM lParam) {
        UINT dwSize = 0;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

        std::vector<BYTE> lpb(dwSize);
        if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb.data(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
            return;

        RAWINPUT* raw = (RAWINPUT*)lpb.data();

        if (raw->header.dwType == RIM_TYPEKEYBOARD)
        {
            UINT vkey = raw->data.keyboard.VKey;
            if (vkey >= 256) return; // 배열 인덱스 초과 방지

            // RI_KEY_BREAK 플래그가 없으면 눌린 것(Down), 있으면 뗀 것(Up)
            bool isDown = !(raw->data.keyboard.Flags & RI_KEY_BREAK);
            m_bKeyCurrState[vkey] = isDown;
        }
        else if (raw->header.dwType == RIM_TYPEMOUSE)
        {
            RAWMOUSE& mouse = raw->data.mouse;

            m_MouseDelta[0] += static_cast<SHORT>(mouse.lLastX);
            m_MouseDelta[1] += static_cast<SHORT>(mouse.lLastY);

            if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)   m_bMouseCurrState[0] = true;
            if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)     m_bMouseCurrState[0] = false;

            if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)  m_bMouseCurrState[1] = true;
            if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)    m_bMouseCurrState[1] = false;

            if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) m_bMouseCurrState[2] = true;
            if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)   m_bMouseCurrState[2] = false;

            if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)      m_bMouseCurrState[3] = true;
            if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)        m_bMouseCurrState[3] = false;

            if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)      m_bMouseCurrState[4] = true;
            if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)        m_bMouseCurrState[4] = false;

            if (mouse.usButtonFlags & RI_MOUSE_WHEEL)
                m_MouseDelta[2] += static_cast<SHORT>(mouse.usButtonData);
        }
    };

    // 메인 루프의 가장 마지막(프레임 끝)에서 호출
    void EndFrame() {
        // 현재 프레임의 상태를 이전 프레임 상태로 복사
        memcpy(m_bKeyPrevState, m_bKeyCurrState, sizeof(m_bKeyCurrState));
        memcpy(m_bMousePrevState, m_bMouseCurrState, sizeof(m_bMouseCurrState));

        // 마우스 이동량은 매 프레임 끝마다 반드시 초기화
        ZeroMemory(m_MouseDelta, sizeof(m_MouseDelta));
    };

    bool Key_Down(UINT vkey) const { return m_bKeyCurrState[vkey] && !m_bKeyPrevState[vkey]; }
    bool Key_Up(UINT vkey) const { return !m_bKeyCurrState[vkey] && m_bKeyPrevState[vkey]; }
    bool Key_Pressing(UINT vkey) const { return m_bKeyCurrState[vkey];}
    bool Mouse_Down(UINT button) const { return m_bMouseCurrState[button] && !m_bMousePrevState[button]; }
    bool Mouse_Up(UINT button) const { return !m_bMouseCurrState[button] && m_bMousePrevState[button]; }
    bool Mouse_Pressing(UINT button) const { return m_bMouseCurrState[button]; }

    SHORT Get_MouseDeltaX() const { return m_MouseDelta[0]; }
    SHORT Get_MouseDeltaY() const { return m_MouseDelta[1]; }
    SHORT Get_MouseWheelDelta() const { return m_MouseDelta[2]; }

private:
    bool m_bKeyCurrState[256] = {};  // 현재 프레임 키보드 상태
    bool m_bKeyPrevState[256] = {};  // 이전 프레임 키보드 상태

    bool m_bMouseCurrState[5] = {};  // 현재 마우스 버튼 상태 (0:좌, 1:우, 2:중, 3:X1, 4:X2)
    bool m_bMousePrevState[5] = {};  // 이전 프레임 마우스 버튼 상태

    SHORT m_MouseDelta[3] = {};      // 마우스 이동량 (X, Y, Wheel)
};