#pragma once
#include "GameObject.h"
#include "RenderManager.h"

class Camera : public GameObject 
{
public:
	Camera() : GameObject() {}
	~Camera() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

public:
	void Go_Right(float dX) { m_Transform->Go_Right(dX); }

public:
	Camera* Set_Owner(Transform* owner) { m_Owner = owner; return this; }
	Camera* Set_RenderManager(RenderManager* RM) { m_RM = RM; }

private:
	Transform* m_Owner = nullptr;
	RenderManager* m_RM = nullptr;
};