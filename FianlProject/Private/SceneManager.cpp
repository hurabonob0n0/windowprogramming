#include "SceneManager.h"

unique_ptr<SceneManager> SceneManager::m_Instance = nullptr;

void Basic_Scene::Initialize()
{
	unique_ptr<GameObject> camera = make_unique<Camera>();
	camera->Initialize()->Set_Name("Camera0");
	m_ObjMgr.Add_Object(std::move(camera));

	unique_ptr<GameObject> mouse = make_unique<Mouse>();
	mouse->Initialize()->Set_Name("Mouse0");
	m_ObjMgr.Add_Object(std::move(mouse));

	//unique_ptr<GameObject> baseObj = make_unique<BaseObject>();
	//baseObj->Initialize()->Set_Name("Quad");
	//m_ObjMgr.Add_Object(std::move(baseObj));

	/*unique_ptr<GameObject> Miho = make_unique<BaseSpriteObject>();
	Miho->Initialize()->Set_Name("Miho");
	m_ObjMgr.Add_Object(std::move(Miho));*/

	unique_ptr<GameObject> Miho = make_unique<Cat>();
	Miho->Initialize()->Set_Name("Miho");
	m_ObjMgr.Add_Object(std::move(Miho));

	/*unique_ptr<GameObject> Miho = make_unique<HW5_1>();
	Miho->Initialize()->Set_Name("Miho");
	m_ObjMgr.Add_Object(std::move(Miho));*/

}
