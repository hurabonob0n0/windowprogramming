#pragma once
#include "GameObject.h"

class BaseSpriteObject : public GameObject
{
public:
	BaseSpriteObject() : GameObject() {}
	~BaseSpriteObject() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
	class Sprite* m_Sprite = nullptr;
};