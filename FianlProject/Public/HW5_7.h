#pragma once
#include "RenderObject.h"

// 1. 패럴랙스 스크롤링을 담당하는 배경 레이어
class BackgroundLayer : public RenderObject
{
public:
	BackgroundLayer() : RenderObject() {}
	~BackgroundLayer() = default;

public:
	GameObject* Initialize(const wchar_t* imagePath, float speed);
	void Update(float dt) override;
	void Late_Update(float dt) {};
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
	class Sprite* m_Sprite = nullptr;
	class VertexBuffer* m_VB = nullptr;
	float m_Speed = 0.0f;
	float m_OffsetX = 0.0f;
};

// 2. 랜덤하게 스폰되어 우측으로 이동하는 몬스터
class Monster : public RenderObject
{
public:
	Monster() : RenderObject() {}
	~Monster() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;
	void Late_Update(float dt) {};
	void Respawn(); // 화면 끝에 도달하면 다시 왼쪽에서 스폰

private:
	class Sprite* m_SpriteType1;
	class Sprite* m_SpriteType2;
	class VertexBuffer* m_VB = nullptr;

	int m_CurrentType = 0; // 0 또는 1 (두 종류 몬스터)
	float m_Speed = 500.0f;
};

// 3. 플레이어 캐릭터
class Player : public RenderObject
{
public:
	Player() : RenderObject() {}
	~Player() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;
	void Late_Update(float dt) { if (m_SprExplosion->Get_currentFrame() >= 62) DontDraw = true; };
	void Set_Dead() { m_isDead1 = true; }
	bool Is_Dead() { return m_isDead1; }

private:
	class Sprite* m_SprIdle;
	class Sprite* m_SprExplosion; // 충돌 시 폭발 애니메이션
	class VertexBuffer* m_VB = nullptr;

	int m_State = 0; // 0: Idle, 1: Jump, 2: Duck
	bool m_isDead1 = false;

	bool DontDraw = false;

	float m_VelocityY = 0.0f;
	float m_Gravity = 980.0f;
	float m_BaseY = -200.0f; // 바닥 높이
	float m_Speed = 500.0f;
};

// 4. 메인 게임 씬 관리자
class HW5_7 : public RenderObject
{
public:
	HW5_7() : RenderObject() {}
	~HW5_7() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
	BackgroundLayer* m_SkyLayer;
	BackgroundLayer* m_GroundLayer;
	Player* m_Player;
	Monster* m_Monster;
};