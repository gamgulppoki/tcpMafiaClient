#include "../etc/pch.h"
#include "../Object/CObj.h"
#include "../Scene/TitleScene.h"
#include "../Scene/LobbyScene.h"
#include "../Scene/InGameScene.h"
#include "../Scene/EndScene.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "CSoundMgr.h"

SceneManager* SceneManager::Instance = nullptr;

SceneManager::SceneManager()
	: PreScene(nullptr), CurrentScene(nullptr)
{

}

SceneManager::~SceneManager()
{
	Release();
}

void SceneManager::Initialize(CObj* player)
{
	bSceneChanged = false;
	CurrentSceneID = SCENE_TITLE;
	CurrentScene = new TitleScene;
	dynamic_cast<TitleScene*>(CurrentScene)->SetPlayer(player);
	CurrentScene->Initialize();
	m_pPlayer = player;
}

void SceneManager::Update()
{
	if (CurrentScene == nullptr) return;
	if (bSceneChanged) return;
	CurrentScene->Update();
}

void SceneManager::Render(HDC hDC)
{
	if (CurrentScene == nullptr) return;
	if (bSceneChanged) return;
	CurrentScene->Render(hDC);
}

void SceneManager::Release()
{
	delete CurrentScene;
	CurrentScene = nullptr;
}

void SceneManager::ChangeScene(EScene newSceneID)
{
	if (CurrentSceneID == newSceneID) return;

	//bSceneChanged = true;

	//if (PreScene)
	//{
	//	delete PreScene;
	//	PreScene = nullptr;
	//}

	PreScene = CurrentScene;

	ObjectManager::Get_Instance()->Delete_All_Except_Player();
	CurrentSceneID = newSceneID;

	if (newSceneID == SCENE_TITLE)
	{
		CurrentScene = new TitleScene;
	}
	else if (newSceneID == SCENE_LOBBY)
	{
		CurrentScene = new LobbyScene;
	}
	else if (newSceneID == SCENE_INGAME)
	{
		CSoundMgr::Get_Instance()->StopBgm(BGM_TITLE);
		CurrentScene = new InGameScene;
	}
	else if (newSceneID == SCENE_ENDGAME)
	{
		CurrentScene = new EndScene;
	}
	if (m_pPlayer)
	{
		CurrentScene->SetPlayer(m_pPlayer);
	}
	CurrentScene->Initialize();

	if (PreScene)
	{
		delete PreScene;
		PreScene = nullptr;
	}

}
