#include "VertexBuffer.h"
#include "GameObject.h"
#include "BaseObject.h"

std::vector<DirectX::XMFLOAT3> VertexBuffer::m_LocalPoints[SHAPE_END];

VertexBuffer::VertexBuffer(GameObject* owner) : Component(owner) { }

// 도형의 기본 생김새(로컬 정점)를 최초 한 번만 만듭니다.
void VertexBuffer::Build_Geometrys() {
    for (int i = 0; i < SHAPE_END; ++i) {
        m_LocalPoints[i].clear();

        int sides = 0;

        switch (i) {
        case TRI: sides = 3; break;
        case QUAD: sides = 4; break;
        case PENTAGON: sides = 5; break;
        case CIRCLE: sides = 32; break;
        }
        if (sides == 4) {
            XMFLOAT3 localpos{ 0.5f,0.5,0.f };
            m_LocalPoints[QUAD].push_back(localpos);
            localpos = { -0.5,0.5 ,0.f };
            m_LocalPoints[QUAD].push_back(localpos);
            localpos = { -0.5,-0.5,0.f };
            m_LocalPoints[QUAD].push_back(localpos);
            localpos = { 0.5,-0.5 ,0.f };
            m_LocalPoints[QUAD].push_back(localpos);
            continue;
        }

        // 크기가 1.0(반지름 0.5)인 기본 도형을 원점(0,0)을 기준으로 생성합니다.
        float radius = 0.5f;
        float fAngleStep = 360.0f / static_cast<float>(sides);

        for (int j = 0; j < sides; ++j) {
            // 기준 각도 90도 (위쪽)
            float fLocalRad = (90.0f + (j * fAngleStep)) * (DirectX::XM_PI / 180.0f);

            DirectX::XMFLOAT3 localPos;
            localPos.x = radius * cosf(fLocalRad);
            localPos.y = radius * sinf(fLocalRad); // 2D 로컬 y축
            localPos.z = 0.0f;

            m_LocalPoints[i].push_back(localPos);
        }
    }    
}


VertexBuffer* VertexBuffer::Set_Shape(SHAPE s) {

    switch (s) {
    case SHAPE::TRI:
        m_Shape = SHAPE::TRI;
        m_Points.resize(3);
        break;
    case SHAPE::QUAD:
        m_Shape = SHAPE::QUAD;
        m_Points.resize(4);
        break;
    case SHAPE::PENTAGON:
        m_Shape = SHAPE::PENTAGON;
        m_Points.resize(5);
        break;
    case SHAPE::CIRCLE:
        m_Shape = SHAPE::CIRCLE;
        m_Points.resize(32);
        break;
    }

    return this;
}

// shape.cpp 내부
VertexBuffer* VertexBuffer::Make_Points(DirectX::FXMMATRIX worldMatrix, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) {
    if (m_LocalPoints[m_Shape].empty()) return nullptr;

    // 1. 월드 행렬 가져오기
    //DirectX::XMMATRIX matWorld = m_pTransform->Get_WorldMatrix();

    // 2. 대망의 WVP 결합 행렬! (순서가 반드시 W -> V -> P 여야 합니다)
    DirectX::XMMATRIX matWVP = worldMatrix * viewMatrix * projMatrix;

    float fWinCX = static_cast<float>(g_WinInfo.WinCX);
    float fWinCY = static_cast<float>(g_WinInfo.WinCY);

    for (size_t i = 0; i < m_LocalPoints[m_Shape].size(); ++i) {
        DirectX::XMVECTOR vLocal = DirectX::XMLoadFloat3(&m_LocalPoints[m_Shape][i]);

        // 3. WVP 결합 행렬을 로컬 정점에 단 한 번만 곱합니다! 
        // (위치, 회전, 크기, 카메라 시점, 비율 압축이 이 한 줄로 모두 적용됨)
        DirectX::XMVECTOR vFinal = DirectX::XMVector3TransformCoord(vLocal, matWVP);

        // 4. 투영된 결과는 -1.0 ~ 1.0 (NDC 공간) 상태입니다.
        float ndcX = DirectX::XMVectorGetX(vFinal);
        float ndcY = DirectX::XMVectorGetY(vFinal);

        // 5. 뷰포트 변환: -1 ~ 1 범위를 화면 픽셀(0 ~ WinCX)로 매핑합니다.
        // WinAPI 특성상 Y축은 아래로 갈수록 커지므로 뒤집어 줍니다.
        ndcX += 1.f;
        ndcX *= 0.5f;

        ndcY -= 1.f;
        ndcY *= -0.5f;

        float screenX = ndcX * fWinCX;
        float screenY = ndcY * fWinCY;

        m_Points[i].x = static_cast<long>(screenX);
        m_Points[i].y = static_cast<long>(screenY);
    }

    return this;
}