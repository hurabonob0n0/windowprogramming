#pragma once
#include "GameObject.h"
#include "Random_Engine.hpp"

// 화면 1600 x 900
// 블록 하나당 100,25 픽셀 사이즈로 만들자.
// 공 크기 20 x 20

class Block : public GameObject
{
public:
	Block() : GameObject() {}
	~Block() {};

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

public:
	void Set_TransformState(float x, float y, float width, float height, float rotDegree = 0) {
		m_Transform->Set_TransformState(x, y, width, height, rotDegree);
	}
	void Set_Hit() {
		if (!m_isHit) {
			m_isHit = true;
		}
		m_isFalling = true; // 떨어지기 시작
	}
	void Change_Color(int r, int g, int b) { m_Shape->Set_Fill(RGB(r, g, b)); }
	void After_Hit_Color_Change();
	bool Is_Hit() const { return m_isHit; }

private:
	bool		m_isHit = false;
	int			RGB[3];
	Shape*		m_Shape = nullptr;
	bool m_isFalling = false;
	float m_FallSpeed = 200.0f;
};








class Bar : public GameObject {
public:
	Bar() : GameObject() {}
	~Bar() {};

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

	float Get_VelocityX() const { return m_VelocityX; }

private:
	Shape* m_Shape = nullptr;

	// 속도 계산을 위한 변수 추가
	float m_PrevX = 800.0f;
	float m_VelocityX = 0.0f;
};







class Ball : public GameObject
{
public:
	Ball() : GameObject() {}
	~Ball() {}

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

	// 공 이동 관련 함수
	void Fire();
	void Reset_Position(float barX, float barY);
	void Set_SpeedMultiplier(float amount) { 
		m_Speed += amount; 
		if (m_Speed < 200.0f) m_Speed = 200.0f; // 최소 속도 제한
		if (m_Speed > 2000.0f) m_Speed = 2000.0f; // 최대 속도 제한
	 }

	// 바와 충돌했을 때 스핀을 먹이는 특수 반사 함수
	void Bounce_Off_Bar(float barVelocityX);

	// 충돌 처리를 위한 접근자 (필요 시 Transform의 Get_Position 등으로 대체 가능)
	float Get_X() { return m_Transform->Get_State(Transform::STATE_POSITION).m128_f32[0]; }
	float Get_Y() { return m_Transform->Get_State(Transform::STATE_POSITION).m128_f32[1]; }
	void Reverse_X() { m_DirX *= -1.f; }
	void Reverse_Y() { m_DirY *= -1.f; }

public:
	bool   m_isFired = false; // 시작 명령어(s)를 받았는지 여부

private:
	Shape* m_Shape = nullptr;
	float  m_DirX = 1.0f;     // 이동 방향 X
	float  m_DirY = 1.0f;    // 이동 방향 Y (초기엔 위로 튕기도록)
	float  m_Speed = 1000.0f;
	
	// 공의 이동 속도
};







class HW4_3 : public GameObject
{
public:
	HW4_3() : GameObject() {}
	~HW4_3() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
	vector<vector<Block*>> m_Blocks;
	Bar* m_Bar;
	Ball* m_Ball;

	bool  m_isPaused = false;
	float m_GridOffsetX = 0.0f; // 벽돌 전체 좌우 이동 오프셋
	float m_GridMoveDir = 1.0f;

	// 통계용 변수
	int m_ChangedColorCount = 0;
	int m_DisappearedCount = 0;
	float m_Time = 0.0f; // T 명령어용
};