#include "shape.h"

Shape* Shape::Set_CenterPoint(FPOINT p)
{
	m_CenterPoint = p;
	return this;
}

Shape* Shape::Set_Width_Height(float w, float h)
{
	m_Width = w; m_Height = h;
	return this;
}

Shape* Shape::Set_Shape(SHAPE s)
{
	switch (s) {
	case TRI:
		m_Points.resize(3);
		break;

	case QUAD:
		m_Points.resize(4);
		break;

	case CIRCLE:
		m_Points.resize(32);
		break;

	case PENTAGON:
		m_Points.resize(5);
		break;
	}

	return this;
}

Shape* Shape::Set_Rot(float R)
{
	m_Rot = R; return this;
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

Shape* Tri::Make_Points()
{
	// 1. 기준 척도를 화면의 가로 길이(WinCX) 하나로 통일합니다.
	// 이렇게 하면 m_Width와 m_Height가 같을 때 완벽한 정삼각형 형태를 유지합니다.
	float fBaseScale = static_cast<float>(g_WinInfo.WinCX);

	float fPixelRadiusX = (m_Width * 0.5f) * fBaseScale;
	float fPixelRadiusY = (m_Height * 0.5f) * fBaseScale; // WinCY 대신 fBaseScale 사용

	// 2. 중심점은 여전히 화면상의 비율 위치(0.0~1.0)에 있어야 하므로 각각의 크기를 곱합니다.
	float fCenterPX = m_CenterPoint.fX * g_WinInfo.WinCX;
	float fCenterPY = (1.0f - m_CenterPoint.fY) * g_WinInfo.WinCY;

	// 3. 회전량 라디안 변환
	float fRotRad = m_Rot * (M_PI / 180.0f);
	float fCosRot = cosf(fRotRad);
	float fSinRot = sinf(fRotRad);

	float fBaseAngles[3] = { 90.0f, 210.0f, 330.0f };

	for (int i = 0; i < 3; ++i)
	{
		// 4. 로컬 픽셀 좌표 계산
		float fLocalRad = fBaseAngles[i] * (M_PI / 180.0f);
		float fLocalPX = fPixelRadiusX * cosf(fLocalRad);
		float fLocalPY = fPixelRadiusY * sinf(fLocalRad);

		// 5. 회전 행렬 적용 (모양 보존)
		float fRotatedPX = fLocalPX * fCosRot - fLocalPY * fSinRot;
		float fRotatedPY = fLocalPX * fSinRot + fLocalPY * fCosRot;

		// 6. 최종 픽셀 위치 결정
		m_Points[i].x = static_cast<long>(fCenterPX + fRotatedPX);
		m_Points[i].y = static_cast<long>(fCenterPY - fRotatedPY);
	}

	return this;
}

Shape* Quad::Make_Points() {
	float fBaseScale = static_cast<float>(g_WinInfo.WinCX); //
	float fRadiusX = (m_Width * 0.5f) * fBaseScale;
	float fRadiusY = (m_Height * 0.5f) * fBaseScale;

	float fCenterPX = m_CenterPoint.fX * g_WinInfo.WinCX; //
	float fCenterPY = (1.0f - m_CenterPoint.fY) * g_WinInfo.WinCY; //

	float fRotRad = m_Rot * (M_PI / 180.0f);
	float fCosRot = cosf(fRotRad), fSinRot = sinf(fRotRad);

	// 사각형은 45도부터 시작하여 90도씩 증가
	for (int i = 0; i < 4; ++i) {
		float fLocalRad = (45.0f + (i * 90.0f)) * (M_PI / 180.0f);
		float fLocalPX = fRadiusX * cosf(fLocalRad);
		float fLocalPY = fRadiusY * sinf(fLocalRad);

		float fRotatedPX = fLocalPX * fCosRot - fLocalPY * fSinRot;
		float fRotatedPY = fLocalPX * fSinRot + fLocalPY * fCosRot;

		m_Points[i].x = static_cast<long>(fCenterPX + fRotatedPX);
		m_Points[i].y = static_cast<long>(fCenterPY - fRotatedPY);
	}
	return this;
}

Shape* Pentagon::Make_Points() {
	float fBaseScale = static_cast<float>(g_WinInfo.WinCX);
	float fRadiusX = (m_Width * 0.5f) * fBaseScale;
	float fRadiusY = (m_Height * 0.5f) * fBaseScale;
	float fCenterPX = m_CenterPoint.fX * g_WinInfo.WinCX;
	float fCenterPY = (1.0f - m_CenterPoint.fY) * g_WinInfo.WinCY;

	float fRotRad = m_Rot * (M_PI / 180.0f);
	float fCosRot = cosf(fRotRad), fSinRot = sinf(fRotRad);

	// 오각형은 72도씩 증가
	for (int i = 0; i < 5; ++i) {
		float fLocalRad = (90.0f + (i * 72.0f)) * (M_PI / 180.0f);
		float fLocalPX = fRadiusX * cosf(fLocalRad);
		float fLocalPY = fRadiusY * sinf(fLocalRad);

		float fRotatedPX = fLocalPX * fCosRot - fLocalPY * fSinRot;
		float fRotatedPY = fLocalPX * fSinRot + fLocalPY * fCosRot;

		m_Points[i].x = static_cast<long>(fCenterPX + fRotatedPX);
		m_Points[i].y = static_cast<long>(fCenterPY - fRotatedPY);
	}
	return this;
}

Shape* Circle::Make_Points() {
	float fBaseScale = static_cast<float>(g_WinInfo.WinCX);
	float fRadiusX = (m_Width * 0.5f) * fBaseScale;
	float fRadiusY = (m_Height * 0.5f) * fBaseScale;
	float fCenterPX = m_CenterPoint.fX * g_WinInfo.WinCX;
	float fCenterPY = (1.0f - m_CenterPoint.fY) * g_WinInfo.WinCY;

	float fRotRad = m_Rot * (M_PI / 180.0f);
	float fCosRot = cosf(fRotRad), fSinRot = sinf(fRotRad);

	bool isPie = (m_PieAngle != 360.0f);

	// 1. 하드코딩된 64 대신, 실제 벡터의 크기를 가져와서 기준으로 삼습니다.
	int totalPoints = static_cast<int>(m_Points.size());
	if (totalPoints <= 1) return this; // 방어 코드

	if (isPie) {
		// --- [파이 모양 (팩맨)] ---
		// [0]번은 중심점, 나머지 점들은 호(Arc)를 그립니다.
		int arcPointCount = totalPoints - 1;

		m_Points[0].x = static_cast<long>(fCenterPX);
		m_Points[0].y = static_cast<long>(fCenterPY);

		for (int i = 0; i < arcPointCount; ++i) {
			// 명시적으로 float 캐스팅을 하여 의도치 않은 정수 나눗셈(소수점 버림)을 방지합니다.
			float fLocalRad = (i * (m_PieAngle / static_cast<float>(arcPointCount - 1))) * (M_PI / 180.0f);

			float fLocalPX = fRadiusX * cosf(fLocalRad);
			float fLocalPY = fRadiusY * sinf(fLocalRad);

			float fRotatedPX = fLocalPX * fCosRot - fLocalPY * fSinRot;
			float fRotatedPY = fLocalPX * fSinRot + fLocalPY * fCosRot;

			// 인덱스 1번부터 차례대로 넣습니다.
			m_Points[i + 1].x = static_cast<long>(fCenterPX + fRotatedPX);
			m_Points[i + 1].y = static_cast<long>(fCenterPY - fRotatedPY);
		}
	}
	else {
		// --- [완전한 원 (360도)] ---
		// 중심점 없이 모든 점을 원의 테두리에만 사용합니다.
		for (int i = 0; i < totalPoints; ++i) {
			float fLocalRad = (i * (m_PieAngle / static_cast<float>(totalPoints - 1))) * (M_PI / 180.0f);

			float fLocalPX = fRadiusX * cosf(fLocalRad);
			float fLocalPY = fRadiusY * sinf(fLocalRad);

			float fRotatedPX = fLocalPX * fCosRot - fLocalPY * fSinRot;
			float fRotatedPY = fLocalPX * fSinRot + fLocalPY * fCosRot;

			// 인덱스 0번부터 끝까지 테두리를 그립니다.
			m_Points[i].x = static_cast<long>(fCenterPX + fRotatedPX);
			m_Points[i].y = static_cast<long>(fCenterPY - fRotatedPY);
		}
	}

	return this;
}