#include "BaseSpriteObject.h"
#include "RawInput.hpp"
#include "VertexBuffer.h"

GameObject* BaseSpriteObject::Initialize()
{
	__super::Initialize();

	m_Sprite = Add_Component<Sprite>();
	//m_Sprite->Load_Image(L"../Sprites/miho2.png");
	m_Sprite->Load_Image(L"../Sprites/jammanbo_12_4_3.png");

	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);


	m_Transform->Set_TransformState(0, 0, 200, 200, 0);
	//m_Transform->set
	return this;
}

void BaseSpriteObject::Update(float dt)
{
	if (g_RawInput->Key_Pressing('W'))
		m_Transform->Go_Up(1000 * dt);
	if (g_RawInput->Key_Pressing('A'))
		m_Transform->Go_Left(1000 * dt);
	if (g_RawInput->Key_Pressing('S'))
		m_Transform->Go_Down(1000 * dt);
	if (g_RawInput->Key_Pressing('D'))
		m_Transform->Go_Right(1000 * dt);

	m_Sprite->Update(dt);
}

void BaseSpriteObject::Late_Update(float dt)
{
	__super::Late_Update(dt);
}

void BaseSpriteObject::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	m_VB->Make_Points(m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
	m_Sprite->Draw(hDC,m_VB->Get_Points());
}
