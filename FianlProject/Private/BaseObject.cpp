#include "BaseObject.h"

GameObject* BaseObject::Initialize()
{
	m_Shape = Add_Component<Shape>();
	m_Shape->Set_Transform()->Set_Shape(QUAD)->Set_Fill(RGB(255,0,0));
	

	m_Transform->Set_TransformState(0, 0, 100, 100, 0);
	//m_Transform->set
	return this;
}

void BaseObject::Update(float dt)
{
	m_Transform->Turn(XMConvertToRadians(90) * dt);
}

void BaseObject::Late_Update(float dt)
{
}

void BaseObject::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	m_Shape->Draw(hDC, viewMatrix, projMatrix);
}
