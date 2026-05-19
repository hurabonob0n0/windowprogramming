#pragma once
#include "RenderObject.h"

class Cat : public RenderObject
{
public:
	Cat() : RenderObject() {}
	~Cat() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

public:
	void Change_Dir();
	void Speed_Up() { m_Speed += 25; }

public:
	void Set_MouseTransform(Transform* mouseTrans) { m_MouseTransform = mouseTrans; }
	void Set_CheeseTransform(Transform* CheeseTrans) { m_CheeseTransform = CheeseTrans; }

private:
	class Sprite* m_Sprite1;
	class Sprite* m_Sprite2;
	class VertexBuffer* m_VB = nullptr;
	class Transform* m_MouseTransform;
	class Transform* m_CheeseTransform;
	class Transform* m_GameMouseTransform;
	//class Shape* m_Shape = nullptr;

private:
	float m_Time = 0;
	float m_Dir = XM_PI;
	float m_Speed = 100;
	bool  m_Mode = false;
};

class Jerry : public RenderObject
{
public:
	Jerry() : RenderObject() {}
	~Jerry() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
	class VertexBuffer* m_VB = nullptr;
	class Sprite* m_Sprite = nullptr;
	class Transform* m_GameMouseTransform;

private:
	bool m_isClicking = false;
};

class HW5_5 : public RenderObject
{
public:
	HW5_5() : RenderObject() {}
	~HW5_5() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
	Cat* m_Tom;
	Jerry* m_Jerry;
	VertexBuffer* m_VB;
	Shape* m_Shape;
	class Transform* m_GameMouseTransform;

	bool m_Toggle = false;
};