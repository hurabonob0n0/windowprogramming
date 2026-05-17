#pragma once
#include "RenderObject.h"

class BaseSpriteObject : public RenderObject
{
public:
	BaseSpriteObject() : RenderObject() {}
	~BaseSpriteObject() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
	class VertexBuffer* m_VB = nullptr;
	class Sprite* m_Sprite = nullptr;
};