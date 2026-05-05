#pragma once
#include "GameObject.h"

class BaseObject : public GameObject
{
public:
	BaseObject() : GameObject() {}
	~BaseObject() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
	Shape* m_Shape = nullptr;
};