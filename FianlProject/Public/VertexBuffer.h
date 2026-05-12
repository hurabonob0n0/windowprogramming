#pragma once
#include "Utils.hpp"
#include "Component.h"
#include "Transform.h"
enum SHAPE{TRI,QUAD,CIRCLE,PENTAGON,SHAPE_END};

//모든 도형을 다 Polygon으로 그리자

class VertexBuffer : public Component{
public:
	VertexBuffer(GameObject* owner);
	virtual ~VertexBuffer() {};

public:
	VertexBuffer*			Make_Points(DirectX::FXMMATRIX worldMatrix,DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix);

public:
	VertexBuffer*			Set_Shape(SHAPE s);

public:
	const vector<POINT>&	Get_Points() const { return m_Points; }

public:
	static void				Build_Geometrys();

protected:
	static std::vector<DirectX::XMFLOAT3>	m_LocalPoints[SHAPE_END];
	vector<POINT>							m_Points;

protected:
	SHAPE									m_Shape{ SHAPE::TRI };
};