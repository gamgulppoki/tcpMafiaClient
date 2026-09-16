#include "../etc/pch.h"
#include "LobbyScene.h"
#include "../Manager/ObjectManager.h"
#include "../Manager/AssetManager.h"
#include "../Object/CObj.h"
#include "../Object/CButton.h"
#include "../Object/CLeaderBoardEntry.h"
#include "../Object/CPlayer.h"
#include "../Manager/SceneManager.h"

LobbyScene::LobbyScene()
	: TotalClientNum(0)
{
}

LobbyScene::~LobbyScene()
{
}

void LobbyScene::Initialize()
{
	// ready 버튼 생성
	CObj* ReadyButton = new CButton(100, 80);
	dynamic_cast<CButton*>(ReadyButton)->SetButtonType(BUTTON_READY);
	ReadyButton->SetInfoXY(WINCX / 2, WINCY - 100);
	ObjectManager::Get_Instance()->Add_Object(BUTTON, ReadyButton);

	// LeaderBoard Margin
	float MarginX = 55;
	float MarginY = 10;

	// 10개의 LeaderBoard Entry 생성
	for (int i = 0; i < 10; i++)
	{
		CObj* newEntry = new CLeaderBoardEntry;
		newEntry->SetInfoXY(MarginX , (MarginY + (i * 50)) );

		// TODO: 나중에 Visible 설정
		//dynamic_cast<CLeaderBoardEntry*>(newEntry)->Set_Check_Visible(true);

		ObjectManager::Get_Instance()->Add_Object(LEADERBOARD_ENTRY, newEntry);
	}

	for (int i = 0; i < 10; i++)
	{
		LobbyReady[i] = false;
	}

	ObjectManager::Get_Instance()->Initialize();

	InitNetwork();
	//thread(&LobbyScene::InitNetwork, this).detach();

	SceneManager::Get_Instance()->SetSceneChanged(false);
}

void LobbyScene::Update()
{
	if (dynamic_cast<CPlayer*>(m_pPlayer)->bGameStarted && SceneManager::Get_Instance()->Get_CurretnSceneID() != SCENE_INGAME) return;
	if (SceneManager::Get_Instance()->Get_CurrentScene() == nullptr) return;

	ObjectManager::Get_Instance()->Update();

	RefreshUI();
}

void LobbyScene::Render(HDC hDC)
{
	if (dynamic_cast<CPlayer*>(m_pPlayer)->bGameStarted && SceneManager::Get_Instance()->Get_CurretnSceneID() != SCENE_INGAME) return;
	if (SceneManager::Get_Instance()->Get_CurrentScene() == nullptr) return;

	if (dynamic_cast<CPlayer*>(m_pPlayer)->bGameStarted)
	{
		return;
	}
	HDC     hBackDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_ETC);
	HDC     hBackGroundDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_LOBBY_BACKGROUND);

	BitBlt(hBackDC, 0, 0, WINCX, WINCY, hBackGroundDC, 0, 0, SRCCOPY);

	m_pPlayer->Render(hBackDC);
	ObjectManager::Get_Instance()->Render(hBackDC);

	BitBlt(hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

	if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_GameStarted())
	{
		SceneManager::Get_Instance()->SetSceneChanged(true);
		SceneManager::Get_Instance()->ChangeScene(SCENE_INGAME);
	}
}

void LobbyScene::Release()
{
	CPlayer* curPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
	for (int i = 0; i < TotalClientNum; i++)
	{
		PlayerInfo newInfo;
		newInfo.PlayerIndex = AllPlayerInfo[i].PlayerIndex;
		wcscpy_s(newInfo.PlayerName, sizeof(newInfo.PlayerName) / sizeof(WCHAR), AllPlayerInfo[i].PlayerName);

		curPlayer->AllClientInfo_Vec.push_back(newInfo);
	}
}

void LobbyScene::InitNetwork()
{
	CPlayer* tmpPlayer = dynamic_cast<CPlayer*>(ObjectManager::Get_Instance()->GetPlayer());
	ClientSocket tmpClientSock = tmpPlayer->Get_ClientSocket();
	int Buff = 0;

	// 여기서 접속 
	if (connect(tmpClientSock.Socket, (SOCKADDR*)&servAdr, sizeof(servAdr)) != SOCKET_ERROR)
	{
	}

	// 현재 플레이어의 index 정도는 가져와 줘야 댐 ...
	recv(tmpClientSock.Socket, (char*)&(tmpPlayer->PlayerIndex), sizeof(tmpPlayer->PlayerIndex), 0);

	// 접속 후 network를 담당하는 thread 만들어주기
	thread(&LobbyScene::ReceivePacket, this).detach();

	return;
}

void LobbyScene::LobbyNetwork()
{
	//thread(&LobbyScene::ReceivePacket, this);
	//while (1)
	//{
	//	if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_GameStarted())
	//	{
	//		SceneManager::Get_Instance()->ChangeScene(SCENE_INGAME);
	//		return;
	//	}

	//	SendPacket();
	//}	
}

void LobbyScene::ReceivePacket()
{
	while (1)
	{
		Sleep(10);

		CPlayer* curPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
		if (curPlayer->bGameStarted)
		{
			SceneManager::Get_Instance()->SetSceneChanged(true);
			SceneManager::Get_Instance()->ChangeScene(SCENE_INGAME);
			return;
		}

		recv(curPlayer->Get_ClientSocket().Socket, (char*)&(curPlayer->bGameStarted), sizeof(curPlayer->bGameStarted), 0);
		recv(curPlayer->Get_ClientSocket().Socket, (char*)&TotalClientNum, sizeof(TotalClientNum), 0);

		for (int i = 0; i < TotalClientNum; i++)
		{
			recv(curPlayer->Get_ClientSocket().Socket, (char*)&(AllPlayerInfo[i].PlayerName), sizeof(AllPlayerInfo[i].PlayerName), 0);
			recv(curPlayer->Get_ClientSocket().Socket, (char*)&(AllPlayerInfo[i].PlayerIndex), sizeof(AllPlayerInfo[i].PlayerIndex), 0);
			recv(curPlayer->Get_ClientSocket().Socket, (char*)&(LobbyReady[i]), sizeof(LobbyReady[i]), 0);
		}

		if (curPlayer->bGameStarted)
		{
			SceneManager::Get_Instance()->SetSceneChanged(true);
			SceneManager::Get_Instance()->ChangeScene(SCENE_INGAME);
			return;
		}
	}
}

void LobbyScene::UpdatePacket()
{
	if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_GameStarted())
	{
		SceneManager::Get_Instance()->SetSceneChanged(true);
		SceneManager::Get_Instance()->ChangeScene(SCENE_INGAME);
	}
	Sleep(100);
}

void LobbyScene::SendPacket()
{
	CPlayer* curPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
	bool curPlayerReady = LobbyReady[curPlayer->Get_PlayerIndex()];
	send(curPlayer->Get_ClientSocket().Socket, (char*)&curPlayerReady, sizeof(curPlayerReady), 0);
}

void LobbyScene::PressReady()
{
	SendPacket();
}

void LobbyScene::RefreshUI()
{
	CPlayer* curPlayer = dynamic_cast<CPlayer*>(ObjectManager::Get_Instance()->GetPlayer());
	auto LeaderBoardEntryList = ObjectManager::Get_Instance()->GetObjectList(LEADERBOARD_ENTRY);

	// 전부 일단 없애기
	for (auto board : *LeaderBoardEntryList)
	{
		dynamic_cast<CLeaderBoardEntry*>(board)->Set_Visible(false);
		dynamic_cast<CLeaderBoardEntry*>(board)->Set_Check_Visible(false);
	}

	// 현재 있는 부분들 갱신
	for (int i = 0; i < TotalClientNum; i++)
	{
		dynamic_cast<CLeaderBoardEntry*>((*LeaderBoardEntryList)[i])->Set_Visible(true);
		dynamic_cast<CLeaderBoardEntry*>((*LeaderBoardEntryList)[i])->Set_PlayerInfo(AllPlayerInfo[i]);

		if (LobbyReady[i] == true)
		{
			dynamic_cast<CLeaderBoardEntry*>((*LeaderBoardEntryList)[i])->Set_Check_Visible(true);
		}
	}
}