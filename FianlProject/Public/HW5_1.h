#pragma once
#include "RenderObject.h"
#include "shape.h"
#include "Sprite.h"
#include "VertexBuffer.h"
#include <vector>

enum class HW5_1_mode { Div1, Div2, Div4, Div6, End };

class ImagePiece; // 전방 선언

class HW5_1 : public RenderObject {
public:
	HW5_1() : RenderObject() {}
	~HW5_1() { Clear_Pieces(); }

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
	void Create_Pieces();
	void Clear_Pieces();

private:
	Transform* mouseTransform = nullptr;
	std::vector<ImagePiece*> m_Pieces;

	int sizeX = 600;
	int sizeY = 600;
	HW5_1_mode mode = HW5_1_mode::Div1;
	bool isBackgroundMode = false;
};

class ImagePiece : public RenderObject
{
public:
	ImagePiece() : RenderObject() {}
	~ImagePiece() { }

public:
	GameObject* Initialize() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

	// 비율 기반으로 그릴 영역 지정
	void Set_MouseTransform(Transform* mouse) { m_MouseTransform = mouse; }

	void Set_Selected(bool selected) { m_bIsSelected = selected; }
	bool Get_Selected() const { return m_bIsSelected; }
	void Toggle_Reverse() { m_bIsReversed = !m_bIsReversed; }

	bool Check_Collision();

private:
	Sprite* m_Sprite = nullptr;
	VertexBuffer* m_VB = nullptr;
	Shape* m_Shape = nullptr;
	Transform* m_MouseTransform = nullptr;

	bool m_bIsSelected = false;
	bool m_bIsReversed = false;
};