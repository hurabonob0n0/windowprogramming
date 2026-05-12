#include "BaseSpriteObject.h"
#include "RawInput.hpp"
#include "VertexBuffer.h"

GameObject* BaseSpriteObject::Initialize()
{
	m_Sprite = Add_Component<Sprite>();
	m_Sprite->Load_Image(L"../Sprites/miho1.png");
	

	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);


	m_Transform->Set_TransformState(0, 0, 100, 100, 0);
	//m_Transform->set
	return this;
}

void BaseSpriteObject::Update(float dt)
{
	if (g_RawInput->Key_Pressing('W'))
		m_Transform->Go_Up(100 * dt);
	if (g_RawInput->Key_Pressing('A'))
		m_Transform->Go_Left(100 * dt);
	if (g_RawInput->Key_Pressing('S'))
		m_Transform->Go_Down(100 * dt);
	if (g_RawInput->Key_Pressing('D'))
		m_Transform->Go_Right(100 * dt);
}

void BaseSpriteObject::Late_Update(float dt)
{

}

void BaseSpriteObject::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	m_VB->Make_Points(m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
	m_Sprite->Draw(hDC,m_VB->Get_Points());
}
