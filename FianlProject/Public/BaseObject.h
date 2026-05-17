#pragma once
#include "RenderObject.h"

class BaseObject : public RenderObject
{
public:
	BaseObject() : RenderObject() {}
	~BaseObject() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
	class VertexBuffer* m_VB = nullptr;
	class Shape* m_Shape = nullptr;
};