#include <cmath>
#include "HW4_3.h"
#include "RawInput.hpp"

//=======================================HW4-3=====================================
GameObject* HW4_3::Initialize()
{
	int sizeX{ 100 }, sizeY{ 25 };
	int marginX = 600, marginY = 100;

	m_Blocks.resize(3);

	for (int i = 2; i >= 0; --i) {
		for (int j = 0; j < 10; ++j) {
			Block* pBlock = new Block();
			pBlock->Set_TransformState(marginX / 2.f + sizeX / 2.f + j * sizeX,
				g_WinInfo.WinCY - marginY - sizeY * 0.5f - i * sizeY,
				sizeX, sizeY);
			m_Blocks[2 - i].push_back((Block*)pBlock->Initialize());
		}
	}

	m_Bar = new Bar();
	m_Bar->Initialize();

	m_Ball = new Ball();
	m_Ball->Initialize();
	return this;
}

void HW4_3::Update(float dt)
{
	if (g_RawInput->Key_Down('Q'));
	if (g_RawInput->Key_Pressing('Q'));
	if (g_RawInput->Key_Up('Q'));


	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 10; ++j) {
			m_Blocks[i][j]->Update(dt);
		}
	}
	//m_Bar->Update(dt);
}

void HW4_3::Late_Update(float dt)
{
	// 1. 키보드 입력 처리 (P: 일시정지, S: 시작, +/-: 속도 조절 등)
	if (g_RawInput->Key_Down('P')) {
		m_isPaused = !m_isPaused;
		// 콘솔창에 색이 변한 개수, 없어진 개수 출력 로직
		// std::cout << "변한 벽돌: " << m_ChangedColorCount << "\n";
	}

	if (m_isPaused) return; // 일시정지면 아래 로직을 스킵

	//if (g_RawInput->Key_Down('S')) m_Ball->Fire();
	if (GetAsyncKeyState('S') & 0x8000) m_Ball->Fire();
	if (g_RawInput->Key_Down(VK_ADD)) m_Ball->Set_SpeedMultiplier(50.0f);
	if (g_RawInput->Key_Down(VK_SUBTRACT)) m_Ball->Set_SpeedMultiplier(-50.0f);

	// 2. 바와 공 업데이트
	m_Bar->Update(dt);

	

	// 3. 벽돌 좌우 이동 (Grid 이동)
	m_GridOffsetX += m_GridMoveDir * 100.0f * dt;
	if (m_GridOffsetX > 200.0f || m_GridOffsetX < -200.0f) m_GridMoveDir *= -1.0f;
	// 모든 Block들의 Transform 위치에 m_GridOffsetX를 반영해 주어야 함

	// 4. 간단한 화면 벽 충돌 로직
	if (m_Ball->Get_X() <= 0 || m_Ball->Get_X() >= g_WinInfo.WinCX) m_Ball->Reverse_X();
	if (m_Ball->Get_Y() >= g_WinInfo.WinCY) m_Ball->Reverse_Y();

	// 바닥으로 떨어졌을 때 (놓침)
	if (m_Ball->Get_Y() <= 0) {
		m_Ball->Reset_Position(m_Bar->Get_Transform()->Get_State(Transform::STATE_POSITION).m128_f32[0],
			m_Bar->Get_Transform()->Get_State(Transform::STATE_POSITION).m128_f32[1]);
	}

	// ==========================================
	// 5. 바(Bar)와의 AABB 충돌 검사
	// ==========================================
	float bX = m_Ball->Get_X();
	float bY = m_Ball->Get_Y();
	float bHalfW = 10.0f; // 공 너비 20 / 2
	float bHalfH = 10.0f; // 공 높이 20 / 2

	float barX = m_Bar->Get_Transform()->Get_State(Transform::STATE_POSITION).m128_f32[0];
	float barY = m_Bar->Get_Transform()->Get_State(Transform::STATE_POSITION).m128_f32[1];
	float barHalfW = 10000.0f; // 바 너비 200 / 2
	float barHalfH = 25.0f;  // 바 높이 50 / 2

	// AABB 충돌 판정 (공 vs 바)
	if (bX + bHalfW > barX - barHalfW &&  // 공의 우측 > 바의 좌측
		bX - bHalfW < barX + barHalfW &&  // 공의 좌측 < 바의 우측
		bY + bHalfH > barY - barHalfH &&  // 공의 하단 > 바의 상단
		bY - bHalfH < barY + barHalfH)    // 공의 상단 < 바의 하단
	{
		int v = m_Bar->Get_VelocityX();
		m_Ball->Bounce_Off_Bar(v);
		//cout << "v : " << v << endl;

		// 파고드는 현상(중복 충돌)을 막기 위해 바 바로 위로 위치 강제 보정
		m_Ball->Get_Transform()->Set_Position(bX, barY + barHalfH + bHalfH + 1.0f);
	}

	// ==========================================
	// 6. 벽돌(Block)과의 인덱스 기반 AABB 충돌 검사
	// ==========================================
	// 회원님의 아이디어 적용! 단, GridOffsetX(벽돌 전체 이동량)를 빼주어야 정확한 열(Column)을 찾습니다.
	int col = (int)(bX - 300.0f) / 100;

	// 공이 블록 그리드 범위(0~9열) 안에 있을 때만 검사
	if (col >= 0 && col < 10) {
		for (int row = 0; row < 3; ++row) {
			Block* pBlock = m_Blocks[row][col];

			// 이미 부딪혀서 떨어지는 중인 벽돌은 무시
			if (pBlock->Is_Hit()) continue;

			float blockX = pBlock->Get_Transform()->Get_State(Transform::STATE_POSITION).m128_f32[0];
			float blockY = pBlock->Get_Transform()->Get_State(Transform::STATE_POSITION).m128_f32[1];
			float blockHalfW = 50.0f; // 블록 너비 100 / 2
			float blockHalfH = 12.5f; // 블록 높이 25 / 2

			// AABB 충돌 판정 (공 vs 해당 열의 블록)
			if (bX + bHalfW > blockX - blockHalfW &&
				bX - bHalfW < blockX + blockHalfW &&
				bY + bHalfH > blockY - blockHalfH &&
				bY - bHalfH < blockY + blockHalfH)
			{
				float distX = bX - blockX; // X축 거리차
				float distY = bY - blockY; // Y축 거리차

				// X축, Y축 각각 얼만큼 파고들었는지 계산 (절댓값 사용)
				float overlapX = (bHalfW + blockHalfW) - std::abs(distX);
				float overlapY = (bHalfH + blockHalfH) - std::abs(distY);

				// 침투 깊이가 더 얕은 쪽이 실제 부딪힌 면!
				if (overlapX < overlapY) {
					// X축 침투가 더 얕음 -> 왼쪽이나 오른쪽 면에 부딪힘
					m_Ball->Reverse_X();

					// 파고든 방향의 반대쪽으로 밀어내기 (위치 보정)
					if (distX > 0) m_Ball->Get_Transform()->Set_Position(bX + overlapX + 1.0f, bY); // 블록 우측
					else           m_Ball->Get_Transform()->Set_Position(bX - overlapX - 1.0f, bY); // 블록 좌측
				}
				else {
					// Y축 침투가 더 얕음 -> 위나 아래 면에 부딪힘
					m_Ball->Reverse_Y();

					// 파고든 방향의 반대쪽으로 밀어내기 (위치 보정)
					if (distY > 0) m_Ball->Get_Transform()->Set_Position(bX, bY + overlapY + 1.0f); // 블록 하단
					else           m_Ball->Get_Transform()->Set_Position(bX, bY - overlapY - 1.0f); // 블록 상단
				}

				// 2. 부딪힌 블록 추락 상태로 변경
				pBlock->Set_Hit();
			
				m_DisappearedCount++;

				// 3. 부딪힌 줄(row)의 모든 벽돌 랜덤 색상 변경 (요구사항 15번)
				int r = g_Random->GetInt(0, 255);
				int g = g_Random->GetInt(0, 255);
				int b = g_Random->GetInt(0, 255);

				for (int j = 0; j < 10; ++j) {
					if (!m_Blocks[row][j]->Is_Hit()) { // 아직 안 떨어진 애들만 색깔 변경
						m_Blocks[row][j]->Change_Color(r, g, b);
						m_ChangedColorCount++;
					}
				}

				break; // 한 번에 한 개의 벽돌만 깨도록 루프 탈출
			}
		}
	}

	if (!m_Ball->m_isFired) {
		// 공이 발사되기 전에는 바를 따라다녀야 함
		m_Ball->Reset_Position(m_Bar->Get_Transform()->Get_State(Transform::STATE_POSITION).m128_f32[0],
			m_Bar->Get_Transform()->Get_State(Transform::STATE_POSITION).m128_f32[1]);
	}
	else {
		m_Ball->Update(dt);
	}
}

void HW4_3::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	// 이렇게 했을 때 왜 오류가 났을까?
	/*int i = 0, j = 0;
	for (auto& vBlocks : m_Blocks) {
		for (auto& block : vBlocks) {
			Draw(hDC, viewMatrix, projMatrix);
			++j;
		}
		++i;
	}*/

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 10; ++j) {
			m_Blocks[i][j]->Draw(hDC, viewMatrix, projMatrix);
		}
	}

	m_Bar->Draw(hDC, viewMatrix, projMatrix);

	m_Ball->Draw(DrawInputs);
}


//=======================================Block=====================================

GameObject* Block::Initialize()
{
	m_Shape = Add_Component<Shape>();
	m_Shape->Set_Shape(QUAD)->Set_Fill(RGB(127, 0, 255))->Set_Line(RGB(63, 0, 127), 3);
	return this;
}

void Block::Update(float dt)
{
	if (m_isFalling) {
		// 부딪혀서 떨어지는 상태일 때
		float currentY = m_Transform->Get_State(Transform::STATE_POSITION).m128_f32[1];
		m_Transform->Set_Position(m_Transform->Get_State(Transform::STATE_POSITION).m128_f32[0], currentY - (m_FallSpeed * dt));

		// 계속해서 랜덤 색상 부여 (Random_Engine 활용)
		Change_Color(g_Random->GetInt(0, 255), g_Random->GetInt(0, 255), g_Random->GetInt(0, 255));
	}
}

void Block::Late_Update(float dt)
{
}

void Block::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	m_Shape->Draw(hDC, viewMatrix, projMatrix);
}

void Block::After_Hit_Color_Change()
{
}


//=========================Bar=====================================

GameObject* Bar::Initialize()
{
	m_Shape = Add_Component<Shape>();
	m_Shape->Set_Shape(QUAD)->Set_Fill(RGB(255, 127, 0))->Set_Line(RGB(127, 63, 0), 3);

	m_Transform->Set_TransformState(800, 100, 200, 50, 0);
	return this;
}

void Bar::Update(float dt)
{
	POINT pt = { 0, 0 };
	GetCursorPos(&pt);
	ScreenToClient(g_WinInfo.hWnd, &pt);

	float currentX = pt.x;

	m_VelocityX = (currentX - m_PrevX) / dt;

	m_PrevX = currentX; // 현재 위치 기억

	m_Transform->Set_Position(currentX, 100);
}

void Bar::Late_Update(float dt)
{
}

void Bar::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	m_Shape->Draw(hDC, viewMatrix, projMatrix);
}


//==========================Ball=========================


GameObject* Ball::Initialize()
{
	m_Shape = Add_Component<Shape>();
	// 공 크기를 20x20으로 설정 (원 모양)
	m_Shape->Set_Shape(CIRCLE)->Set_Fill(RGB(0, 0, 255));
	m_Transform->Set_TransformState(800, 200, 20, 20, 0);
	return this;
}

void Ball::Update(float dt)
{
	if (!m_isFired) return; // S키를 누르기 전에는 움직이지 않음

	// 방향 * 속도 * dt 로 이동
	float currentX = Get_X();
	float currentY = Get_Y();
	m_Transform->Set_Position(currentX + (m_DirX * dt), currentY + (m_DirY * dt));
	//cout << "x : " << m_DirX << ",  y : " << m_DirY << endl;
}

void Ball::Late_Update(float dt)
{
}

void Ball::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	m_Shape->Draw(DrawInputs);
}

void Ball::Reset_Position(float barX, float barY)
{
	m_isFired = false;
	m_Transform->Set_Position(barX, barY + 25.0f); // 바의 살짝 위쪽에 안착
}

//void Ball::Fire()
//{
//	if (!m_isFired) {
//		m_isFired = true;
//		// 튕기는 방향 대각선 설정 (예: 우상단)
//		m_DirX = 1000.0f;
//		m_DirY = 1000.0f;
//	}
//}

void Ball::Fire()
{
	if (!m_isFired) {
		m_isFired = true;

		// 1. 튕기는 방향 대각선 비율 설정 (예: 우상단 1:1)
		m_DirX = 1.0f;
		m_DirY = 1.0f;

		// 2. 방향 벡터 정규화
		float length = std::sqrt(m_DirX * m_DirX + m_DirY * m_DirY);
		if (length != 0.0f) {
			m_DirX /= length;
			m_DirY /= length;
		}

		// 3. 고정 속도 곱하기
		m_DirX *= m_Speed;
		m_DirY *= m_Speed;
	}
}

//void Ball::Bounce_Off_Bar(float barVelocityX)
//{	
//	float MaxSpeed = 1500;
//	float MinSpeedY = 300;
//
//	if (barVelocityX > 0 && m_DirX > 0) {
//		m_DirX += 300;
//		m_DirY = MaxSpeed - m_DirX;
//		m_DirY = max(m_DirY, MinSpeedY);
//		m_DirX = MaxSpeed - m_DirY;
//	}
//	else if (barVelocityX < 0 && m_DirX < 0) {
//		m_DirX -= 300;
//		m_DirY = MaxSpeed + m_DirX;
//		m_DirY = max(m_DirY, MinSpeedY);
//		m_DirX = -MaxSpeed + m_DirY;
//	}
//	else if (barVelocityX > 0 && m_DirX < 0) {
//		m_DirX += 300;
//		m_DirY = abs(MaxSpeed - m_DirX);
//	}
//	else if (barVelocityX < 0 && m_DirX > 0) {
//		m_DirX -= 300;
//		m_DirY = abs(MaxSpeed - m_DirX);
//	}
//	else
//		m_DirY = abs(m_DirY);
//	//cout << "X : " << m_DirX << "Y : " << m_DirY << endl;
//}

void Ball::Bounce_Off_Bar(float barVelocityX)
{
	// 1. 공은 무조건 위로 튕기도록 설정 (절댓값 씌우기)
	m_DirY = std::abs(m_DirY);

	// 2. 바의 이동 속도(스핀) 반영
	// 속도값이 너무 크면 각도가 휙휙 꺾이므로 마찰(friction) 비율을 곱해줍니다.
	float frictionWeight = 0.05f;
	m_DirX += (barVelocityX * frictionWeight);

	// 3. 대망의 방향 벡터 정규화 (Normalize) - 피타고라스의 정리
	float length = std::sqrt(m_DirX * m_DirX + m_DirY * m_DirY);
	if (length != 0.0f) {
		m_DirX /= length; // 길이를 1로 만듦 (비율만 남김)
		m_DirY /= length;
	}

	// [보너스 방어코드] 공이 너무 가로로 누워서(Y비율이 낮아서) 무한반사되는 것 방지
	if (m_DirY < 0.3f) {
		m_DirY = 0.3f;
		// Y값을 올렸으니 다시 한번 정규화
		length = std::sqrt(m_DirX * m_DirX + m_DirY * m_DirY);
		m_DirX /= length;
		m_DirY /= length;
	}

	// 4. 고정 속도(m_Speed) 곱하기
	m_DirX *= m_Speed;
	m_DirY *= m_Speed;
}