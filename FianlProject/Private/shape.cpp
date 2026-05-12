#include "shape.h"
#include "GameObject.h"
#include "BaseObject.h"

Shape::Shape(GameObject* owner) : Component(owner) { }

void Shape::Draw(HDC hDC,const vector<POINT> Points)
{
	HPEN hPen = CreatePen(PS_SOLID, m_LineWidth, m_LineColor);
	HPEN oldPen = (HPEN)SelectObject(hDC, hPen);

	// 2. 브러시(내부 채우기) 생성 및 선택
	HBRUSH hBrush = m_UseFill ? CreateSolidBrush(m_FillColor) : (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	Polygon(hDC, &Points[0], (int)Points.size());

	// 4. 원래 도구로 복구 및 생성한 도구 삭제 (중요!)
	SelectObject(hDC, oldPen);
	SelectObject(hDC, oldBrush);
	DeleteObject(hPen);
	if (m_UseFill) DeleteObject(hBrush);
}