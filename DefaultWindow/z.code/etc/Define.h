#pragma once
#include "pch.h"

#define WINCX		500
#define WINCY		700

#define	PURE		= 0

#define VK_MAX		0xff

#define IDC_CHATINPUT 1001

extern HWND g_hWnd;
extern HWND g_Dlg;
extern HWND g_hEdit;
extern HINSTANCE hInst;

extern WSADATA wsaData;
extern SOCKADDR_IN servAdr;

extern bool bMafiaWin;
extern bool bMute;
extern float g_fVolume;


typedef struct tagInfo
{
	float fX;
	float fY;
	float fCX;
	float fCY;

}INFO;

template<typename T>
void Safe_Delete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

enum EAssetID
{
	// etc
	ASSET_ETC,

	// Scene Background
	ASSET_TITLE_BACKGROUND,
	ASSET_LOBBY_BACKGROUND,
	ASSET_INGAME_BACKGROUND_DAY,
	ASSET_INGAME_BACKGROUND_NIGHT,
	ASSET_ENDGAME_BACKGROUND,

	// Character Icon
	ASSET_NONE,
	ASSET_CITIZEN,
	ASSET_MAFIA,
	ASSET_POLICE,

	// Button
	ASSET_BUTTON_PLAY,
	ASSET_BUTTON_READY,
	ASSET_BUTTON_SOUND_ON,
	ASSET_BUTTON_SOUND_OFF,

	// Etc
	ASSET_PANNEL,
	ASSET_LEADERBOARD_ENTRY,
	ASSET_CHECK,
	ASSET_INPUT_BOX,
	ASSET_CROSSHAIR,
	ASSET_INVEST_MARK,
	ASSET_CROSS_MARK,
	ASSET_DEAD_MARK,
	ASSET_MAFIA_WIN,
	ASSET_CITIZEN_WIN,

	ASSET_END
};

enum EScene
{
	SCENE_TITLE,
	SCENE_LOBBY,
	SCENE_INGAME,
	SCENE_ENDGAME,

	SCENE_END
};

enum EButtonType
{
	BUTTON_PLAY,
	BUTTON_READY,
	BUTTON_SOUND,

	BUTTON_END
};

enum EObjectID
{
	PLAYER,
	LEADERBOARD_ENTRY,
	PANNEL,
	INPUT_BOX,
	BUTTON,
	TIME_DISPLAY,
	MOUSE,

	OBJ_END
};

enum EPannelState
{
	PANNEL_STATE_NONE,
	PANNEL_STATE_CITIZEN,
	PANNEL_STATE_MAFIA,
	PANNEL_STATE_POLICE,

	PANNEL_STATE_END
};

enum EPannelPickState
{
	PANNEL_PICK_NONE,
	PANNEL_PICK_VOTED,
	PANNEL_PICK_TARGETED,
	PANNEL_PICK_INVESTIGATED,

	PANNEL_PICK_END
};

enum EInGameState
{
	INGAME_STATE_DAY,
	INGAME_STATE_VOTE,
	INGAME_STATE_NIGHT,

	INGAME_STATE_END
};

enum EPlayerJob
{
	PLAYER_JOB_NONE,
	PLAYER_JOB_CITIZEN,
	PLAYER_JOB_MAFIA,
	PLAYER_JOB_POLICE,

	PLAYER_JOB_END
};

struct TChatMessage
{
	WCHAR Message[1024];
	WCHAR MessageOwner[1024];
	RECT ChatRect;
	bool bVisible = true;
};

struct PlayerInfo
{
	int PlayerIndex;
	WCHAR PlayerName[1024];
	EPlayerJob PlayerJob;
};

enum ESoundType
{
	SOUND_BGM,
	SOUND_EFFECT,
	
	SOUND_END
};

enum EBgmID
{
	BGM_TITLE,
	BGM_DAY,
	BGM_VOTE,
	BGM_NIGHT,

	BGM_END
};

enum ESoundEffectID
{
	SE_SCREAM,
	SE_SHOT,

	SE_END
};

// 음악 파일 경로
const std::wstring BgmDirections[BGM_END] =
{
	L"C:\\Users\\sarah\\Desktop\\API시연회\\Mafia_Client_Last\\x64\\Sound\\Bgm\\TitleTheme.wav",
	L"C:\\Users\\sarah\\Desktop\\API시연회\\Mafia_Client_Last\\x64\\Sound\\Bgm\\DayTheme.wav",
	L"C:\\Users\\sarah\\Desktop\\API시연회\\Mafia_Client_Last\\x64\\Sound\\Bgm\\VoteTheme.wav",
	L"C:\\Users\\sarah\\Desktop\\API시연회\\Mafia_Client_Last\\x64\\Sound\\Bgm\\NightTheme.wav"
};

const std::wstring TitleSound = L"C:\\Users\\sarah\\Desktop\\API시연회\\Mafia_Client_Last\\x64\\Sound\\Bgm\\TitleTheme.wav";
const std::wstring DaySound = L"C:\\Users\\sarah\\Desktop\\API시연회\\Mafia_Client_Last\\x64\\Sound\\Bgm\\DayTheme.wav";
const std::wstring VoteSound = L"C:\\Users\\sarah\\Desktop\\API시연회\\Mafia_Client_Last\\x64\\Sound\\Bgm\\VoteTheme.wav";
const std::wstring NightSound = L"C:\\Users\\sarah\\Desktop\\API시연회\\Mafia_Client_Last\\x64\\Sound\\Bgm\\NightTheme.wav";

const std::wstring SoundEffectDirections[SE_END] =
{
	L"C:\\Users\\sarah\\Desktop\\API시연회\\Mafia_Client_Last\\x64\\Sound\\SoundEffect\\Screaming.wav",
	L"C:\\Users\\sarah\\Desktop\\API시연회\\Mafia_Client_Last\\x64\\Sound\\SoundEffect\\PistolShot.wav"
};