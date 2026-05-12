#pragma once
#include "Utils.hpp"
#include "Component.h"
#include "Transform.h"

class Shape : public Component{
public:
	Shape(GameObject* owner);
	virtual ~Shape() {};

public:
	void			Draw(HDC hDC, const vector<POINT> Points);
	Shape*			Set_Line(COLORREF color, int width = 3) {
		m_LineColor = color;
		m_LineWidth = width;
		return this;
	}
	Shape*			Set_Fill(COLORREF color, bool useFill = true) {
		m_FillColor = color;
		m_UseFill = useFill;
		return this;
	}

protected:
	COLORREF		m_LineColor{ RGB(0, 0, 0) };		// 기본 검은색
	int				m_LineWidth{ 3 };					// 기본 두께 1
	COLORREF		m_FillColor{ RGB(255, 255, 255) };	// 기본 흰색
	bool			m_UseFill{ true };					// 채우기 여부
};