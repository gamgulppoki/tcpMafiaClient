#include "../etc/pch.h"
#include "CMainGame.h"
#include "../Manager/AssetManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ObjectManager.h"
#include "../Object/CMouse.h"
#include "../Manager/KeyManager.h"
#include "../Network/NetworkDefine.h"
#include "../Manager/NetworkManager.h"
#include "../Manager/CSoundMgr.h"
#include "../Object/CButton.h"

CMainGame::CMainGame() : m_pPlayer(nullptr)
{
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	if (!m_pPlayer)
	{
		m_pPlayer = new CPlayer;
		m_pPlayer->Initialize();
	}
	ObjectManager::Get_Instance()->SetPlayer(m_pPlayer);

	AssetManager::Get_Instance()->LoadAssets();
	CSoundMgr::Get_Instance()->LoadAllSound();

	CObj* newMouse = new CMouse;
	ObjectManager::Get_Instance()->Add_Object(MOUSE, newMouse);

	CObj* newSoundButton = new CButton(20, 20);
	static_cast<CButton*>(newSoundButton)->SetButtonType(BUTTON_SOUND);
	static_cast<CButton*>(newSoundButton)->SetPos(20, 670);
	ObjectManager::Get_Instance()->Add_Object(BUTTON, newSoundButton);

	SceneManager::Get_Instance()->Initialize(m_pPlayer);
}

void CMainGame::Update()
{
	if (dynamic_cast<CPlayer*>(m_pPlayer)->bGameStarted && SceneManager::Get_Instance()->Get_CurretnSceneID() != SCENE_INGAME) return;
	if (SceneManager::Get_Instance()->Get_CurrentScene() == nullptr) return;
	m_pPlayer->Update();
	SceneManager::Get_Instance()->Update();
	KeyManager::Get_Instance()->Update();
}

void CMainGame::Render()
{
	if (dynamic_cast<CPlayer*>(m_pPlayer)->bGameStarted && SceneManager::Get_Instance()->Get_CurretnSceneID() != SCENE_INGAME) return;
	if (SceneManager::Get_Instance()->Get_CurrentScene() == nullptr) return;
	SceneManager::Get_Instance()->Render(m_hDC);
}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);
	
	AssetManager::Get_Instance()->Destroy_Instance();
	SceneManager::Get_Instance()->Destroy_Instance();
	ObjectManager::Get_Instance()->Destroy_Instance();
	KeyManager::Get_Instance()->Destroy_Instance();
	CSoundMgr::Get_Instance()->Destroy_Instance();

	Safe_Delete<CObj*>(m_pPlayer);
}
