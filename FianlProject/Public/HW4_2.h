#pragma once
#include "GameObject.h"

//1. 가로 도로 + 중앙선, 세로 도로 + 중앙선, 사람 + 횡단보도, 자동차 8대, 가로 신호등, 세로 신호등 그리기
class Road : public GameObject {
public:
	Road() {};
	~Road() {};

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

};

class HW4_2 : public GameObject
{
public:
	HW4_2() : GameObject() {}
	~HW4_2() = default;

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:

};