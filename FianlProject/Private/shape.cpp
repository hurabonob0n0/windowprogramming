#include "shape.h"

std::vector<DirectX::XMFLOAT3> Shape::m_LocalPoints[SHAPE_END];

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

Shape* Shape::Make_Points() {
    if (!m_pTransform || m_LocalPoints[m_Shape].empty()) return this;

    // 1. 연결된 Transform에서 4x4 월드 행렬을 가져옵니다.
    DirectX::XMMATRIX matWorld = m_pTransform->Get_WorldMatrix();

    //  핵심: 행렬에서 중심점(Translation) 위치만 따로 추출합니다.
    float tx = DirectX::XMVectorGetX(matWorld.r[3]);
    float ty = DirectX::XMVectorGetY(matWorld.r[3]);

    // 2. 화면 비율 스케일업을 위한 정보
    float fWinCX = static_cast<float>(g_WinInfo.WinCX);
    float fWinCY = static_cast<float>(g_WinInfo.WinCY);

    for (size_t i = 0; i < m_LocalPoints[m_Shape].size(); ++i) {
        DirectX::XMVECTOR vLocal = DirectX::XMLoadFloat3(&m_LocalPoints[m_Shape][i]);
        DirectX::XMVECTOR vWorld = DirectX::XMVector3TransformCoord(vLocal, matWorld);

        // 3. 중심점을 빼서 회전과 크기만 적용된 '순수 오프셋(모양)'을 구합니다.
        float offsetX = DirectX::XMVectorGetX(vWorld) - tx;
        float offsetY = DirectX::XMVectorGetY(vWorld) - ty;

        // 4. 위치(중심점)는 기존처럼 X, Y 각자의 화면 비율에 맞춥니다.
        float centerX = tx * fWinCX;
        float centerY = (1.0f - ty) * fWinCY; // Y축 뒤집기 적용

        // 5. 도형의 모양(오프셋)은 오직 X축 길이(fWinCX) 하나로만 통일하여 더해줍니다!
        // (화면 좌표계는 아래로 갈수록 +Y이므로 오프셋은 빼줍니다)
        float finalX = centerX + (offsetX * fWinCX);
        float finalY = centerY - (offsetY * fWinCX);

        m_Points[i].x = static_cast<long>(finalX);
        m_Points[i].y = static_cast<long>(finalY);
    }

    return this;
}

void Shape::Draw(HDC hDC)
{
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