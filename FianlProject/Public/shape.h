#pragma once
#include "Utils.hpp"

typedef struct tagFPoint {
	float fX{ -1 };
	float fY{ -1 };

	tagFPoint& operator += (const tagFPoint& other) { fX += other.fX; fY += other.fY; return *this; }
	// 관례적으로 대입 연산자는 자기 자신의 참조를 반환합니다. (a = b = c; 가능하게 함)
	tagFPoint& operator = (const tagFPoint& other) {
		// 나 자신을 대입하는 경우(a = a)를 대비한 방어 코드
		if (this != &other) {
			fX = other.fX;
			fY = other.fY;
		}
		return *this;
	}
} FPOINT;

enum SHAPE{TRI,QUAD,CIRCLE,PENTAGON,};

//모든 도형을 다 Polygon으로 그리자

class Shape {
public:
	Shape() {};
	virtual ~Shape() {};

public:
	void Draw(HDC hDC);
	Shape* Set_CenterPoint(FPOINT p);
	Shape* Set_Width_Height(float w, float h);
	Shape* Set_Rot(float R);
	Shape* Set_Line(COLORREF color, int width = 3) {
		m_LineColor = color;
		m_LineWidth = width;
		return this;
	}
	Shape* Set_Fill(COLORREF color, bool useFill = true) {
		m_FillColor = color;
		m_UseFill = useFill;
		return this;
	}
	virtual Shape* Make_Points() = 0;

protected:
	Shape* Set_Shape(SHAPE s);

protected:
	FPOINT			m_CenterPoint{};
	float			m_Width{ -1 }, m_Height{ -1 };
	vector<POINT>	m_Points;
	float			m_Rot{ -1 };
	COLORREF		m_LineColor{ RGB(0, 0, 0) }; // 기본 검은색
	int				m_LineWidth{ 3 };           // 기본 두께 1
	COLORREF		m_FillColor{ RGB(255, 255, 255) }; // 기본 흰색
	bool			m_UseFill{ true };          // 채우기 여부
};

class Tri : public Shape {
public:
	Tri() {};

public:
	virtual Shape* Make_Points();

public:
	static Tri* Create() {
		Tri* pInstance = new Tri();
		pInstance->Set_Shape(TRI);
		return pInstance;
	}
};

class Quad : public Shape {
public:
	Quad() {}
	virtual Shape* Make_Points() override;
	static Quad* Create() {
		Quad* pInstance = new Quad();
		pInstance->Set_Shape(QUAD); //
		return pInstance;
	}
};

class Pentagon : public Shape {
public:
	Pentagon() {}
	virtual Shape* Make_Points() override;
	static Pentagon* Create() {
		Pentagon* pInstance = new Pentagon();
		pInstance->Set_Shape(PENTAGON); //
		return pInstance;
	}
};

class Circle : public Shape {
public:
	Circle() : m_PieAngle(360.0f) {} // 기본값 360도 (완전한 원)
	virtual Shape* Make_Points() override;

	// 파이 각도 설정 (메서드 체이닝)
	Circle* Set_PieAngle(float angle) { m_PieAngle = angle; return this; }

	static Circle* Create() {
		Circle* pInstance = new Circle();
		pInstance->Set_Shape(CIRCLE); //
		return pInstance;
	}

private:
	float m_PieAngle;
};