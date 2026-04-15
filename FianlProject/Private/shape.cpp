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
	float fRadiusX = m_Width * 0.5f;
	float fRadiusY = m_Height * 0.5f;

	// 2. 3등분 각도 (360도 / 3 = 120도씩)
	// 90도(위쪽)부터 시작하려면 시작 각도를 90도로 잡습니다.
	float fStartAngle = 210.f + m_Rot;

	for (int i = 0; i < 3; ++i)
	{
		// 각도를 라디안으로 변환
		float fRadian = (fStartAngle + (i * 120.0f)) * (M_PI / 180.0f);

		// 타원 방정식으로 비율 좌표(0.0 ~ 1.0) 계산
		float fRatioX = m_CenterPoint.fX + fRadiusX * cosf(fRadian);
		float fRatioY = m_CenterPoint.fY + fRadiusY * sinf(fRadian);

		m_Points[i].x = RatioToX(fRatioX, g_WinInfo.WinCX);
		m_Points[i].y = RatioToY(fRatioY, g_WinInfo.WinCY);
	}

	return this;
}
