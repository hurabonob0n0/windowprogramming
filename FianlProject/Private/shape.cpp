#include "shape.h"
#include "GameObject.h"
#include "BaseObject.h"

std::vector<DirectX::XMFLOAT3> Shape::m_LocalPoints[SHAPE_END];

Shape::Shape(GameObject* owner) : Component(owner) { m_pTransform = owner->Get_Transform(); }

// 도형의 기본 생김새(로컬 정점)를 최초 한 번만 만듭니다.
void Shape::Build_Geometrys() {
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
            localpos = { -0.5,0.5,0.f };
            m_LocalPoints[QUAD].push_back(localpos);
            localpos = { -0.5,-0.5,0.f };
            m_LocalPoints[QUAD].push_back(localpos);
            localpos = { 0.5,-0.5,0.f };
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

Shape* Shape::Set_Transform(Transform* pTransform)
{
    if (pTransform)
        m_pTransform = pTransform;
    else
        m_pTransform = m_Owner->Get_Transform();
    return this;
}

Shape* Shape::Set_Shape(SHAPE s) {

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

void Shape::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
    Make_Points(viewMatrix, projMatrix);

	HPEN hPen = CreatePen(PS_SOLID, m_LineWidth, m_LineColor);
	HPEN oldPen = (HPEN)SelectObject(hDC, hPen);

	// 2. 브러시(내부 채우기) 생성 및 선택
	HBRUSH hBrush = m_UseFill ? CreateSolidBrush(m_FillColor) : (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	// 3. 그리기 (계산된 점들을 사용)
	if (!m_Points.empty()) {
		Polygon(hDC, &m_Points[0], (int)m_Points.size());
	}

	// 4. 원래 도구로 복구 및 생성한 도구 삭제 (중요!)
	SelectObject(hDC, oldPen);
	SelectObject(hDC, oldBrush);
	DeleteObject(hPen);
	if (m_UseFill) DeleteObject(hBrush);
}

//// shape.cpp 내부
//Shape* Shape::Make_Points(DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) {
//    if (!m_pTransform || m_LocalPoints[m_Shape].empty()) return this;
//
//    // 1. 월드 행렬 가져오기
//    DirectX::XMMATRIX matWorld = m_pTransform->Get_WorldMatrix();
//
//    // [핵심 1] 월드 행렬에서 중심점(Translation) 위치만 따로 추출합니다.
//    float tx = DirectX::XMVectorGetX(matWorld.r[3]);
//    float ty = DirectX::XMVectorGetY(matWorld.r[3]);
//
//    // [핵심 2] 위치(tx, ty)를 (0,0)으로 초기화한 '순수 모양(크기+회전)' 행렬을 만듭니다.
//    DirectX::XMMATRIX matWorldOrigin = matWorld;
//    matWorldOrigin.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//
//    // 2. 크기와 회전에만 W -> V -> P 결합 행렬을 생성합니다! (투영 보정 적용)
//    DirectX::XMMATRIX matWVP = matWorldOrigin * viewMatrix * projMatrix;
//
//    float fWinCX = static_cast<float>(g_WinInfo.WinCX);
//    float fWinCY = static_cast<float>(g_WinInfo.WinCY);
//
//    for (size_t i = 0; i < m_LocalPoints[m_Shape].size(); ++i) {
//        DirectX::XMVECTOR vLocal = DirectX::XMLoadFloat3(&m_LocalPoints[m_Shape][i]);
//
//        // 3. 중심이 (0,0)인 상태에서 회전, 크기, 비율 압축(WVP)이 모두 적용된 오프셋을 구합니다.
//        DirectX::XMVECTOR vFinal = DirectX::XMVector3TransformCoord(vLocal, matWVP);
//
//        float ndcX = DirectX::XMVectorGetX(vFinal);
//        float ndcY = DirectX::XMVectorGetY(vFinal);
//
//        // 4. 뷰포트 변환 1: NDC 모양(오프셋)을 실제 픽셀 크기로 변환
//        // 투영 행렬이 이미 종횡비를 보정해 주었으므로, 각각 fWinCX, fWinCY를 곱해주면 정상적인 정사각형 오프셋이 나옵니다.
//        float offsetX = (ndcX * 0.5f) * fWinCX;
//        float offsetY = (ndcY * -0.5f) * fWinCY;
//
//        // 5. 뷰포트 변환 2: 중심점 위치는 기존처럼 화면 백분율(0~1)로 독립적으로 계산합니다!
//        float centerX = tx * fWinCX;
//        float centerY = (1.0f - ty) * fWinCY;
//
//        // 6. 최종 위치 = 백분율로 계산된 화면 중심점 + 완벽하게 계산된 픽셀 오프셋
//        m_Points[i].x = static_cast<long>(centerX + offsetX);
//        m_Points[i].y = static_cast<long>(centerY + offsetY);
//    }
//
//    return this;
//}

// shape.cpp 내부
Shape* Shape::Make_Points(DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) {
    if (!m_pTransform || m_LocalPoints[m_Shape].empty()) return this;

    // 1. 월드 행렬 가져오기
    DirectX::XMMATRIX matWorld = m_pTransform->Get_WorldMatrix();

    // 2. 대망의 WVP 결합 행렬! (순서가 반드시 W -> V -> P 여야 합니다)
    DirectX::XMMATRIX matWVP = matWorld * viewMatrix * projMatrix;

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


//Shape* Shape::Make_Points(DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) {
//    if (!m_pTransform || m_LocalPoints[m_Shape].empty()) return this;
//
//    // 1. 연결된 Transform에서 4x4 월드 행렬을 가져옵니다.
//    DirectX::XMMATRIX matWorld = m_pTransform->Get_WorldMatrix();
//
//    //  핵심: 행렬에서 중심점(Translation) 위치만 따로 추출합니다.
//    float tx = DirectX::XMVectorGetX(matWorld.r[3]);
//    float ty = DirectX::XMVectorGetY(matWorld.r[3]);
//
//    // 2. 화면 비율 스케일업을 위한 정보
//    float fWinCX = static_cast<float>(g_WinInfo.WinCX);
//    float fWinCY = static_cast<float>(g_WinInfo.WinCY);
//
//    for (size_t i = 0; i < m_LocalPoints[m_Shape].size(); ++i) {
//        DirectX::XMVECTOR vLocal = DirectX::XMLoadFloat3(&m_LocalPoints[m_Shape][i]);
//        DirectX::XMVECTOR vWorld = DirectX::XMVector3TransformCoord(vLocal, matWorld);
//
//        // 3. 중심점을 빼서 회전과 크기만 적용된 '순수 오프셋(모양)'을 구합니다.
//        float offsetX = DirectX::XMVectorGetX(vWorld) - tx;
//        float offsetY = DirectX::XMVectorGetY(vWorld) - ty;
//
//        // 4. 위치(중심점)는 기존처럼 X, Y 각자의 화면 비율에 맞춥니다.
//        float centerX = tx * fWinCX;
//        float centerY = (1.0f - ty) * fWinCY; // Y축 뒤집기 적용
//
//        // 5. 도형의 모양(오프셋)은 오직 X축 길이(fWinCX) 하나로만 통일하여 더해줍니다!
//        // (화면 좌표계는 아래로 갈수록 +Y이므로 오프셋은 빼줍니다)
//        float finalX = centerX + (offsetX * fWinCY);
//        float finalY = centerY - (offsetY * fWinCY);
//
//        m_Points[i].x = static_cast<long>(finalX);
//        m_Points[i].y = static_cast<long>(finalY);
//    }
//
//    return this;
//}

//Shape* Circle::Make_Points() {
//	bool isPie = (m_PieAngle != 360.0f);
//
//	// 파이 형태가 아닌 완전한 원(360도)일 경우, 부모 클래스의 통일된 로직을 재사용합니다.
//	if (!isPie) {
//		return Shape::Make_Points(); // __super::Make_Points()와 동일합니다.
//	}
//
//	// --- [파이 모양 (팩맨)] ---
//	float fBaseScale = static_cast<float>(g_WinInfo.WinCX);
//	float fRadiusX = (m_Width * 0.5f) * fBaseScale;
//	float fRadiusY = (m_Height * 0.5f) * fBaseScale;
//	float fCenterPX = m_CenterPoint.fX * g_WinInfo.WinCX;
//	float fCenterPY = (1.0f - m_CenterPoint.fY) * g_WinInfo.WinCY;
//
//	float fRotRad = m_Rot * (M_PI / 180.0f);
//	float fCosRot = cosf(fRotRad), fSinRot = sinf(fRotRad);
//
//	int totalPoints = static_cast<int>(m_Points.size());
//	if (totalPoints <= 1) return this;
//
//	int arcPointCount = totalPoints - 1;
//
//	m_Points[0].x = static_cast<long>(fCenterPX);
//	m_Points[0].y = static_cast<long>(fCenterPY);
//
//	for (int i = 0; i < arcPointCount; ++i) {
//		float fLocalRad = (i * (m_PieAngle / static_cast<float>(arcPointCount - 1))) * (M_PI / 180.0f);
//
//		float fLocalPX = fRadiusX * cosf(fLocalRad);
//		float fLocalPY = fRadiusY * sinf(fLocalRad);
//
//		float fRotatedPX = fLocalPX * fCosRot - fLocalPY * fSinRot;
//		float fRotatedPY = fLocalPX * fSinRot + fLocalPY * fCosRot;
//
//		m_Points[i + 1].x = static_cast<long>(fCenterPX + fRotatedPX);
//		m_Points[i + 1].y = static_cast<long>(fCenterPY - fRotatedPY);
//	}
//
//	return this;
//}