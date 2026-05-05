#include "Board.h"

GameObject* Board::Initialize()
{
	m_Transform->Set_TransformState(0, 0, 22.5 * 1.414, 22.5 * 1.414, 45);
	m_Shape = Add_Component<Shape>();
	m_Shape->Set_Shape(QUAD)->Set_Line(RGB(0,0,0),1);
	return this;
}

void Board::Update(float dt)
{
}

void Board::Late_Update(float dt)
{
}

void Board::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	for(int i = 0; i<40; ++i){
		for (int j = 0; j < 40; ++j) {
			m_Transform->Set_Position((float)i * 22.5, (float)j * 22.5);
			m_Shape->Draw(hDC, viewMatrix, projMatrix);
		}
	}
}
