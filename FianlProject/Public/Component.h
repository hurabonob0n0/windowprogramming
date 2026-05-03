#pragma once
#include "Utils.hpp"

class GameObject; // 전방 선언 (순환 참조 방지)

class Component {
public:
    Component(GameObject* owner) : m_Owner(owner) {}
    virtual ~Component() {};

protected:
    GameObject* m_Owner{ nullptr }; // 나를 소유하고 있는 게임 오브젝트
};