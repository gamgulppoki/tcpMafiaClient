#include "../etc/pch.h"
#include "CButton.h"
#include "../Manager/AssetManager.h"
#include "../Manager/SceneManager.h"
#include "CLeaderBoardEntry.h"
#include "../Manager/ObjectManager.h"
#include "CPlayer.h"
#include "../Object/CLeaderBoardEntry.h"
#include "../Scene/LobbyScene.h"
#include "../Manager/CSoundMgr.h"

CButton::CButton()
	: ButtonType(BUTTON_END), ButtonAssetID(ASSET_END), m_iDrawID(0)
{
}

CButton::CButton(float fcx, float fcy)
	: ButtonType(BUTTON_END), ButtonAssetID(ASSET_END), m_iDrawID(0)
{
	m_tInfo.fCX = fcx;
	m_tInfo.fCY = fcy;
}

CButton::~CButton()
{
	Release();
}

void CButton::Initialize()
{
	if (ButtonType == BUTTON_PLAY)
	{
		ButtonAssetID = ASSET_BUTTON_PLAY;
		GetButtonAsset();
	}
	else if (ButtonType == BUTTON_READY)
	{
		ButtonAssetID = ASSET_BUTTON_READY;
		GetButtonAsset();
	}
	else if (ButtonType == BUTTON_SOUND)
	{
		SoundButtonOnHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_BUTTON_SOUND_ON);
		SoundButtonOffHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_BUTTON_SOUND_OFF);
	}
}

void CButton::Update()
{
	__super::Update_Rect();

	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		m_iDrawID = 1;
	}
	else
		m_iDrawID = 0;
}

void CButton::Render(HDC hDC)
{
	if (ButtonType == BUTTON_SOUND)
	{
		if (CSoundMgr::Get_Instance()->bSoundOn)
		{
			GdiTransparentBlt(hDC,
				m_tRect.left,
				m_tRect.top,
				(int)m_tInfo.fCX,
				(int)m_tInfo.fCY,
				SoundButtonOnHDC,
				0,
				0,
				(int)m_tInfo.fCX,   // 복사할 이미지 가로
				(int)m_tInfo.fCY,   // 복사할 이미지 세로
				RGB(255, 255, 255));    // 제거할 픽셀 색상 값
		}
		else if (!CSoundMgr::Get_Instance()->bSoundOn)
		{
			GdiTransparentBlt(hDC,
				m_tRect.left,
				m_tRect.top,
				(int)m_tInfo.fCX,
				(int)m_tInfo.fCY,
				SoundButtonOffHDC,
				0,
				0,
				(int)m_tInfo.fCX,   // 복사할 이미지 가로
				(int)m_tInfo.fCY,   // 복사할 이미지 세로
				RGB(255, 255, 255));    // 제거할 픽셀 색상 값
		}
	}
	else
	{
		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			ButtonHDC,
			m_iDrawID * (int)m_tInfo.fCX,
			0,
			(int)m_tInfo.fCX,   // 복사할 이미지 가로
			(int)m_tInfo.fCY,   // 복사할 이미지 세로
			RGB(255, 255, 255));    // 제거할 픽셀 색상 값
	}
}

void CButton::Release()
{
}

void CButton::OnClicked()
{
	if (ButtonType == BUTTON_PLAY)
	{
		// scene 변환
		SceneManager::Get_Instance()->ChangeScene(SCENE_LOBBY);
	}
	else if (ButtonType == BUTTON_READY)
	{
		LobbyScene* curLobby = dynamic_cast<LobbyScene*>(SceneManager::Get_Instance()->Get_CurrentScene());
		//curLobby->Send_LobbyPacket();
		curLobby->PressReady();
	}
	else if (ButtonType == BUTTON_SOUND)
	{
		CSoundMgr::Get_Instance()->bSoundOn = !CSoundMgr::Get_Instance()->bSoundOn;
		if (CSoundMgr::Get_Instance()->bSoundOn)
		{
			CSoundMgr::Get_Instance()->PlayCurBgm();
		}
		else if (!CSoundMgr::Get_Instance()->bSoundOn)
		{
			CSoundMgr::Get_Instance()->StopCurBgm();
		}
	}
}

void CButton::GetButtonAsset()
{
	ButtonHDC = AssetManager::Get_Instance()->Find_AssetHDC(ButtonAssetID);
}

void CButton::SetPos(float x, float y)
{
	m_tInfo.fX = x;
	m_tInfo.fY = y;
}
