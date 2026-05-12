#include "Sprite.h"
#include <sstream>
#include <cctype>
#include <algorithm>

// 정적 멤버 정의
std::unordered_map<std::string, CImage> Sprite::g_Sprites;

// wstring 에서 string 으로 변환
static std::string WStringToString(const std::wstring& ws) {
	if (ws.empty()) return {};
	int size = ::WideCharToMultiByte(CP_ACP, 0, ws.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (size <= 0) return {};
	std::string s(size - 1, '\0');
	::WideCharToMultiByte(CP_ACP, 0, ws.c_str(), -1, &s[0], size, nullptr, nullptr);
	return s;
}

void Sprite::Update(float dt) {
	if (m_FrameTime <= 0.f) return;

	m_CurFrameTime += dt;
	while (m_CurFrameTime >= m_FrameTime) {
		m_CurFrameTime -= m_FrameTime;
		++m_CurFrame;
		if (m_CurFrame >= m_totalFrameCnt) {
			m_CurFrame = 0;
		}
	}
}

bool Sprite::Load_Image(LPCTSTR filePath) {
	// 1. 파일명 추출 및 메타데이터 파싱을 가장 먼저 수행합니다.
	std::wstring wpath(filePath);
	size_t pos = wpath.find_last_of(L"\\/");
	std::wstring fname = (pos == std::wstring::npos) ? wpath : wpath.substr(pos + 1);
	size_t dot = fname.find_last_of(L'.');
	std::wstring nameNoExt = (dot == std::wstring::npos) ? fname : fname.substr(0, dot);

	std::string nameAscii = WStringToString(nameNoExt);

	std::vector<std::string> tokens;
	std::stringstream ss(nameAscii);
	std::string token;
	while (std::getline(ss, token, '_')) {
		tokens.push_back(token);
	}

	if (tokens.size() >= 4) {
		m_Name = tokens[0];
		m_totalFrameCnt = max(1, std::atoi(tokens[1].c_str()));
		m_ColumnCnt = max(1, std::atoi(tokens[2].c_str()));
		int fps = max(1, std::atoi(tokens[3].c_str()));
		m_FrameTime = 1.f / static_cast<float>(fps);
	}
	else {
		m_Name = nameAscii;
		m_totalFrameCnt = 1;
		m_ColumnCnt = 1;
		m_FrameTime = 1.f;
	}

	// 2. 이미 캐시에 같은 이름의 이미지가 로드되어 있는지 확인합니다 (중복 방지).
	if (g_Sprites.find(m_Name) == g_Sprites.end()) {
		// 3. 캐시에 없다면 지역 변수 없이 맵의 CImage 객체에 직접 Load 합니다!
		if (g_Sprites[m_Name].Load(filePath) == E_FAIL) {
			g_Sprites.erase(m_Name); // 로드 실패 시 빈 객체 제거
			return false;
		}
	}

	// 4. 로드가 완료된(또는 이미 존재하는) 안전한 참조(&)를 가져와서 설정합니다.
	CImage& safeImg = g_Sprites[m_Name];

	m_RowCnt = (m_totalFrameCnt + m_ColumnCnt - 1) / m_ColumnCnt;
	m_CurFrame = 0;
	m_CurFrameTime = 0.f;

	m_Width = safeImg.GetWidth();
	m_Height = safeImg.GetHeight();
	m_FrameWidth = (m_ColumnCnt > 0) ? (m_Width / m_ColumnCnt) : m_Width;
	m_FrameHeight = (m_RowCnt > 0) ? (m_Height / m_RowCnt) : m_Height;

	return true;
}

void Sprite::Draw(HDC hDC, const std::vector<POINT>& Points) {
	if (g_Sprites.find(m_Name) == g_Sprites.end()) {
		return;
	}
	if (Points.size() < 4) {
		return;
	}

	// 현재 프레임의 소스 좌표 계산
	int srcX = (m_CurFrame % m_ColumnCnt) * m_FrameWidth;
	int srcY = (m_CurFrame / m_ColumnCnt) * m_FrameHeight;

	// 대상 영역의 바운딩 박스 계산
	long minX = Points[0].x, maxX = Points[0].x;
	long minY = Points[0].y, maxY = Points[0].y;
	for (const auto& p : Points) {
		minX = min(minX, p.x);
		maxX = max(maxX, p.x);
		minY = min(minY, p.y);
		maxY = max(maxY, p.y);
	}

	int destW = static_cast<int>(maxX - minX);
	int destH = static_cast<int>(maxY - minY);
	if (destW <= 0 || destH <= 0) {
		return;
	}

	// CImage::AlphaBlend 로 그리기 (회전 없음)
	//CImage& img = g_Sprites[m_Name];
	g_Sprites[m_Name].AlphaBlend(hDC,
		static_cast<int>(minX), static_cast<int>(minY), destW, destH,
		srcX, srcY, m_FrameWidth, m_FrameHeight, 255);
	/*img.StretchBlt(hDC,
		static_cast<int>(minX), static_cast<int>(minY), destW, destH,
		srcX, srcY, m_FrameWidth, m_FrameHeight );*/
}

