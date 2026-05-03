#pragma once
#include "Utils.hpp"
#include "Component.h"
#include "Transform.h"
enum SHAPE{TRI,QUAD,CIRCLE,PENTAGON,SHAPE_END};

//모든 도형을 다 Polygon으로 그리자

class Shape : public Component{
public:
	Shape(GameObject* owner) : Component(owner) {};
	virtual ~Shape() {};

public:
	void			Draw(HDC hDC);
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
	virtual Shape*	Make_Points();

public:
	Shape*			Set_Transform(Transform* pTransform) { m_pTransform = pTransform; return this; }
	Shape*			Set_Shape(SHAPE s);

public:
	static void		Build_Geometrys();

protected:
	Transform*								m_pTransform{ nullptr };

protected:
	static std::vector<DirectX::XMFLOAT3>	m_LocalPoints[SHAPE_END];
	vector<POINT>							m_Points;
	COLORREF								m_LineColor{ RGB(0, 0, 0) };		// 기본 검은색
	int										m_LineWidth{ 3 };					// 기본 두께 1
	COLORREF								m_FillColor{ RGB(255, 255, 255) };	// 기본 흰색
	bool									m_UseFill{ true };					// 채우기 여부

protected:
	SHAPE									m_Shape{ SHAPE::TRI };
};

//class Tri : public Shape {
//public:
//	Tri() {};
//
//public:
//	static Tri* Create() {
//		Tri* pInstance = new Tri();
//		pInstance->Set_Shape(TRI);
//		return pInstance;
//	}
//};
//
//class Quad : public Shape {
//public:
//	Quad() {}
//	static Quad* Create() {
//		Quad* pInstance = new Quad();
//		pInstance->Set_Shape(QUAD); //
//		return pInstance;
//	}
//};
//
//class Pentagon : public Shape {
//public:
//	Pentagon() {}
//	static Pentagon* Create() {
//		Pentagon* pInstance = new Pentagon();
//		pInstance->Set_Shape(PENTAGON); //
//		return pInstance;
//	}
//};
//
//class Circle : public Shape {
//public:
//	Circle() {} // 기본값 360도 (완전한 원)
//	//virtual Shape* Make_Points() override;
//
//	// 파이 각도 설정 (메서드 체이닝)
//	//Circle* Set_PieAngle(float angle) { m_PieAngle = angle; return this; }
//
//	static Circle* Create() {
//		Circle* pInstance = new Circle();
//		pInstance->Set_Shape(CIRCLE); //
//		return pInstance;
//	}
//};