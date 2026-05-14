#include "Mouse.h"
#include "RawInput.hpp"
#include "VertexBuffer.h"
#include "Sprite.h"
#include <algorithm>

// 전역 윈도우 정보 (크기 제한용)
extern WinInfo g_WinInfo;

GameObject* Mouse::Initialize()
{
    // 1. 스프라이트 컴포넌트 추가 및 커서 이미지 로드
    m_Sprite = Add_Component<Sprite>();
    m_Sprite->Load_Image(L"../Sprites/cursor.png"); // 구한 커서 이미지 경로

    // 2. 버텍스 버퍼 추가 (사각형)
    m_VB = Add_Component<VertexBuffer>();
    m_VB->Set_Shape(QUAD);

    // 3. 마우스 초기 위치를 화면 정중앙으로 세팅
    m_AbsoluteX = 0;//g_WinInfo.WinCX / 2.0f;
    m_AbsoluteY = 0;//g_WinInfo.WinCY / 2.0f;

    // 커서의 크기(예: 32x32)와 위치 설정
    m_Transform->Set_TransformState(0, 0, 16, 16, 0);
    m_Transform->Set_Position(m_AbsoluteX, m_AbsoluteY);

    return this;
}

void Mouse::Update(float dt)
{
    // 1. Raw Input의 Delta 값을 현재 위치에 누적시킵니다.
    // (마우스 민감도를 조절하고 싶다면 여기에 특정 float 값을 곱해주면 됩니다)
    m_AbsoluteX += g_RawInput->Get_MouseDeltaX();
    m_AbsoluteY -= g_RawInput->Get_MouseDeltaY();

    

    // 3. 계산된 최종 위치를 Transform 컴포넌트에 갱신
    m_Transform->Set_Position(m_AbsoluteX, m_AbsoluteY);
}

void Mouse::Late_Update(float dt)
{
}

void Mouse::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
    // 렌더링 파이프라인 적용
    m_VB->Make_Points(m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
    m_Sprite->Draw(hDC, m_VB->Get_Points());
}