#pragma once
#include "Utilities.hpp"
#include <vector>

typedef struct tagFPoint {
	float fX{ -1 };
	float fY{ -1 };

	tagFPoint& operator += (const tagFPoint& other) { fX += other.fX; fY += other.fY; return *this; }
	void operator = (const tagFPoint& other) { fX = other.fX; fY = other.fY;}
} FPOINT;

enum SHAPE{TRI,QUAD,CIRCLE,PENTAGON,};

//모든 도형을 다 Polygon으로 그리자

class Shape {
public:
	Shape() {};
	virtual ~Shape() {};

public:
	virtual void Draw() = 0;
	Shape* Set_CenterPoint(FPOINT p);
	Shape* Set_Width_Height(float w, float h);
	Shape* Set_Shape(SHAPE s);
	Shape* Set_Rot(float R);
	virtual Shape* Make_Points() = 0;

protected:
	FPOINT m_CenterPoint{};
	float m_Width{ -1 }, m_Height{ -1 };
	vector<POINT> m_Points;
	float m_Rot{ -1 };

protected:
	HDC m_hDC = nullptr;
};

class Tri : public Shape {
public:
	Tri() {};

public:
	virtual void Draw();
	virtual Shape* Make_Points();
};