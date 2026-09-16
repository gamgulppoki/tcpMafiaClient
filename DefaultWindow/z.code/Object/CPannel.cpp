#include "../etc/pch.h"
#include "CPannel.h"
#include "../Manager/AssetManager.h"

CPannel::CPannel()
	: PannelState(PANNEL_STATE_NONE), PannelPickState(PANNEL_PICK_NONE), IconSize(50.f), bVisible(false), bMyPannel(false), bDead(false)
{
	m_tInfo.fCX = 80.f;
	m_tInfo.fCY = 80.f;
}

CPannel::~CPannel()
{
	Release();
}

void CPannel::Initialize()
{
	PannelHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_PANNEL);

	NoneIconHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_NONE);
	CitizenIconHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_CITIZEN);
	MafiaIconHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_MAFIA);
	PoliceIconHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_POLICE);

	CrossHairHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_CROSSHAIR);
	DeadMarkHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_DEAD_MARK);
	CrossMarkHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_CROSS_MARK);
	InvestMarkHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_INVEST_MARK);

	Update_Rect();

	NameRect.top = m_tRect.top - 15;
	NameRect.bottom = m_tRect.bottom - 15;
	NameRect.left = m_tRect.left;
	NameRect.right = m_tRect.right;

	IconTop = m_tInfo.fY + 15;
	IconLeft = m_tInfo.fX + 15;

	// TODO: 나중에 삭제
}

void CPannel::Update()
{
}

void CPannel::Render(HDC hDC)
{
	if (!bVisible) return;

	// 닉네임 렌더링
	SetTextColor(hDC, RGB(255, 255, 255));    // 글자 색을 흰색으로
	SetBkMode(hDC, TRANSPARENT);              // 배경 투명
	DrawText(hDC, (m_pPlayer->Get_Player_Name()), -1, &NameRect, DT_VCENTER | DT_WORDBREAK);

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		PannelHDC,
		0,
		0,
		(int)m_tInfo.fCX,   // 복사할 이미지 가로
		(int)m_tInfo.fCY,   // 복사할 이미지 세로
		RGB(255, 255, 255));    // 제거할 픽셀 색상 값

	Render_Pannel_State(hDC);

	if (bDead)
	{
		GdiTransparentBlt(hDC,
			IconLeft,
			IconTop,
			(int)IconSize,
			(int)IconSize,
			DeadMarkHDC,
			0,
			0,
			(int)IconSize,  // 복사할 이미지 가로
			(int)IconSize,  // 복사할 이미지 세로
			RGB(255, 255, 255));    // 제거할 픽셀 색상 값

		return;
	}

	Render_Pannel_PickState(hDC);

}

void CPannel::Release()
{
}

void CPannel::Render_Pannel_State(HDC hDC)
{
	if (PannelState == PANNEL_STATE_CITIZEN)
	{
		GdiTransparentBlt(hDC,
			IconLeft,
			IconTop,
			(int)IconSize,
			(int)IconSize,
			CitizenIconHDC,
			0,
			0,
			(int)IconSize,   // 복사할 이미지 가로
			(int)IconSize,   // 복사할 이미지 세로
			RGB(255, 255, 255));    // 제거할 픽셀 색상 값
	}
	else if (PannelState == PANNEL_STATE_MAFIA)
	{
		GdiTransparentBlt(hDC,
			IconLeft,
			IconTop,
			(int)IconSize,
			(int)IconSize,
			MafiaIconHDC,
			0,
			0,
			(int)IconSize,   // 복사할 이미지 가로
			(int)IconSize,   // 복사할 이미지 세로
			RGB(255, 255, 255));    // 제거할 픽셀 색상 값
	}
	else if (PannelState == PANNEL_STATE_POLICE)
	{
		GdiTransparentBlt(hDC,
			IconLeft,
			IconTop,
			(int)IconSize,
			(int)IconSize,
			PoliceIconHDC,
			0,
			0,
			(int)IconSize,  // 복사할 이미지 가로
			(int)IconSize,  // 복사할 이미지 세로
			RGB(255, 255, 255));    // 제거할 픽셀 색상 값
	}
	else if (PannelState == PANNEL_STATE_NONE)
	{
		GdiTransparentBlt(hDC,
			IconLeft,
			IconTop,
			(int)IconSize,
			(int)IconSize,
			NoneIconHDC,
			0,
			0,
			(int)IconSize,  // 복사할 이미지 가로
			(int)IconSize,  // 복사할 이미지 세로
			RGB(255, 255, 255));    // 제거할 픽셀 색상 값
	}
}

void CPannel::Render_Pannel_PickState(HDC hDC)
{
	if (PannelPickState == PANNEL_PICK_VOTED)
	{
		GdiTransparentBlt(hDC,
			IconLeft,
			IconTop,
			(int)IconSize,
			(int)IconSize,
			CrossMarkHDC,
			0,
			0,
			(int)IconSize,  // 복사할 이미지 가로
			(int)IconSize,  // 복사할 이미지 세로
			RGB(255, 255, 255));    // 제거할 픽셀 색상 값
	}
	else if (PannelPickState == PANNEL_PICK_TARGETED)
	{
		GdiTransparentBlt(hDC,
			IconLeft,
			IconTop,
			(int)IconSize,
			(int)IconSize,
			CrossHairHDC,
			0,
			0,
			(int)IconSize,  // 복사할 이미지 가로
			(int)IconSize,  // 복사할 이미지 세로
			RGB(255, 255, 255));    // 제거할 픽셀 색상 값
	}
	else if (PannelPickState == PANNEL_PICK_INVESTIGATED)
	{
		GdiTransparentBlt(hDC,
			IconLeft,
			IconTop,
			(int)IconSize,
			(int)IconSize,
			InvestMarkHDC,
			0,
			0,
			(int)IconSize,  // 복사할 이미지 가로
			(int)IconSize,  // 복사할 이미지 세로
			RGB(255, 255, 255));    // 제거할 픽셀 색상 값
	}
}

void CPannel::OnClicked()
{
	if (bMyPannel) return;
	
	PannelState = (EPannelState)((PannelState + 1) % (PANNEL_STATE_END));
}

void CPannel::OnVoted()
{
	//PannelPickState = PANNEL_PICK_VOTED;
}

void CPannel::OnTargeted()
{
	//PannelPickState = PANNEL_PICK_TARGETED;
}

void CPannel::OnInvestigated()
{
	//PannelPickState = PANNEL_PICK_INVESTIGATED;
}

void CPannel::Set_Pos(float x, float y)
{
	m_tInfo.fX = x;
	m_tInfo.fY = y;
}

void CPannel::Set_MyPannelState(EPlayerJob playerJob)
{
	if (playerJob == PLAYER_JOB_CITIZEN)
	{
		PannelState = PANNEL_STATE_CITIZEN;
	}
	else if (playerJob == PLAYER_JOB_MAFIA)
	{
		PannelState = PANNEL_STATE_MAFIA;
	}
	else if (playerJob == PLAYER_JOB_POLICE)
	{
		PannelState = PANNEL_STATE_POLICE;
	}
	else
	{
		PannelState = PANNEL_STATE_NONE;
	}
}

void CPannel::Update_Rect()
{
	m_tRect.top = m_tInfo.fY;
	m_tRect.bottom = m_tInfo.fY + m_tInfo.fCY;
	m_tRect.left = m_tInfo.fX;
	m_tRect.right = m_tInfo.fX + m_tInfo.fCX;
}
