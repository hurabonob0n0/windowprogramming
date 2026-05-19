// Transform.h
#pragma once
#include "Component.h"



class Transform : public Component {
public:
    enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
    Transform(GameObject* owner) : Component(owner) {
        XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
    }
    virtual ~Transform() = default;

public:
    //Getters
    XMVECTOR Get_State(STATE eState) const {
        return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
    }
    XMFLOAT3 Get_Scaled() const {
        return XMFLOAT3(XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
            XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
            XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
    }
    XMMATRIX Get_WorldMatrix() const { return XMLoadFloat4x4(&m_WorldMatrix); }
    XMMATRIX Get_WorldMatrixInv() const { return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)); }

public:
    //Setters
    void Set_WorldMatrix(const XMMATRIX& mat) { XMStoreFloat4x4(&m_WorldMatrix, mat); }
    void Set_Scale(float x, float y, float z = 1.f) {
        // 1. 현재 방향 벡터들을 가져와서 크기를 1로 정규화(Normalize)합니다.
        XMVECTOR vRight = XMVector3Normalize(Get_State(STATE_RIGHT));
        XMVECTOR vUp = XMVector3Normalize(Get_State(STATE_UP));
        XMVECTOR vLook = XMVector3Normalize(Get_State(STATE_LOOK));

        // 2. 정규화된 방향 벡터에 새로운 스케일(길이)을 곱해줍니다.
        Set_State(STATE_RIGHT, vRight * x);
        Set_State(STATE_UP, vUp * y);
        Set_State(STATE_LOOK, vLook * z);
    }
    void Set_TransformState(float x, float y, float width, float height, float rotDegree) {
        XMMATRIX matScale = XMMatrixScaling(width, height, 1.0f);
        XMMATRIX matRot = XMMatrixRotationZ(XMConvertToRadians(rotDegree));
        XMMATRIX matTrans = XMMatrixTranslation(x, y, 0.0f);

        // 크기 -> 회전 -> 이동 순으로 행렬 곱셈
        XMMATRIX matWorld = matScale * matRot * matTrans;
        XMStoreFloat4x4(&m_WorldMatrix, matWorld);
    };
    void Set_State(STATE eState, XMVECTOR vState)
    {
        XMMATRIX TransformMatrix = XMLoadFloat4x4(&m_WorldMatrix);
        TransformMatrix.r[eState] = vState;
        XMStoreFloat4x4(&m_WorldMatrix, TransformMatrix);
    }
    void Set_Position(float x, float y) {
        m_WorldMatrix._41 = x;
        m_WorldMatrix._42 = y;
    }

public:
    //Transform
    void Turn(float radxTimeDelta) {
        // 1. 현재 스케일을 안전하게 구해둡니다.
        XMFLOAT3 vScaled = Get_Scaled();

        // 2. 방향 벡터를 가져온 뒤, 반드시 '정규화(Normalize)'하여 길이를 1로 만듭니다. (가장 중요!)
        XMVECTOR vRight = XMVector3Normalize(Get_State(STATE_RIGHT));
        XMVECTOR vUp = XMVector3Normalize(Get_State(STATE_UP));
        XMVECTOR vLook = XMVector3Normalize(Get_State(STATE_LOOK));

        // 3. 회전 행렬을 만듭니다.
        XMMATRIX RotationMatrix = XMMatrixRotationZ(radxTimeDelta);

        // 4. 길이가 1인 방향 벡터를 회전시킨 후, 아까 구해둔 스케일을 다시 곱해줍니다.
        // (방향 벡터를 변환할 때는 XMVector3TransformNormal이 논리적으로 더 안전합니다.)
        Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix) * vScaled.x);
        Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix) * vScaled.y);
        Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix) * vScaled.z);
    }
    void Go_Right(float SpeedXTimeDelta) {
        m_WorldMatrix._41 += SpeedXTimeDelta;
    }
    void Go_Up(float SpeedXTimeDelta) {
        m_WorldMatrix._42 += SpeedXTimeDelta;
    }
    void Go_Left(float SpeedXTimeDelta) {
        m_WorldMatrix._41 -= SpeedXTimeDelta;
    }
    void Go_Down(float SpeedXTimeDelta) {
        m_WorldMatrix._42 -= SpeedXTimeDelta;
    }
    void Go_Dir(float radian, float SpeedxTD) {
        float Xdir = cos(radian);
        float Ydir = sin(radian);
        m_WorldMatrix._41 += Xdir * SpeedxTD;
        m_WorldMatrix._42 += Ydir * SpeedxTD;
    }
    void Go_to_Pos(float Xpos, float Ypos, float SpeedxTD) {
        float Xdelta = Xpos - m_WorldMatrix._41;
        float Ydelta = Ypos - m_WorldMatrix._42;

        XMVECTOR direction = XMVectorSet(Xdelta, Ydelta, 0.0f, 0.0f);
        XMVECTOR normalizedDir = XMVector2Normalize(direction);
        XMVECTOR velocity = XMVectorScale(normalizedDir, SpeedxTD);

        m_WorldMatrix._41 += XMVectorGetX(velocity);
        m_WorldMatrix._42 += XMVectorGetY(velocity);
    }

private:
    XMFLOAT4X4 m_WorldMatrix;
};