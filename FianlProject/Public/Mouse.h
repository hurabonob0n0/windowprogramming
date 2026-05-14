#pragma once
#include "GameObject.h"

class Mouse : public GameObject
{
public:
    Mouse() : GameObject() {}
    ~Mouse() = default;

public:
    GameObject* Initialize() override;
    void Update(float dt) override;
    void Late_Update(float dt) override;
    void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
    class VertexBuffer* m_VB = nullptr;
    class Sprite* m_Sprite = nullptr;

    // Delta 값을 누적할 실제 마우스 좌표
    float m_AbsoluteX = 0.f;
    float m_AbsoluteY = 0.f;
};