#include "Camera.h"
#include "RenderManager.h"

GameObject* Camera::Initialize()
{
	m_RM = RenderManager::Get_Instance();
	return this;
}

void Camera::Update(float dt)
{
	
}

void Camera::Late_Update(float dt)
{
	if (m_Owner) {
		XMVECTOR pos = m_Owner->Get_State(Transform::STATE_POSITION);
		m_Transform->Set_Position(pos.m128_f32[0], pos.m128_f32[1]);
	}
	//else {
	//	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//		m_Transform->Go_Right(dt*1000);
	//	if (GetAsyncKeyState(VK_UP) & 0x8000)
	//		m_Transform->Go_Up(dt * 1000);
	//	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//		m_Transform->Go_Left(dt * 1000);
	//	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//		m_Transform->Go_Down(dt * 1000);
	//}

	m_RM->Set_ViewMatrix(m_Transform->Get_WorldMatrixInv());
	

	//===============이 부분은 종횡비로 계산====================
	//// 1. 화면의 종횡비 (세로 / 가로) 계산
	//float aspectRatio = (float)g_WinInfo.WinCX / (float)g_WinInfo.WinCY;

	//// 2. 가로 크기는 1.0으로 고정, 세로는 비율만큼 보정
	//float cameraWidth = aspectRatio;
	//float cameraHeight = 1.f;
	//
	//m_RM->Set_ProjMatrix(XMMatrixOrthographicLH(cameraWidth, cameraHeight, 0.f, 1.f));


	//=========이부분은 픽셀 퍼펙트======================
	m_RM->Set_ProjMatrix(XMMatrixOrthographicLH(g_WinInfo.WinCX, g_WinInfo.WinCY, 0.f, 1.f));
}

void Camera::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
}