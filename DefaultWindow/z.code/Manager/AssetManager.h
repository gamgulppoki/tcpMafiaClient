#pragma once
#include "../etc/pch.h"
#include "../etc/Define.h"

class CAsset;

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	void Release();

public:
	void LoadAssets();
	HDC Find_AssetHDC(EAssetID assetID);

public:
	static AssetManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new AssetManager;
		}
		return Instance;
	}

	void	Destroy_Instance()
	{
		if (Instance)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

private:
	static AssetManager* Instance;

	unordered_map<EAssetID, const TCHAR*> AssetDirection =
	{
		// Background
		{ASSET_ETC, L"../Assets/BackGround/EtcBackground.bmp"},
		{ASSET_TITLE_BACKGROUND, L"../Assets/BackGround/TitleBackground.bmp"},
		{ASSET_LOBBY_BACKGROUND, L"../Assets/BackGround/LobbyBackground.bmp"},
		{ASSET_INGAME_BACKGROUND_DAY, L"../Assets/BackGround/InGameBackground_Day.bmp"},
		{ASSET_INGAME_BACKGROUND_NIGHT, L"../Assets/BackGround/InGameBackground_Night.bmp"},

		// Icon
		{ASSET_NONE, L"../Assets/Icon/None_Icon.bmp"},
		{ASSET_CITIZEN, L"../Assets/Icon/Citizen_Icon.bmp"},
		{ASSET_MAFIA, L"../Assets/Icon/Mafia_Icon.bmp"},
		{ASSET_POLICE, L"../Assets/Icon/Police_Icon.bmp"},

		// Button
		{ASSET_BUTTON_PLAY, L"../Assets/Button/PlayButton.bmp"},
		{ASSET_BUTTON_READY, L"../Assets/Button/ReadyButton.bmp"},
		{ASSET_BUTTON_SOUND_ON, L"../Assets/Button/SoundButtonOn.bmp"},
		{ASSET_BUTTON_SOUND_OFF, L"../Assets/Button/SoundButtonOff.bmp"},

		// Etc
		{ASSET_LEADERBOARD_ENTRY, L"../Assets/Etc/LeaderBoard_Entry.bmp"},
		{ASSET_PANNEL, L"../Assets/Etc/Pannel.bmp"},
		{ASSET_CHECK, L"../Assets/Etc/Check.bmp"},
		{ASSET_INPUT_BOX, L"../Assets/Etc/InputBox.bmp"},
		{ASSET_CROSSHAIR, L"../Assets/Etc/CrossHair.bmp"},
		{ASSET_INVEST_MARK, L"../Assets/Etc/InvestMark.bmp"},
		{ASSET_CROSS_MARK, L"../Assets/Etc/CrossMark.bmp"},
		{ASSET_DEAD_MARK, L"../Assets/Etc/DeadMark.bmp"},
		{ASSET_MAFIA_WIN, L"../Assets/Etc/MafiaWin.bmp"},
		{ASSET_CITIZEN_WIN, L"../Assets/Etc/CitizenWin.bmp"},
	};

	unordered_map<EAssetID, CAsset*> LoadedAssets;
};

