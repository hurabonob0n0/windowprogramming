#include "RenderObject.h"
#include "RenderManager.h"

GameObject* RenderObject::Initialize()
{
	m_LayerType = eLayerType::Default;
	m_RenderMgr = RenderManager::Get_Instance();
	return this;
}

void RenderObject::Update(float dt)
{
}

void RenderObject::Late_Update(float dt)
{
	m_RenderMgr->Add_RenderObject(this, m_LayerType);
}

void RenderObject::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
}
