#include "BaseSpriteObject.h"
#include "RawInput.hpp"
#include "VertexBuffer.h"

GameObject* BaseSpriteObject::Initialize()
{
	__super::Initialize();

	m_Sprite = Add_Component<Sprite>();
	//m_Sprite->Load_Image(L"../Sprites/miho2.png");
	m_Sprite->Load_Image(L"../Sprites/Idle-Anim_32_4_4.png");

	m_Sprite2 = Add_Component<Sprite>();
	//m_Sprite->Load_Image(L"../Sprites/miho2.png");
	m_Sprite2->Load_Image(L"../Sprites/Attack-Anim_112_14_14.png");

	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);

	m_Shape = Add_Component<Shape>();
	m_Shape->Set_Line(RGB(0, 0, 0), 5)->Set_Fill(RGB(255, 255, 255), false);


	m_Transform->Set_TransformState(0, 0, 2, 2, 0);

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
	m_Sprite2->Update(dt);
	
}

void BaseSpriteObject::Late_Update(float dt)
{
	__super::Late_Update(dt);
}

void BaseSpriteObject::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	m_Transform->Set_Scale(2, 2);

	float frameWidth = static_cast<float>(m_Sprite->Get_FrameWidth());
	float frameHeight = static_cast<float>(m_Sprite->Get_FrameHeight());

	DirectX::XMMATRIX spriteScaleMat = DirectX::XMMatrixScaling(frameWidth, frameHeight, 1.0f);

	DirectX::XMMATRIX renderWorldMatrix = spriteScaleMat * m_Transform->Get_WorldMatrix();
	m_VB->Make_Points(renderWorldMatrix, viewMatrix, projMatrix);
	m_Sprite->Draw(hDC,m_VB->Get_Points());


	frameWidth = static_cast<float>(m_Sprite2->Get_FrameWidth());
	frameHeight = static_cast<float>(m_Sprite2->Get_FrameHeight());

	spriteScaleMat = DirectX::XMMatrixScaling(frameWidth, frameHeight, 1.0f);

	renderWorldMatrix = spriteScaleMat * m_Transform->Get_WorldMatrix();
	m_VB->Make_Points(renderWorldMatrix, viewMatrix, projMatrix);
	m_Sprite2->Draw(hDC, m_VB->Get_Points());

	m_Transform->Set_Scale(50, 50);
	m_VB->Make_Points(m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
	m_Shape->Draw(hDC, m_VB->Get_Points());
}
