#include "Sprite.h"
#include "GameObject.h"
#include <gdiplus.h>

using namespace Gdiplus;

// 정적 멤버 초기화
GdiplusStartupInput Sprite::gdiplusStartupInput;
ULONG_PTR Sprite::gdiplusToken = 0;
int Sprite::initCount = 0;

Sprite::Sprite(GameObject* owner)
	: Component(owner), m_pImage(nullptr), m_pTransform(nullptr),
	  m_Width(0.0f), m_Height(0.0f), m_Opacity(255)
{
	m_pTransform = owner->Get_Transform();
	Initialize_GDIPlus();
}

Sprite::~Sprite()
{
	if (m_pImage) {
		delete m_pImage;
		m_pImage = nullptr;
	}
	Shutdown_GDIPlus();
}

void Sprite::Initialize_GDIPlus()
{
	if (initCount == 0) {
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
	}
	initCount++;
}

void Sprite::Shutdown_GDIPlus()
{
	initCount--;
	if (initCount == 0 && gdiplusToken) {
		GdiplusShutdown(gdiplusToken);
		gdiplusToken = 0;
	}
}

bool Sprite::Load_Image(const std::string& filePath)
{
	// 문자열을 WCHAR로 변환
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &filePath[0], (int)filePath.size(), NULL, 0);
	std::wstring wfilePath(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &filePath[0], (int)filePath.size(), &wfilePath[0], size_needed);

	// 기존 이미지 해제
	if (m_pImage) {
		delete m_pImage;
	}

	// 새 이미지 로드
	m_pImage = new Image(wfilePath.c_str());

	// 이미지 로드 확인
	if (m_pImage->GetLastStatus() != Ok) {
		delete m_pImage;
		m_pImage = nullptr;
		return false;
	}

	// 크기 설정
	m_Width = (float)m_pImage->GetWidth();
	m_Height = (float)m_pImage->GetHeight();

	return true;
}

void Sprite::Draw(HDC hDC, DirectX::FXMMATRIX viewMatrix, DirectX::CXMMATRIX projMatrix)
{
	if (!m_pImage || !m_pTransform) return;

	// Transform에서 위치 정보 가져오기
	DirectX::XMVECTOR pos = m_pTransform->Get_State(Transform::STATE_POSITION);
	float x = DirectX::XMVectorGetX(pos);
	float y = DirectX::XMVectorGetY(pos);

	// Graphics 객체 생성
	Graphics graphics(hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	// 투명도 설정
	ColorMatrix colorMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, (float)m_Opacity / 255.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	ImageAttributes imageAttrs;
	imageAttrs.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	// 이미지 그리기
	graphics.DrawImage(m_pImage, RectF(x, y, m_Width, m_Height), 0, 0, 
		(float)m_pImage->GetWidth(), (float)m_pImage->GetHeight(), UnitPixel, &imageAttrs);
}