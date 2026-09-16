#pragma once

#include "../etc/Define.h"

class CScene;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Initialize(CObj* player);
	void Update();
	void Render(HDC hDC);
	void Release();

public:
	CScene* Get_CurrentScene() { return CurrentScene; }
	EScene Get_CurretnSceneID() { return CurrentSceneID; }

	void ChangeScene(EScene newSceneID);
	bool IsInGameScene() { return (CurrentSceneID == SCENE_INGAME); }

	void SetSceneChanged(bool boolean) { bSceneChanged = boolean; }
	bool GetSceneChanged() { return bSceneChanged; }

public:
	static SceneManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new SceneManager;
		}
		
		return Instance;
	}

	void Destroy_Instance()
	{
		if (CurrentScene)
		{
			delete CurrentScene;
			CurrentScene = nullptr;
		}
		if (PreScene)
		{
			delete PreScene;
			PreScene = nullptr;
		}
		if (Instance)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

private:
	static SceneManager* Instance;

	CScene* CurrentScene;
	CScene* PreScene;
	EScene  CurrentSceneID;
	bool bSceneChanged = false;

	CObj* m_pPlayer;
};

