#include "BaseObject.h"

GameObject* BaseObject::Initialize()
{
	m_Shape = Add_Component<Shape>();
	m_Shape->Set_Shape(QUAD)->Set_Line(RGB(0, 0, 0), 1)->Set_Fill(RGB(255,0,0));
	

	m_Transform->Set_TransformState(0, 0, 22.5, 22.5, 0);
	//m_Transform->set
	return this;
}

void BaseObject::Update(float dt)
{
	if (GetAsyncKeyState('D') & 0x8000)
		m_Transform->Go_Right(100*dt);
	if (GetAsyncKeyState('W') & 0x8000)
		m_Transform->Go_Up(100 * dt);
	if (GetAsyncKeyState('A') & 0x8000)
		m_Transform->Go_Left(100 * dt);
	if (GetAsyncKeyState('S') & 0x8000)
		m_Transform->Go_Down(100 * dt);
}

void BaseObject::Late_Update(float dt)
{
}

void BaseObject::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	m_Shape->Draw(hDC, viewMatrix, projMatrix);
}
