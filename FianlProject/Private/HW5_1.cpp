#include "HW5_1.h"
#include "RawInput.hpp"
#include "SceneManager.h"

GameObject* HW5_1::Initialize()
{
	__super::Initialize();
	mouseTransform = SceneManager::Get_Instance()->Get_ActiveScene()->Get_Object("Mouse0")->Get_Transform();

	Create_Pieces();
	return this;
}

void HW5_1::Clear_Pieces()
{
	for (auto p : m_Pieces) {
		delete p;
	}
	m_Pieces.clear();
}

void HW5_1::Create_Pieces()
{
	Clear_Pieces();

	int cols = 1, rows = 1;
	if (mode == HW5_1_mode::Div2) { cols = 2; rows = 1; }
	else if (mode == HW5_1_mode::Div4) { cols = 2; rows = 2; }
	else if (mode == HW5_1_mode::Div6) { cols = 3; rows = 2; }
	if (cols != 1) {
		for (int r = 0; r < rows; ++r) {
			for (int c = 0; c < cols; ++c) {
				ImagePiece* piece = new ImagePiece();
				piece->Initialize();
				piece->Set_MouseTransform(mouseTransform);

				float WinCX = g_WinInfo.WinCX;
				float WinCY = g_WinInfo.WinCY;
				float WidthX = WinCX / cols;
				float WidthY = WinCY / rows;
				float worldX = g_WinInfo.WinCX / cols * c - g_WinInfo.WinCX/2 + WidthX/2;
				float worldY = g_WinInfo.WinCY / rows * r - g_WinInfo.WinCY / 2 + WidthY / 2;
				piece->Get_Transform()->Set_TransformState(worldX, worldY, g_WinInfo.WinCX / cols, g_WinInfo.WinCY / rows, 0);

				m_Pieces.push_back(piece);
			}
		}
	}
	else
	{
		ImagePiece* piece = new ImagePiece();
		piece->Initialize();
		piece->Set_MouseTransform(mouseTransform);

		float worldX = 0;
		float worldY = 0;
		piece->Get_Transform()->Set_TransformState(worldX, worldY, sizeX,sizeY, 0);

		m_Pieces.push_back(piece);
	}
}

void HW5_1::Update(float dt)
{
	bool modeChanged = false;

	// 1, 2, 4, 6 등분 처리
	if (g_RawInput->Key_Down('1')) { mode = HW5_1_mode::Div1; modeChanged = true; }
	if (g_RawInput->Key_Down('2')) { mode = HW5_1_mode::Div2; modeChanged = true; }
	if (g_RawInput->Key_Down('4')) { mode = HW5_1_mode::Div4; modeChanged = true; }
	if (g_RawInput->Key_Down('6')) { mode = HW5_1_mode::Div6; modeChanged = true; }

	// 크기 조절 (+ / -)
	if (g_RawInput->Key_Down(VK_OEM_PLUS)) {
		sizeX += 100; sizeY += 100;
		modeChanged = true;
	}
	if (g_RawInput->Key_Down(VK_OEM_MINUS)) {
		sizeX = max(100, sizeX - 100); sizeY = max(100, sizeY - 100);
		modeChanged = true;
	}

	// 'a' 키: 배경 모드 전환 (화면 크기 800x600 가정, 실제 엔진 창 크기로 수정 요망)
	if (g_RawInput->Key_Down('A')) {
		isBackgroundMode = !isBackgroundMode;
		if (isBackgroundMode) {
			sizeX = g_WinInfo.WinCX; sizeY = g_WinInfo.WinCY; // 창의 전체 해상도 입력
		}
		else {
			sizeX = 600; sizeY = 600; // 원래 크기
		}
		modeChanged = true;
	}

	// 상태가 변했다면 조각들 재배치
	if (modeChanged) {
		Create_Pieces();
	}

	// 마우스 클릭 시 충돌한 조각만 선택 처리
	if (g_RawInput->Mouse_Down(0)) {
		for (auto p : m_Pieces) {
			p->Set_Selected(p->Check_Collision());
		}
	}

	// 'r' 키: 선택된 조각 반전
	if (g_RawInput->Key_Down('R')) {
		for (auto p : m_Pieces) {
			if (p->Get_Selected()) {
				p->Toggle_Reverse();
			}
		}
	}

	for (auto p : m_Pieces) p->Update(dt);
}

void HW5_1::Late_Update(float dt)
{
	for (auto p : m_Pieces) p->Late_Update(dt);
	__super::Late_Update(dt);
}

void HW5_1::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	for (auto p : m_Pieces) p->Draw(hDC, viewMatrix, projMatrix);
}

GameObject* ImagePiece::Initialize()
{
	__super::Initialize();
	m_Sprite = Add_Component<Sprite>();
	m_Sprite->Load_Image(L"../Sprites/miho2.png");

	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);

	m_Shape = Add_Component<Shape>();
	// 투명 브러시와 빨간색 테두리 세팅
	m_Shape->Set_Line(RGB(255, 0, 0), 5)->Set_Fill(RGB(0, 0, 0), false);

	return this;
}


void ImagePiece::Update(float dt) {}
void ImagePiece::Late_Update(float dt) { __super::Late_Update(dt); }

void ImagePiece::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	m_VB->Make_Points(m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);

	// 반전 상태면 NOTSRCCOPY, 아니면 SRCCOPY를 사용해 그리기
	DWORD dop = m_bIsReversed ? NOTSRCCOPY : SRCCOPY;

	// 투명화(isAlpha)는 false로 꺼두고 Raster Operation을 적용함
	m_Sprite->Draw(hDC, m_VB->Get_Points(), false, dop);

	// 선택되었다면 빨간 테두리 출력
	if (m_bIsSelected && m_Shape)
	{
		m_Shape->Draw(hDC, m_VB->Get_Points());
	}
}

bool ImagePiece::Check_Collision()
{
	if (m_MouseTransform == nullptr) return false;

	float myX = m_Transform->Get_State(Transform::STATE_POSITION).m128_f32[0];
	float myY = m_Transform->Get_State(Transform::STATE_POSITION).m128_f32[1];
	float myScaleX = m_Transform->Get_Scaled().x;
	float myScaleY = m_Transform->Get_Scaled().y;

	float mouseX = m_MouseTransform->Get_State(Transform::STATE_POSITION).m128_f32[0];
	float mouseY = m_MouseTransform->Get_State(Transform::STATE_POSITION).m128_f32[1];

	float halfW = myScaleX * 0.5f;
	float halfH = myScaleY * 0.5f;

	if (mouseX >= myX - halfW && mouseX <= myX + halfW &&
		mouseY >= myY - halfH && mouseY <= myY + halfH)
	{
		return true;
	}

	return false;
}