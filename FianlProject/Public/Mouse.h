#pragma once
#include "RenderObject.h"

class Mouse : public RenderObject
{
public:
    Mouse() : RenderObject() {}
    ~Mouse() = default;

public:
    GameObject* Initialize() override;
    void Update(float dt) override;
    void Late_Update(float dt) override;
    void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) override;

private:
    void Lock_Cursor();

private:
    class VertexBuffer* m_VB = nullptr;
    class Sprite* m_Sprite = nullptr;
    class Camera* m_MainCamera = nullptr;
    class Transform* m_MainCameraTransform = nullptr;

private:
    float m_AbsoluteX = 0.f;
    float m_AbsoluteY = 0.f;
};