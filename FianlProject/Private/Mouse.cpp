#include "Mouse.h"
#include "RawInput.hpp"
#include "VertexBuffer.h"
#include "Sprite.h"
#include <algorithm>
#include "SceneManager.h"

// 전역 윈도우 정보 (크기 제한용)
extern WinInfo g_WinInfo;

GameObject* Mouse::Initialize()
{
    __super::Initialize();
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
    m_Transform->Set_TransformState(0, 0, 100, 100, 0);
    m_Transform->Set_Position(m_AbsoluteX, m_AbsoluteY);

    Set_LayerType(eLayerType::UI);

    m_MainCamera = (Camera*)SceneManager::Get_Instance()->Get_ActiveScene()->Get_Object("Camera0");
    m_MainCameraTransform = m_MainCamera->Get_Transform();
    return this;
}

void Mouse::Update(float dt)
{
    // 1. Raw Input의 Delta 값을 현재 위치에 누적시킵니다.
    // (마우스 민감도를 조절하고 싶다면 여기에 특정 float 값을 곱해주면 됩니다)
    m_AbsoluteX += g_RawInput->Get_MouseDeltaX();
    m_AbsoluteY -= g_RawInput->Get_MouseDeltaY();

    int halfX = (int)(g_WinInfo.WinCX * 0.5f);
    if (m_AbsoluteX >= halfX) {
        m_AbsoluteX = halfX;
        m_MainCameraTransform->Go_Right(2000 * dt);
    }
    if (m_AbsoluteX <= -halfX) {
        m_AbsoluteX = -halfX;
        m_MainCameraTransform->Go_Left(2000 * dt);
    }

    int halfY = (int)(g_WinInfo.WinCY * 0.5f);
    if (m_AbsoluteY >= halfY) {
        m_AbsoluteY = halfY;
        m_MainCameraTransform->Go_Up(2000 * dt);
    }
    if (m_AbsoluteY <= -halfY) {
        m_AbsoluteY = -halfY;
        m_MainCameraTransform->Go_Down(2000 * dt);
    }

    Lock_Cursor();
}

void Mouse::Late_Update(float dt)
{
    m_Transform->Set_Position(m_AbsoluteX, m_AbsoluteY );
    __super::Late_Update(dt);
}

void Mouse::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
    // 렌더링 파이프라인 적용
    m_VB->Make_Points(m_Transform->Get_WorldMatrix(), XMMatrixIdentity(), projMatrix);
    m_Sprite->Draw(hDC, m_VB->Get_Points());
}

void Mouse::Lock_Cursor()
{
    RECT rect;
    // 1. 현재 창의 클라이언트 영역(게임 화면 크기)을 얻어옵니다.
    GetClientRect(g_WinInfo.hWnd, &rect);

    // 2. 클라이언트 좌표(창 기준)를 스크린 좌표(모니터 기준)로 변환합니다.
    POINT ptLT = { rect.left, rect.top };
    POINT ptRB = { rect.right, rect.bottom };
    ClientToScreen(g_WinInfo.hWnd, &ptLT);
    ClientToScreen(g_WinInfo.hWnd, &ptRB);

    // 3. 변환된 스크린 좌표로 RECT를 다시 세팅합니다.
    SetRect(&rect, ptLT.x, ptLT.y, ptRB.x, ptRB.y);

    // 4. 해당 RECT 영역 안으로 마우스를 가둡니다.
    ClipCursor(&rect);
}
