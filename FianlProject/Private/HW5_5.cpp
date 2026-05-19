#include "HW5_5.h"
#include "RawInput.hpp"
#include "VertexBuffer.h"
#include "Random_Engine.hpp"
#include "SceneManager.h"

GameObject* HW5_5::Initialize()
{
	__super::Initialize();

	m_Tom = new Cat();
	m_Tom->Initialize();

	m_Jerry = new Jerry();
	m_Jerry->Initialize();

	m_Tom->Set_MouseTransform(nullptr);
	m_Tom->Set_CheeseTransform(nullptr);

	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);

	m_Shape = Add_Component<Shape>();
	m_Shape->Set_Line(RGB(255, 255, 255), 10)->Set_Fill(RGB(0, 100, 255));

	m_Transform->Set_Scale(50, 50);

	m_GameMouseTransform = SceneManager::Get_Instance()->Get_ActiveScene()->Get_Object("Mouse0")->Get_Transform();

	return this;
}

void HW5_5::Update(float dt)
{
	if (g_RawInput->Key_Down('R'))
		Initialize();

	if (g_RawInput->Mouse_Down(1)) {
		m_Transform->Set_Position(m_GameMouseTransform->Get_State(Transform::STATE_POSITION).m128_f32[0], 
			m_GameMouseTransform->Get_State(Transform::STATE_POSITION).m128_f32[1]);
		m_Tom->Set_CheeseTransform(m_Transform); 	m_Toggle = true;
	}

	if (g_RawInput->Mouse_Down(0))
		m_Tom->Set_MouseTransform(m_Jerry->Get_Transform());

	if(g_RawInput->Mouse_Up(0))
		m_Tom->Set_MouseTransform(nullptr);

	m_Tom->Update(dt);
	m_Jerry->Update(dt);
}

void HW5_5::Late_Update(float dt)
{
	if (m_Toggle) {
		XMVECTOR vPos = m_Transform->Get_State(Transform::STATE_POSITION);
		float XPos, YPos;
		XPos = vPos.m128_f32[0]; YPos = vPos.m128_f32[1];
		XMFLOAT3 vScale = m_Transform->Get_Scaled();

		Transform* TomTransform = m_Tom->Get_Transform();
		XMVECTOR vTomPos = TomTransform->Get_State(Transform::STATE_POSITION);
		float TomPosX, TomPosY;
		TomPosX = vTomPos.m128_f32[0]; TomPosY = vTomPos.m128_f32[1];
		XMFLOAT3 vTomScale = TomTransform->Get_Scaled();

		if (XPos + vScale.x / 2 > TomPosX - vTomScale.x / 2 &&
			XPos - vScale.x / 2 < TomPosX + vTomScale.x / 2 &&
			YPos + vScale.y / 2 > TomPosY - vTomScale.y / 2 &&
			YPos - vScale.y / 2 < TomPosY + vTomScale.y / 2) {
			m_Toggle = false; m_Tom->Set_CheeseTransform(nullptr); m_Tom->Speed_Up();
		}
	}
	
	m_Tom->Late_Update(dt);
	m_Jerry->Late_Update(dt);

	__super::Late_Update(dt);
}

void HW5_5::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	m_Tom->Draw(hDC, viewMatrix, projMatrix);
	m_Jerry->Draw(hDC, viewMatrix, projMatrix);

	if (m_Toggle) {
		m_VB->Make_Points(m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
		m_Shape->Draw(hDC, m_VB->Get_Points());
	}
}

GameObject* Cat::Initialize()
{
	__super::Initialize();
	m_Sprite1 = Add_Component<Sprite>();
	m_Sprite1->Load_Image(L"../Sprites/Idle-Anim_32_4_4.png");

	m_Sprite2 = Add_Component<Sprite>();
	m_Sprite2->Load_Image(L"../Sprites/Attack-Anim_112_14_14.png");

	m_Transform->Set_Position(0, 0);
	m_Transform->Set_Scale(2, 2);

	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);

	m_GameMouseTransform = SceneManager::Get_Instance()->Get_ActiveScene()->Get_Object("Mouse0")->Get_Transform();

	//m_Shape = Add_Component<Shape>();
	//m_Shape->Set_Line(RGB(0, 0, 0), 5)->Set_Fill(RGB(255, 255, 255), false);
	return this;
}

void Cat::Update(float dt)
{
	if (m_CheeseTransform) {
		XMVECTOR v = m_CheeseTransform->Get_State(Transform::STATE_POSITION);
		m_Transform->Go_to_Pos(v.m128_f32[0], v.m128_f32[1], m_Speed*dt);
	}
	else if (m_MouseTransform) {
		XMVECTOR v = m_MouseTransform->Get_State(Transform::STATE_POSITION);
		m_Transform->Go_to_Pos(v.m128_f32[0], v.m128_f32[1], m_Speed * dt);
	}
	else {
		if (m_Time > 3) { m_Time = 0; Change_Dir(); }

		XMVECTOR vPos = m_Transform->Get_State(Transform::STATE_POSITION);
		if (vPos.m128_f32[0] >= 800) { m_Transform->Set_Position(795, vPos.m128_f32[1]); Change_Dir(); m_Time = 0; }
		if (vPos.m128_f32[0] <= -800) { m_Transform->Set_Position(-795, vPos.m128_f32[1]); Change_Dir(); m_Time = 0; }
		if (vPos.m128_f32[1] >= 450) { m_Transform->Set_Position(vPos.m128_f32[0], 445); Change_Dir(); m_Time = 0; }
		if (vPos.m128_f32[1] <= -450) { m_Transform->Set_Position(vPos.m128_f32[0], -445); Change_Dir(); m_Time = 0; }

		m_Transform->Go_Dir(m_Dir, m_Speed * dt);
	}

	float XPos, YPos;
	XPos = m_Transform->Get_State(Transform::STATE_POSITION).m128_f32[0];
	YPos = m_Transform->Get_State(Transform::STATE_POSITION).m128_f32[1];
	float MouseX = m_GameMouseTransform->Get_State(Transform::STATE_POSITION).m128_f32[0];
	float MouseY = m_GameMouseTransform->Get_State(Transform::STATE_POSITION).m128_f32[1];
	//cout << "P x : " << XPos << ", P y : " << YPos << "    M x : " << MouseX << ", M y : " << MouseY << endl;
	if (MouseX< XPos + 50 && MouseX > XPos - 50 && MouseY < YPos + 50 && MouseY > YPos - 50) {
		if (g_RawInput->Mouse_Down(0))
			m_Mode = !m_Mode;
	}
}

void Cat::Late_Update(float dt)
{
	if (m_Mode == false)
		m_Sprite1->Update(dt);
	else
		m_Sprite2->Update(dt);

	m_Time += dt;

	if (g_RawInput->Key_Down(VK_OEM_PLUS))
		m_Speed += 25;

	if (g_RawInput->Key_Down(VK_OEM_MINUS))
		m_Speed -= 25;

	//__super::Late_Update(dt);
}

void Cat::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	//m_Transform->Set_Scale(2, 2);
	if (m_Mode == false) {
		float frameWidth = static_cast<float>(m_Sprite1->Get_FrameWidth());
		float frameHeight = static_cast<float>(m_Sprite1->Get_FrameHeight());

		DirectX::XMMATRIX spriteScaleMat = DirectX::XMMatrixScaling(frameWidth, frameHeight, 1.0f);

		DirectX::XMMATRIX renderWorldMatrix = spriteScaleMat * m_Transform->Get_WorldMatrix();
		m_VB->Make_Points(renderWorldMatrix, viewMatrix, projMatrix);
		m_Sprite1->Draw(hDC, m_VB->Get_Points());

	}

	else {
		float frameWidth = static_cast<float>(m_Sprite2->Get_FrameWidth());
		float frameHeight = static_cast<float>(m_Sprite2->Get_FrameHeight());

		DirectX::XMMATRIX spriteScaleMat = DirectX::XMMatrixScaling(frameWidth, frameHeight, 1.0f);

		DirectX::XMMATRIX renderWorldMatrix = spriteScaleMat * m_Transform->Get_WorldMatrix();
		m_VB->Make_Points(renderWorldMatrix, viewMatrix, projMatrix);
		m_Sprite2->Draw(hDC, m_VB->Get_Points());
	}

	/*m_Transform->Set_Scale(50, 50);
	m_VB->Make_Points(m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
	m_Shape->Draw(hDC,m_VB->Get_Points());*/
}

void Cat::Change_Dir()
{
	m_Dir = g_Random->GetFloatRange(0, XM_2PI);
}

GameObject* Jerry::Initialize()
{
	__super::Initialize();
	m_Sprite = Add_Component<Sprite>();
	m_Sprite->Load_Image(L"../Sprites/Walk-Anim_32_4_4.png");

	m_Transform->Set_Position(0, 0);
	m_Transform->Set_Scale(2, 2);

	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);

	m_GameMouseTransform = SceneManager::Get_Instance()->Get_ActiveScene()->Get_Object("Mouse0")->Get_Transform();

	return this;
}

void Jerry::Update(float dt)
{
	if (g_RawInput->Mouse_Pressing(0)) {
		XMVECTOR vPos = m_GameMouseTransform->Get_State(Transform::STATE_POSITION);
		m_Transform->Set_Position(vPos.m128_f32[0], vPos.m128_f32[1]);
	}
	m_Sprite->Update(dt);
}

void Jerry::Late_Update(float dt)
{
	//__super::Late_Update(dt);
}

void Jerry::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	if (g_RawInput->Mouse_Pressing(0)) {
		float frameWidth = static_cast<float>(m_Sprite->Get_FrameWidth());
		float frameHeight = static_cast<float>(m_Sprite->Get_FrameHeight());

		DirectX::XMMATRIX spriteScaleMat = DirectX::XMMatrixScaling(frameWidth, frameHeight, 1.0f);

		DirectX::XMMATRIX renderWorldMatrix = spriteScaleMat * m_Transform->Get_WorldMatrix();
		m_VB->Make_Points(renderWorldMatrix, viewMatrix, projMatrix);
		m_Sprite->Draw(hDC, m_VB->Get_Points());
	}
}
