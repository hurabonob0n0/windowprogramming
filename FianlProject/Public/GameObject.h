#pragma once
#include "Component.h" // Transform 선언 포함
#include "Transform.h"
#include "shape.h"
#include "Sprite.h"


class GameObject {
public:
    GameObject() {
        // 객체가 생성될 때 무조건 Transform 컴포넌트를 하나 추가하고 포인터를 캐싱합니다.
        m_Transform = Add_Component<Transform>();
    }
    ~GameObject() = default;

public:
    virtual GameObject* Initialize() = 0;
    virtual void Update(float dt) = 0;
    virtual void Late_Update(float dt) = 0;
    virtual void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix) = 0;

public:
    Transform* Get_Transform() const { return m_Transform; }
    template <typename T>
    T* Add_Component() {
        m_Components.push_back(std::make_unique<T>(this));
        return static_cast<T*>(m_Components.back().get());
    }
    template <typename T>
    T* Get_Component() {
        for (auto& comp : m_Components) {
            T* target = dynamic_cast<T*>(comp.get());
            if (target) return target;
        }
        return nullptr;
    }

protected:
    std::vector<std::unique_ptr<Component>> m_Components;
    Transform* m_Transform{ nullptr }; // 빠른 접근을 위한 캐싱 포인터
};