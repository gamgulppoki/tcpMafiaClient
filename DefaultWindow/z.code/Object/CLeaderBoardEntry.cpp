#include "../etc/pch.h"
#include "CLeaderBoardEntry.h"
#include "../Manager/AssetManager.h"
#include "CPlayer.h"

CLeaderBoardEntry::CLeaderBoardEntry()
	: bCheckVisible(false), bVisible(false)
{
}

CLeaderBoardEntry::CLeaderBoardEntry(float fx, float fy)
	: bCheckVisible(false)
{
	m_tInfo.fX = fx;
	m_tInfo.fY = fy;
}

CLeaderBoardEntry::~CLeaderBoardEntry()
{
	Release();
}

void CLeaderBoardEntry::Initialize()
{
	m_tInfo.fCX = 392;
	m_tInfo.fCY = 80;

	Update_Rect();

	CheckRect.top = m_tRect.top + 5;
	CheckRect.bottom = m_tRect.bottom + 5;
	CheckRect.left = m_tRect.left + 340;
	CheckRect.right = m_tRect.left + 380;

	NameRect.top = m_tRect.top + 20;
	NameRect.bottom = m_tRect.bottom + 20;
	NameRect.left = m_tRect.left+30;
	NameRect.right = m_tRect.right+30;

	EntryHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_LEADERBOARD_ENTRY);
	CheckHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_CHECK);
}

void CLeaderBoardEntry::Update()
{
	Update_Rect();
}

void CLeaderBoardEntry::Render(HDC hDC)
{
	if (!bVisible) return;

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		EntryHDC,
		0,
		0,
		(int)m_tInfo.fCX,   // 복사할 이미지 가로
		(int)m_tInfo.fCY,   // 복사할 이미지 세로
		RGB(255, 255, 255));    // 제거할 픽셀 색상 값

	SetTextColor(hDC, RGB(255, 255, 255));    // 글자 색을 흰색으로
	SetBkMode(hDC, TRANSPARENT);              // 배경 투명
	DrawText(hDC, (m_PlayerInfo.PlayerName), -1, &NameRect, DT_VCENTER | DT_WORDBREAK);

	if (bCheckVisible)
	{
		GdiTransparentBlt(hDC,
			CheckRect.left,
			CheckRect.top,
			40,
			40,
			CheckHDC,
			0,
			0,
			40,   // 복사할 이미지 가로
			40,   // 복사할 이미지 세로
			RGB(255, 255, 255));    // 제거할 픽셀 색상 값
	}
}

void CLeaderBoardEntry::Release()
{
}

void CLeaderBoardEntry::Update_Rect()
{
	m_tRect.top = m_tInfo.fY;
	m_tRect.left = m_tInfo.fX;
	m_tRect.right = m_tInfo.fX + m_tInfo.fCX;
	m_tRect.bottom = m_tInfo.fY + m_tInfo.fCY;
}
