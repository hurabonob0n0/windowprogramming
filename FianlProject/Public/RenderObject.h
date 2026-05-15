#pragma once
#include "GameObject.h"

class RenderObject : public GameObject {
public:
	RenderObject() : GameObject() {}
	~RenderObject() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

public:
	void Set_LayerType(eLayerType type) { m_LayerType = type; }

private:
	eLayerType m_LayerType{ eLayerType::End };
	class RenderManager* m_RenderMgr;
}