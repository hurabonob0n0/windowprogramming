#pragma once
#include "Component.h"
#include "Transform.h"
#include <gdiplus.h>
#include <string>

#pragma comment(lib, "gdiplus.lib")

class Sprite : public Component {
public:
	Sprite(GameObject* owner);
	virtual ~Sprite();

public:
	// 이미지 로드
	bool Load_Image(const std::string& filePath);
	
	// 그리기
	void Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix);

	// 투명도 설정 (0-255)
	void Set_Opacity(int opacity) { m_Opacity = opacity; }
	int Get_Opacity() const { return m_Opacity; }

	// 크기 조정
	void Set_Width(float width) { m_Width = width; }
	void Set_Height(float height) { m_Height = height; }
	float Get_Width() const { return m_Width; }
	float Get_Height() const { return m_Height; }

private:
	Gdiplus::Image* m_pImage;
	Transform* m_pTransform;
	
	float m_Width;
	float m_Height;
	int m_Opacity;

	static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	static ULONG_PTR gdiplusToken;
	static int initCount;

	void Initialize_GDIPlus();
	void Shutdown_GDIPlus();
};