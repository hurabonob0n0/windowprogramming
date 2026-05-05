// RenderManager.h
#pragma once
#include "Utils.hpp"
#include "GameObject.h"

class RenderManager {
public:
    RenderManager() {
        // 초기 행렬은 단위 행렬(Identity)로 세팅
        m_ViewMatrix = DirectX::XMMatrixIdentity();
        m_ProjMatrix = DirectX::XMMatrixIdentity();
    }
    ~RenderManager() = default;

private:
    static std::unique_ptr<RenderManager> m_Instance;

public:
    static RenderManager* Get_Instance() {
        if (!m_Instance) {
            m_Instance = std::unique_ptr<RenderManager>(new RenderManager());
        }
        return m_Instance.get();
    }
    static void Destroy_Instance() {
        m_Instance.reset();
    }

public:
    // 카메라가 매 프레임 이 함수들을 호출하여 행렬을 갱신합니다.
    void Set_ViewMatrix(DirectX::FXMMATRIX view) { m_ViewMatrix = view; }
    void Set_ProjMatrix(DirectX::CXMMATRIX proj) { m_ProjMatrix = proj; }

    //// 이번 프레임에 그려질 객체를 등록합니다.
    //void Add_RenderObject(GameObject* obj) { m_RenderObjects.push_back(obj); }

    //// 렌더링이 끝나면 리스트를 비워줍니다.
    //void Clear_RenderObjects() { m_RenderObjects.clear(); }

    //// 등록된 모든 객체에게 View, Proj 행렬을 넘기며 Draw를 호출합니다.
    //void Render_All(HDC hDC) {
    //    for (auto& obj : m_RenderObjects) {
    //        if (obj) {
    //            obj->Draw(hDC, m_ViewMatrix, m_ProjMatrix);
    //        }
    //    }
    //}

public:
    XMMATRIX Get_ViewMatrix()const { return m_ViewMatrix; }
    XMMATRIX Get_ProjMatrix()const { return m_ProjMatrix; }



private:
    DirectX::XMMATRIX m_ViewMatrix;
    DirectX::XMMATRIX m_ProjMatrix;
    //std::list<GameObject*> m_RenderObjects;
};