#pragma once
#include <atlimage.h>
#include "Component.h"
#include <unordered_map>
#include <string>
#include <vector>

class Sprite : public Component { 
public:
	Sprite(GameObject* Owner) : Component(Owner) {}
	virtual ~Sprite() {}

public:
	void Update(float dt);
	void Draw(HDC hDC, const std::vector<POINT>& Points);

public:
	bool Load_Image(LPCTSTR filePath);

private:
	static std::unordered_map<std::string, CImage> g_Sprites;

private:
	int				m_totalFrameCnt = 1;
	int				m_ColumnCnt = 1;
	int				m_RowCnt = 1;
	int				m_CurFrame = 0;
	int				m_Width = 0;
	int				m_Height = 0;
	int				m_FrameWidth = 0;
	int				m_FrameHeight = 0;
	float			m_CurFrameTime = 0.f;
	float			m_FrameTime = 1.f;
	std::string		m_Name;
};