#pragma once
#include "ObjectManager.h"

class Scene {
public:
    virtual ~Scene() = default;
    virtual void Initialize() = 0;
    virtual void Update(float dt) { m_ObjMgr.Update(dt); }
    virtual void Late_Update(float dt) { m_ObjMgr.Late_Update(dt); }

public:
    void Add_Object(unique_ptr<GameObject> obj) { m_ObjMgr.Add_Object(std::move(obj)); }
    GameObject* Get_Object(string name) { return m_ObjMgr.Find_Object(name); }

protected:
    ObjectManager m_ObjMgr;
};

class SceneManager {
public:
    SceneManager() {}
    ~SceneManager() = default;

public:
    static unique_ptr<SceneManager> m_Instance;

public:
    static SceneManager* Get_Instance() {
        if (m_Instance == nullptr)
            m_Instance = unique_ptr<SceneManager>(new SceneManager());
        return m_Instance.get();
    }

public:
    void Update(float dt) { if (m_ActiveScene) m_ActiveScene->Update(dt); }
    void Late_Update(float dt) { if (m_ActiveScene) m_ActiveScene->Late_Update(dt); }


    template <typename T>
    Scene* Change_Scene() {
        m_ActiveScene = std::make_unique<T>();
        m_ActiveScene->Initialize();
        return m_ActiveScene.get();
    }

    Scene* Get_ActiveScene() { return m_ActiveScene.get(); }
private:
    std::unique_ptr<Scene> m_ActiveScene;
};

class Basic_Scene : public Scene{
public:
    Basic_Scene() : Scene() {}
    ~Basic_Scene() = default;

public:
    void Initialize() override;
};