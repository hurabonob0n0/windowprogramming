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
		m_Points.resize(64);
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

	// 파이 모양일 경우 중심점을 포함하기 위해 점 개수 조절
	bool isPie = (m_PieAngle < 360.0f);
	int pointCount = 64; // SHAPE::CIRCLE의 기본 크기

	// m_Points 크기 재조정 (파이 형태면 중심점 + 1)
	m_Points.assign(isPie ? pointCount + 1 : pointCount, { 0, 0 });

	int startIndex = 0;
	if (isPie) {
		// 첫 번째 점을 중심점으로 설정
		m_Points[0].x = static_cast<long>(fCenterPX);
		m_Points[0].y = static_cast<long>(fCenterPY);
		startIndex = 1;
	}

	for (int i = 0; i < pointCount; ++i) {
		// m_PieAngle 내에서 점들을 등분하여 배치
		float fLocalRad = (i * (m_PieAngle / (pointCount - 1))) * (M_PI / 180.0f);
		float fLocalPX = fRadiusX * cosf(fLocalRad);
		float fLocalPY = fRadiusY * sinf(fLocalRad);

		float fRotatedPX = fLocalPX * fCosRot - fLocalPY * fSinRot;
		float fRotatedPY = fLocalPX * fSinRot + fLocalPY * fCosRot;

		m_Points[startIndex + i].x = static_cast<long>(fCenterPX + fRotatedPX);
		m_Points[startIndex + i].y = static_cast<long>(fCenterPY - fRotatedPY);
	}
	return this;
}