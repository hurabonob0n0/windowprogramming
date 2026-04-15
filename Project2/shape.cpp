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

void Tri::Draw()
{
}

Shape* Tri::Make_Points()
{
	__super::Set_Shape(TRI);

	float hW = m_Width * 0.5f;
	float hH = m_Height * 0.5f;



	/*m_Points[0] = { m_CenterPoint.fX, m_CenterPoint.fY + hH };
	m_Points[1] = { m_CenterPoint.fX - hW, m_CenterPoint.fY - hH };
	m_Points[2] = { m_CenterPoint.fX + hW, m_CenterPoint.fY - hH };*/

	return this;
}
