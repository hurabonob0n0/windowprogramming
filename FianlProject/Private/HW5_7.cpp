#include "HW5_7.h"
#include "RawInput.hpp"
#include "VertexBuffer.h"
#include "Random_Engine.hpp"
#include "Sprite.h"

// ==========================================
// 1. BackgroundLayer 구현
// ==========================================
GameObject* BackgroundLayer::Initialize(const wchar_t* imagePath, float speed)
{
	__super::Initialize();
	m_Sprite = Add_Component<Sprite>();
	m_Sprite->Load_Image(imagePath); // 단일 이미지 로드
	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);

	m_Speed = speed;
	return this;
}

void BackgroundLayer::Update(float dt)
{
	m_OffsetX -= m_Speed * dt;

	// 스프라이트가 화면 밖으로 나가면 다시 이어 붙이도록 루프 처리
	if (m_OffsetX <= -m_Sprite->Get_FrameWidth())
		m_OffsetX += m_Sprite->Get_FrameWidth();
}

void BackgroundLayer::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	float w = static_cast<float>(m_Sprite->Get_FrameWidth());
	float h = static_cast<float>(m_Sprite->Get_FrameHeight());
	DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(w, h, 1.0f);

	// 첫 번째 이미지 그리기
	DirectX::XMMATRIX trans1 = DirectX::XMMatrixTranslation(m_OffsetX, 0.0f, 0.0f);
	m_VB->Make_Points(scaleMat * trans1 * m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
	m_Sprite->Draw(hDC, m_VB->Get_Points());

	// 이어지는 두 번째 이미지 그리기 (루프용)
	DirectX::XMMATRIX trans2 = DirectX::XMMatrixTranslation(m_OffsetX + w, 0.0f, 0.0f);
	m_VB->Make_Points(scaleMat * trans2 * m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
	m_Sprite->Draw(hDC, m_VB->Get_Points());
}


// ==========================================
// 2. Monster 구현
// ==========================================
GameObject* Monster::Initialize()
{
	__super::Initialize();

	// 두 종류의 몬스터 스프라이트 로드
	m_SpriteType1 = Add_Component<Sprite>();
	m_SpriteType1->Load_Image(L"../Sprites/Idle-Anim_32_4_4.png");
	m_SpriteType2 = Add_Component<Sprite>();
	m_SpriteType2->Load_Image(L"../Sprites/Shoot-Anim_96_12_12.png");

	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);

	Respawn(); // 초기 스폰
	return this;
}

void Monster::Respawn()
{
	m_CurrentType = g_Random->GetInt(0, 1);


	// 바닥 또는 공중(랜덤 높이) 생성
	float spawnY = (m_CurrentType == 0) ? -200.0f : g_Random->GetFloatRange(-200,0);
	m_Transform->Set_Position(-800.0f, spawnY); // 왼쪽 끝에서 스폰
}

void Monster::Update(float dt)
{
	// 오른쪽으로 이동
	XMVECTOR vPos = m_Transform->Get_State(Transform::STATE_POSITION);
	m_Transform->Set_Position(vPos.m128_f32[0] + m_Speed * dt, vPos.m128_f32[1]);

	if (vPos.m128_f32[0] > 800.0f) {
		Respawn(); // 오른쪽 끝 도달 시 다시 스폰
	}

	if (m_CurrentType == 0) m_SpriteType1->Update(dt);
	else m_SpriteType2->Update(dt);
}

void Monster::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	Sprite* curSprite = (m_CurrentType == 0) ? m_SpriteType1 : m_SpriteType2;
	float w = static_cast<float>(curSprite->Get_FrameWidth());
	float h = static_cast<float>(curSprite->Get_FrameHeight());

	if (m_CurrentType == 0)
		m_Transform->Set_Scale(3, 3);
	else
		m_Transform->Set_Scale(2, 2);
	DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(w, h, 1.0f);
	m_VB->Make_Points(scaleMat * m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
	curSprite->Draw(hDC, m_VB->Get_Points());
}


// ==========================================
// 3. Player 구현
// ==========================================
GameObject* Player::Initialize()
{
	__super::Initialize();
	m_SprIdle = Add_Component<Sprite>(); m_SprIdle->Load_Image(L"../Sprites/Walk-Anim_32_4_4.png");
	m_SprExplosion = Add_Component<Sprite>(); m_SprExplosion->Load_Image(L"../Sprites/Explode_64_8_32_fixed.png");

	m_VB = Add_Component<VertexBuffer>();
	m_VB->Set_Shape(QUAD);

	m_Transform->Set_Position(400.0f, m_BaseY); // 초기 우측 배치
	return this;
}

void Player::Update(float dt)
{
	if (m_isDead1) {
		m_SprExplosion->Update(dt);
		return; // 죽으면 조작 불가
	}

	XMVECTOR vPos = m_Transform->Get_State(Transform::STATE_POSITION);
	float posX = vPos.m128_f32[0];
	float posY = vPos.m128_f32[1];

	// 좌우 이동
	if (g_RawInput->Key_Pressing(VK_LEFT)) posX -= m_Speed * dt;
	if (g_RawInput->Key_Pressing(VK_RIGHT)) posX += m_Speed * dt;

	// 점프 로직 (J 키)
	if (m_State != 1 && g_RawInput->Key_Down(VK_UP)) {
		m_State = 1;
		m_VelocityY = 600.0f; // 점프 파워
	}

	// 엎드리기 로직 (아래 화살표) - 점프 중이 아닐 때만
	if (m_State != 1) {
		if (g_RawInput->Key_Pressing(VK_DOWN)) m_State = 2;
		else m_State = 0;
	}

	// 중력 적용
	if (m_State == 1) {
		posY += m_VelocityY * dt;
		m_VelocityY -= m_Gravity * dt;

		if (posY <= m_BaseY) { // 바닥 착지
			posY = m_BaseY;
			m_State = 0;
			m_VelocityY = 0.0f;
		}
	}

	m_Transform->Set_Position(posX, posY);

	// 상태에 맞는 스프라이트 애니메이션 업데이트
	m_SprIdle->Update(dt);
}

void Player::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	Sprite* curSprite = m_SprIdle;
	if (m_isDead1) curSprite = m_SprExplosion;

	float w = static_cast<float>(curSprite->Get_FrameWidth());
	float h = static_cast<float>(curSprite->Get_FrameHeight());

	// 엎드렸을 때 히트박스와 스프라이트 크기 줄이기 (선택사항)
	if (m_State == 2 && !m_isDead1) h *= 0.5f;

	if(!DontDraw){
		DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(w, h, 1.0f);
		m_VB->Make_Points(scaleMat * m_Transform->Get_WorldMatrix(), viewMatrix, projMatrix);
		curSprite->Draw(hDC, m_VB->Get_Points());
	}
}


// ==========================================
// 4. HW5_7 (메인 씬 관리자) 구현
// ==========================================
GameObject* HW5_7::Initialize()
{
	__super::Initialize();

	// 레이어별 속도 다르게 초기화 (하늘은 빠르고 땅은 느리게)
	m_SkyLayer = new BackgroundLayer();
	m_SkyLayer->Initialize(L"../Sprites/sun.png", 200.0f);

	m_GroundLayer = new BackgroundLayer();
	m_GroundLayer->Initialize(L"../Sprites/ground.png", 100.0f)->Get_Transform()->Set_Position(0, -325);

	m_Player = new Player();
	m_Player->Initialize()->Get_Transform()->Set_Scale(3,3);

	m_Monster = new Monster();
	m_Monster->Initialize();

	return this;
}

void HW5_7::Update(float dt)
{
	m_SkyLayer->Update(dt);
	m_GroundLayer->Update(dt);


	m_Player->Update(dt);
	m_Monster->Update(dt);

}

void HW5_7::Late_Update(float dt)
{
	m_Player->Late_Update(dt);
	__super::Late_Update(dt);
	if (m_Player->Is_Dead()) return;

	// 5-5에서 사용한 간단한 AABB 충돌 처리 재사용
	XMVECTOR pPos = m_Player->Get_Transform()->Get_State(Transform::STATE_POSITION);
	XMVECTOR mPos = m_Monster->Get_Transform()->Get_State(Transform::STATE_POSITION);

	float px = pPos.m128_f32[0], py = pPos.m128_f32[1];
	float mx = mPos.m128_f32[0], my = mPos.m128_f32[1];

	// 스프라이트 크기에 맞춰 넉넉하게 충돌 바운더리 50.0f 설정
	if (abs(px - mx) < 50.0f && abs(py - my) < 50.0f) {
		m_Player->Set_Dead(); // 충돌 시 폭발 애니메이션 트리거
	}

	
}

void HW5_7::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	// 그리는 순서(Z-Order)가 중요함! 배경 -> 몬스터 -> 플레이어 순으로 그려야 해
	m_SkyLayer->Draw(hDC, viewMatrix, projMatrix);
	m_GroundLayer->Draw(hDC, viewMatrix, projMatrix);
	m_Monster->Draw(hDC, viewMatrix, projMatrix);
	m_Player->Draw(hDC, viewMatrix, projMatrix);
}