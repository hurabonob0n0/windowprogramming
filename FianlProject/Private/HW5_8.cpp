#include "HW5_8.h"
#include "Sprite.h"
#include "Random_Engine.hpp"
#include "RawInput.hpp"
#include "VertexBuffer.h"
#include "SceneManager.h"

GameObject* PICHA::Initialize()
{
	m_Sprite0 = Add_Component<Sprite>();
	m_Sprite0->Load_Image(L"../Sprites/Walk-Anim_32_4_4.png");

	m_Sprite1 = Add_Component<Sprite>();
	m_Sprite1->Load_Image(L"../Sprites/Faint-Anim_32_4_4.png");

	m_Transform->Set_Scale(2, 2);
	m_Transform->Set_Position(g_Random->GetFloatRange(-800, 800), 420);

	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);

	m_Speed = g_Random->GetFloatRange(100, 300);

	return this;
}

void PICHA::Update(float dt)
{
	if (g_RawInput->Key_Down('P'))
		m_isFalling = true;

	if (m_isCollision == false)
	{
		if (m_isFalling) {
			float PosX = m_Transform->Get_PosX();
			float PosY = m_Transform->Get_PosY();

			PosY -= m_Speed * dt;
			m_Transform->Set_Position(PosX, PosY);

		}
	}


}

void PICHA::Late_Update(float dt)
{
	if (m_Transform->Get_PosY() < -480)
		m_Transform->Set_PosY(470.f);

	float mPosX = m_Transform->Get_PosX();
	float mPosY = m_Transform->Get_PosY();
	float mSizX = 25;
	float mSizY = 25;

	bool col = false;

	if (m_Obstacle[0]) {
		float OPosX = m_Obstacle[0]->Get_PosX();
		float OPosY = m_Obstacle[0]->Get_PosY();
		float OSizX = m_Obstacle[0]->Get_Scaled().x * 0.5;
		float OSizY = m_Obstacle[0]->Get_Scaled().y * 0.5;
		if (abs(OPosX - mPosX) < mSizX + OSizX && abs(OPosY - mPosY) < mSizY + OSizY) {
			m_Transform->Set_PosY(OPosY + OSizY);
			col = true;
			m_Sprite1->Update(dt);
		}
	}
	if (m_Obstacle[1]) {
		float OPosX = m_Obstacle[1]->Get_PosX();
		float OPosY = m_Obstacle[1]->Get_PosY();
		float OSizX = m_Obstacle[1]->Get_Scaled().x * 0.5;
		float OSizY = m_Obstacle[1]->Get_Scaled().y * 0.5;
		if (abs(OPosX - mPosX) < mSizX + OSizX && abs(OPosY - mPosY) < mSizY + OSizY) {
			m_Transform->Set_PosY(OPosY + OSizY);
			col = true;
			m_Sprite1->Update(dt);
		}
	}
	if (m_Obstacle[2]) {
		float OPosX = m_Obstacle[2]->Get_PosX();
		float OPosY = m_Obstacle[2]->Get_PosY();
		float OSizX = m_Obstacle[2]->Get_Scaled().x * 0.5;
		float OSizY = m_Obstacle[2]->Get_Scaled().y * 0.5;
		if (abs(OPosX - mPosX) < mSizX + OSizX && abs(OPosY - mPosY) < mSizY + OSizY) {
			m_Transform->Set_PosY(OPosY+OSizY);
			col = true;
			m_Sprite1->Update(dt);
		}
	}

	m_isCollision = col;

	m_Sprite0->Update(dt);
}

void PICHA::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	if (m_isCollision) {
		XMMATRIX scaleMat = XMMatrixScaling(m_Sprite1->Get_FrameWidth(), m_Sprite1->Get_FrameHeight(), 1);
		m_VB->Make_Points(scaleMat * m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
		m_Sprite1->Draw(hDC, m_VB->Get_Points());
	}
	else {
		XMMATRIX scaleMat = XMMatrixScaling(m_Sprite0->Get_FrameWidth(), m_Sprite0->Get_FrameHeight(), 1);
		m_VB->Make_Points(scaleMat * m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
		m_Sprite0->Draw(hDC, m_VB->Get_Points());
	}
	
}


GameObject* Tile::Initialize()
{
	m_Sprite0 = Add_Component<Sprite>();
	m_Sprite0->Load_Image(L"../Sprites/Cloud.png");

	m_Transform->Set_Scale(g_Random->GetFloatRange(100, 200), g_Random->GetFloatRange(100, 150));
	m_Transform->Set_Position(g_Random->GetFloatRange(-750, 750), g_Random->GetFloatRange(-400, 400));

	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);

	m_MouseTransform = SceneManager::Get_Instance()->Get_ActiveScene()->Get_Object("Mouse0")->Get_Transform();

	return this;
}

void Tile::Update(float dt)
{
	if (g_RawInput->Mouse_Pressing(0)) {
		float MPosX = m_MouseTransform->Get_PosX();
		float MPosY = m_MouseTransform->Get_PosY();

		float CPosX = m_Transform->Get_PosX();
		float CPosY = m_Transform->Get_PosY();
		float CSizeX = m_Transform->Get_Scaled().x * 0.5;
		float CSizeY = m_Transform->Get_Scaled().y * 0.5;
		if (abs(CPosX - MPosX) < CSizeX && abs(CPosY - MPosY) < CSizeY) {
			float dX = g_RawInput->Get_MouseDeltaX();
			float dY = -g_RawInput->Get_MouseDeltaY();
			CPosX += dX;
			CPosY += dY;
			m_Transform->Set_Position(CPosX, CPosY);
		}
	}

}

void Tile::Late_Update(float dt)
{
	if (g_RawInput->Mouse_Down(1)) {
		float MPosX = m_MouseTransform->Get_PosX();
		float MPosY = m_MouseTransform->Get_PosY();

		float CPosX = m_Transform->Get_PosX();
		float CPosY = m_Transform->Get_PosY();
		float CSizeX = m_Transform->Get_Scaled().x * 0.5;
		float CSizeY = m_Transform->Get_Scaled().y * 0.5;
		if (abs(CPosX - MPosX) < CSizeX && abs(CPosY - MPosY) < CSizeY) {
			m_IsDead = true;
		}
	}
}

void Tile::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	m_VB->Make_Points(m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
	m_Sprite0->Draw(hDC, m_VB->Get_Points());
}




GameObject* HW5_8::Initialize()
{
	__super::Initialize();

	Transform* t1[3]{ nullptr };
	

	for (int i = 0; i < 3; ++i) {
		Tile* t = new Tile();
		t->Initialize();
		if (i == 0)
			t1[i] = t->Get_Transform();
		if (i == 1)
			t1[i] = t->Get_Transform();
		if (i == 2)
			t1[i] = t->Get_Transform();
		m_Tiles.push_back(t);
	}
	for (int i = 0; i < 40; ++i) {
		PICHA* p = new PICHA();
		p->Initialize();
		for (int j = 0; j < 3; ++j) {
			p->set_Obstacle(j,t1[j]);
		}
		m_Pichas.push_back(p);
	}

	return this;
}

void HW5_8::Update(float dt)
{
	for (auto& p : m_Pichas)
		p->Update(dt);

	for (auto& p : m_Tiles)
		p->Update(dt);
}

void HW5_8::Late_Update(float dt)
{
	int i = 0;
	int j = -1;
	for (auto it = m_Tiles.begin(); it != m_Tiles.end();) {
		(*it)->Late_Update(dt);
		if ((*it)->Is_Dead()) {
			it = m_Tiles.erase(it);
			j = i;
		}
		else
			++it;
		++i;
	}

	for (auto& p : m_Pichas) {
		p->Late_Update(dt);
		p->set_Obstacle(j, nullptr);
	}



	__super::Late_Update(dt);
}

void HW5_8::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	for (auto& p : m_Tiles)
		p->Draw(hDC, viewMatrix, projMatrix);

	for (auto& p : m_Pichas)
		p->Draw(hDC, viewMatrix, projMatrix);
}
