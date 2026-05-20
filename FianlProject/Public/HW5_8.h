#pragma once
#include "RenderObject.h"

class PICHA : public RenderObject {
public:
	PICHA() : RenderObject() {}
	~PICHA() {}

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

public:
	void set_Obstacle(int i ,Transform* t) {if(i >= 0) m_Obstacle[i] = t; }
	

private:
	bool m_isFalling = false;
	Transform* m_Obstacle[3];
	
	Transform* m_MouseTransform;
	bool m_isCollision = false;

private:
	Sprite* m_Sprite0;
	Sprite* m_Sprite1;
	class VertexBuffer* m_VB;
	float m_Speed;
};

class Tile : public RenderObject {
public:
	Tile() : RenderObject() {}
	~Tile() {}

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
	Transform* m_MouseTransform;
	Sprite* m_Sprite0;
	class VertexBuffer* m_VB;
};

class HW5_8 : public RenderObject {
public:
	HW5_8() : RenderObject() {}
	~HW5_8() {}

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

public:
	list<Tile*> m_Tiles;
	list<PICHA*> m_Pichas;
};