#include "BaseObject.h"
#include "RawInput.hpp"
#include "VertexBuffer.h"

GameObject* BaseObject::Initialize()
{
	__super::Initialize();

	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);

	m_Shape = Add_Component<Shape>();
	m_Shape->Set_Line(RGB(0, 0, 0), 1)->Set_Fill(RGB(255,0,0));

	m_Transform->Set_TransformState(0, 0, 22.5, 22.5, 0);
	return this;
}

void BaseObject::Update(float dt)
{
	if (g_RawInput->Key_Pressing(VK_UP))
		m_Transform->Go_Up(100 * dt);
	if (g_RawInput->Key_Pressing(VK_LEFT))
		m_Transform->Go_Left(100 * dt);
	if (g_RawInput->Key_Pressing(VK_DOWN))
		m_Transform->Go_Down(100 * dt);
	if (g_RawInput->Key_Pressing(VK_RIGHT))
		m_Transform->Go_Right(100 * dt);
}

void BaseObject::Late_Update(float dt)
{
	__super::Late_Update(dt);
}

void BaseObject::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	m_VB->Make_Points(m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
	m_Shape->Draw(hDC,m_VB->Get_Points());
}
