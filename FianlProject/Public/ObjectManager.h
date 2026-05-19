#pragma once
#include "Utils.hpp"
//Components
#include "Shape.h"
#include "Sprite.h"
#include "Transform.h"
#include "VertexBuffer.h"

//Objects
#include "GameObject.h"
#include "RenderObject.h"
#include "Camera.h"
#include "BaseObject.h"
#include "BaseSpriteObject.h"
#include "Mouse.h"
#include "HW5_1.h"
#include "HW5_5.h"

// ObjectManager.h
class ObjectManager {

public:
	void Update(float dt) {
		for (auto& obj : m_Objects) {
			obj->Update(dt);
		}
	}
	void Late_Update(float dt) {
		for (auto iter = m_Objects.begin(); iter != m_Objects.end(); ) {
			(*iter)->Late_Update(dt);
			if ((*iter)->Is_Dead())
				iter = m_Objects.erase(iter);
			else
				++iter;
		}
	}
	void Add_Object(std::unique_ptr<class GameObject> obj) {
		m_Objects.push_back(std::move(obj));
	}
	GameObject* Find_Object(string name) {
		for (auto& obj : m_Objects) {
			if (obj->Get_Name() == name)
				return obj.get();
		}
	}

private:
	std::list<std::unique_ptr<class GameObject>> m_Objects;
};