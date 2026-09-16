#include "../etc/pch.h"
#include "../etc/Define.h"
#include "InGameScene.h"
#include "../Manager/AssetManager.h"
#include "../Manager/ObjectManager.h"
#include "../Manager/KeyManager.h"
#include "../Object/CPlayer.h"
#include "../Object/CPannel.h"
#include "../Manager/SceneManager.h"
#include "../Network/NetworkDefine.h"
#include "../Manager/CSoundMgr.h"
#include "../Object/TimeDisplay.h"

InGameScene::InGameScene()
	: InGameState(INGAME_STATE_DAY)
	, bCanVote(false), bCanInvest(false), bCanKill(false)
{
}

InGameScene::~InGameScene()
{
	Release();
}

void InGameScene::Initialize()
{
	Player = dynamic_cast<CPlayer*>(m_pPlayer);

	// network
	GetInitPacket();

	// client
	hEdit = CreateWindowEx(
		WS_EX_CLIENTEDGE, // 컨트롤의 테두리 스타일
		TEXT("EDIT"),     // Edit Control의 미리 정의된 클래스 이름
		TEXT(""),         // 초기 텍스트 (빈 문자열)
		WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, // 윈도우 스타일
		10, 200, 380, 25, // 위치 (x, y) 및 크기 (width, height) - 예시 좌표
		g_hWnd,             // 부모 윈도우 핸들
		(HMENU)IDC_CHATINPUT, // 컨트롤 ID (고유한 식별자)
		NULL, // 인스턴스 핸들
		NULL              // 추가 데이터
	);
	
	int offsetx = 10.f;
	int offsety = 20.f;
	// pannel 10개 생성
	for (int i = 0; i < 10; i++)
	{
		CObj* newPannel = new CPannel;

		float x = (i % 5) * 100 + offsetx;
		float y = (i / 5) * 100 + offsety;
		dynamic_cast<CPannel*>(newPannel)->Set_Pos(x, y);
		dynamic_cast<CPannel*>(newPannel)->Set_Visible(false);

		ObjectManager::Get_Instance()->Add_Object(PANNEL, newPannel);
	}

	auto PannelList = ObjectManager::Get_Instance()->GetObjectList(PANNEL);
	for (int i = 0; i < Player->AllClientInfo_Vec.size(); i++)
	{
		CPannel* tmpPannel = dynamic_cast<CPannel*>((*PannelList)[i]);
		tmpPannel->Set_Visible(true);

		if (Player->Get_PlayerIndex() == i)
		{
			tmpPannel->Set_Player(Player);
			tmpPannel->Set_MyPannel(true);
			tmpPannel->Set_MyPannelState(Player->Get_PlayerJob());
		}
		else
		{
			// 플레이어 생성
			CPlayer* newPlayer = new CPlayer;
			newPlayer->Set_PlayerName(Player->AllClientInfo_Vec[i].PlayerName);
			newPlayer->Set_PlayerIndex(Player->AllClientInfo_Vec[i].PlayerIndex);
			newPlayer->Set_PlayerDead(false);

			tmpPannel->Set_Player(newPlayer);
		}
	}

	// Time Display 생성
	CObj* newTimeDisplay = new TimeDisplay;
	static_cast<TimeDisplay*>(newTimeDisplay)->SetInfoXY(480, 260);
	ObjectManager::Get_Instance()->Add_Object(TIME_DISPLAY,	newTimeDisplay);
	pTime = static_cast<TimeDisplay*>(newTimeDisplay);

	ObjectManager::Get_Instance()->Initialize();
	SceneManager::Get_Instance()->SetSceneChanged(false);
	CSoundMgr::Get_Instance()->PlayBgm(BGM_DAY);
	
	thread(&InGameScene::ReceivePacket, this).detach();
}

void InGameScene::Update()
{
	SetWindowText(g_hWnd, to_wstring(RemainTime).c_str() );

	if (InGameState == INGAME_STATE_DAY)
	{
		UpdateInGameDay();
	}
	else if (InGameState == INGAME_STATE_VOTE)
	{
		UpdateInGameVote();
	}
	else if (InGameState == INGAME_STATE_NIGHT)
	{
		UpdateInGameNight();
	}

	// 전하고 다르다면 멘트 업데이트 (밤이 되었습니다 등등..)
	if (PreInGameState != InGameState)
	{
		UpdateInGameStateChanged();
	}

	auto AllPannelList = ObjectManager::Get_Instance()->GetObjectList(PANNEL);
	for (int i = 0; i < 10; i++)
	{
		dynamic_cast<CPannel*>((*AllPannelList)[i])->Set_PannelPickState(PANNEL_PICK_NONE);
	}

	if (VotedPannel != -1)
	{
		auto PannelList = ObjectManager::Get_Instance()->GetObjectList(PANNEL);
		dynamic_cast<CPannel*>((*PannelList)[VotedPannel])->Set_PannelPickState(PANNEL_PICK_VOTED);
	}


	if (TargetedPannel != -1)
	{
		auto PannelList = ObjectManager::Get_Instance()->GetObjectList(PANNEL);
		if (Player->Get_PlayerJob() == PLAYER_JOB_POLICE)
		{
			dynamic_cast<CPannel*>((*PannelList)[PoliceTarget])->Set_PannelPickState(PANNEL_PICK_INVESTIGATED);
		}
		else if (Player->Get_PlayerJob() == PLAYER_JOB_MAFIA)
		{
			dynamic_cast<CPannel*>((*PannelList)[MafiaTargetIndex])->Set_PannelPickState(PANNEL_PICK_TARGETED);
		}
	}

	if (KilledPlayerIndex != -1)
	{
		auto PannelList = ObjectManager::Get_Instance()->GetObjectList(PANNEL);
		dynamic_cast<CPannel*>((*PannelList)[KilledPlayerIndex])->Set_PannelDead(true);
		KilledPlayerIndex = -1;
	}

	PreInGameState = InGameState;
}

void InGameScene::Render(HDC hDC)
{
	if (InGameState == INGAME_STATE_DAY)
	{
		RenderInGameDay(hDC);
	}
	else if (InGameState == INGAME_STATE_VOTE)
	{
		RenderInGameVote(hDC);
	}
	else if (InGameState == INGAME_STATE_NIGHT)
	{
		RenderInGameNight(hDC);
	}

	if (bPreGameEnd)
	{
		SceneManager::Get_Instance()->SetSceneChanged(false);
		Player->bGameStarted = false;
		SceneManager::Get_Instance()->ChangeScene(SCENE_ENDGAME);
	}
}

void InGameScene::Release()
{
	closesocket(Player->Get_ClientSocket().Socket);
	WSACleanup();
}

void InGameScene::RefreshUI()
{
}

void InGameScene::PushMessages(TChatMessage newTChatMessage)
{
	newTChatMessage.ChatRect.top = 650;
	newTChatMessage.ChatRect.bottom = 700;
	newTChatMessage.ChatRect.left = 20;
	newTChatMessage.ChatRect.right = 480;

	lock_guard<mutex> lock(MutexMessage);
	Messages.push_back(newTChatMessage);
}

void InGameScene::PushSystemMessages(WCHAR* message)
{
	TChatMessage newMessage;
	newMessage.ChatRect.top = 650;
	newMessage.ChatRect.bottom = 700;
	newMessage.ChatRect.left = 20;
	newMessage.ChatRect.right = 480;
	wcscpy_s(newMessage.MessageOwner, sizeof(newMessage.MessageOwner) / sizeof(WCHAR), L"시스템");
	wcscpy_s(newMessage.Message, sizeof(newMessage.Message) / sizeof(WCHAR), message);

	lock_guard<mutex> lock(MutexMessage);

	Messages.push_back(newMessage);

	// 원래있던 메세지들 위로 조금씩 올려주기
	if (!Messages.empty())
	{
		for (auto& message : Messages)
		{
			message.ChatRect.top -= 40;
			message.ChatRect.bottom -= 40;

			if (message.bVisible && message.ChatRect.top <= 200)
			{
				message.bVisible = false;
			}
		}
	}
}

void InGameScene::GetInitPacket                                                                                                                      ()
{
	EPlayerJob tmpPlayerJobBuffer = PLAYER_JOB_MAFIA;

	recv(Player->Get_ClientSocket().Socket, (char*)(&tmpPlayerJobBuffer), sizeof(tmpPlayerJobBuffer), 0);

	Player->Set_PlayerJob(tmpPlayerJobBuffer);
	EPlayerJob tmpPlayerJob = Player->Get_PlayerJob();
	if (tmpPlayerJob == PLAYER_JOB_MAFIA)
	{
		WCHAR SystemMessage[1024] = L"당신의 직업은 마피아 입니다.";
		PushSystemMessages(SystemMessage);
	}
	else if (tmpPlayerJob == PLAYER_JOB_POLICE)
	{
		WCHAR SystemMessage[1024] = L"당신의 직업은 경찰 입니다";
		PushSystemMessages(SystemMessage);
	}
	else if (tmpPlayerJob == PLAYER_JOB_CITIZEN)
	{
		WCHAR SystemMessage[1024] = L"당신의 직업은 시민 입니다";
		PushSystemMessages(SystemMessage);
	}
	else
	{
		WCHAR SystemMessage[1024] = L"시스템 오류. 잘못된 직업 입니다";
		PushSystemMessages(SystemMessage);
	}

	recv(Player->Get_ClientSocket().Socket, (char*)&PlayerNum, sizeof(PlayerNum), 0);

	for (int i = 0; i < PlayerNum; i++)
	{
		PlayerInfo newPlayerInfo;
		recv(Player->Get_ClientSocket().Socket, (char*)&(newPlayerInfo.PlayerIndex), sizeof(newPlayerInfo.PlayerIndex), 0);
		recv(Player->Get_ClientSocket().Socket, (char*)&(newPlayerInfo.PlayerName), sizeof(newPlayerInfo.PlayerName), 0);
		Player->AllClientInfo_Vec.push_back(newPlayerInfo);
	}
}

void InGameScene::ReceivePacket()
{
	SOCKET mySocket = Player->Get_ClientSocket().Socket;

	while (1)
	{
		Sleep(100);

		if (bPreGameEnd) return;
		recv(mySocket, (char*)&bPreGameEnd, sizeof(bPreGameEnd), 0);
		recv(mySocket, (char*)&bMafiaWin, sizeof(bMafiaWin), 0);
		if (bPreGameEnd) return;

		// 메세지 받기 유무 받기
		bool bnewMessage = false;
		recv(mySocket, (char*)&bnewMessage, sizeof(bnewMessage), 0);

		if (bnewMessage)
		{
			// 메세지 받기
			TChatMessage newMessage;
			recv(mySocket, (char*)(newMessage.MessageOwner), sizeof(newMessage.MessageOwner), 0);
			recv(mySocket, (char*)(newMessage.Message), sizeof(newMessage.Message), 0);

			PushMessages(newMessage);

			// 원래있던 메세지들 위로 조금씩 올려주기
			lock_guard<mutex> lock(MutexMessage);

			if (!Messages.empty())
			{
				for (auto& message : Messages)
				{
					message.ChatRect.top -= 40;
					message.ChatRect.bottom -= 40;

					if (message.bVisible && message.ChatRect.top <= 200)
					{
						message.bVisible = false;
					}
				}
			}
		}

		// 현재 게임 상태와 시간 받기
		recv(mySocket, (char*)&InGameState, sizeof(InGameState), 0);
		recv(mySocket, (char*)&RemainTime, sizeof(RemainTime), 0);
		pTime->SetRemainTime(RemainTime);

		// 게임 상태가 바뀌었을 때
		bool bGameStateChanged;
		recv(mySocket, (char*)&bGameStateChanged, sizeof(bGameStateChanged), 0);

		if (bGameStateChanged)
		{
			WCHAR Blank[1024] = L"";
			PushSystemMessages(Blank);

			if (InGameState == INGAME_STATE_DAY)
			{
				WCHAR SystemMessage[1024];
				recv(mySocket, (char*)&SystemMessage, sizeof(SystemMessage), 0);
				PushSystemMessages(SystemMessage);
				recv(mySocket, (char*)&KilledPlayerIndex, sizeof(KilledPlayerIndex), 0);
				recv(mySocket, (char*)&PoliceTarget, sizeof(PoliceTarget), 0);
				recv(mySocket, (char*)&PoliceTargetIsMafia, sizeof(PoliceTargetIsMafia), 0);
				// 경찰에게 마피아인지 아닌지 띄워주고, 마피아 타겟은 죽이는거 해줘야 함 
				if (Player->Get_PlayerJob() == PLAYER_JOB_POLICE)
				{
					if (PoliceTargetIsMafia)
					{
						WCHAR ResultMessage[1024];
						wstring ResultPlayerName = L"Client" + to_wstring(PoliceTarget) + L" (이)는 마피아 입니다.";
						wcscpy_s(ResultMessage, sizeof(ResultMessage) / sizeof(WCHAR), ResultPlayerName.c_str());
						PushSystemMessages(ResultMessage);
					}
					else
					{
						if (PoliceTarget != -1)
						{
							WCHAR ResultMessage[1024];
							wstring ResultPlayerName = L"Client" + to_wstring(PoliceTarget) + L" (이)는 마피아가 아닙니다.";
							wcscpy_s(ResultMessage, sizeof(ResultMessage) / sizeof(WCHAR), ResultPlayerName.c_str());
							PushSystemMessages(ResultMessage);
						}
					}
				}
				// 마피아 처리
				if (KilledPlayerIndex != -1)
				{
					WCHAR ResultMessage[1024];
					wstring ResultPlayerName = L"마피아가 Client" + to_wstring(KilledPlayerIndex) + L" (이)를 살해했습니다.";
					wcscpy_s(ResultMessage, sizeof(ResultMessage) / sizeof(WCHAR), ResultPlayerName.c_str());
					PushSystemMessages(ResultMessage);
					CSoundMgr::Get_Instance()->PlaySoundEffect(SE_SHOT);
				}
			}
			if (InGameState == INGAME_STATE_VOTE)
			{
				WCHAR SystemMessage[1024];
				recv(mySocket, (char*)&SystemMessage, sizeof(SystemMessage), 0);
				PushSystemMessages(SystemMessage);

			}
			
			// 투표 처형 받아주기
			if (InGameState == INGAME_STATE_NIGHT)
			{
				recv(mySocket, (char*)&VoteResultPannelIndex, sizeof(VoteResultPannelIndex), 0);
				if (VoteResultPannelIndex != -1)
				{
					ObjectManager::Get_Instance()->Set_Pannel_DeadPlayer(VoteResultPannelIndex);
					WCHAR DeadMessage[1024] = L"가 투표로 처형되었습니다.";
					wstring DeadResultMessage = to_wstring(VoteResultPannelIndex) + DeadMessage;
					wcscpy_s(DeadMessage, sizeof(DeadMessage) / sizeof(WCHAR), DeadResultMessage.c_str());
					PushSystemMessages(DeadMessage);
					CSoundMgr::Get_Instance()->PlaySoundEffect(SE_SCREAM);
				}
				VoteResultPannelIndex = -1;

				WCHAR SystemMessage[1024];
				recv(mySocket, (char*)&SystemMessage, sizeof(SystemMessage), 0);
				PushSystemMessages(SystemMessage);
			}
			VotedPannel = -1;
			TargetedPannel = -1;
		}

		// 직업에 따라서 다르게 받기
		// 밤에 능력 받아주는 거
		if (Player->Get_PlayerJob() == PLAYER_JOB_CITIZEN)
		{

		}
		else if (Player->Get_PlayerJob() == PLAYER_JOB_MAFIA)
		{
			recv(mySocket, (char*)&MafiaTargetIndex, sizeof(MafiaTargetIndex), 0);
			TargetedPannel = MafiaTargetIndex;
		}
		else if (Player->Get_PlayerJob() == PLAYER_JOB_POLICE)
		{
			recv(mySocket, (char*)&PoliceTarget, sizeof(PoliceTarget), 0);
			TargetedPannel = PoliceTarget;
		}

		// 투표 관련
		bool bVoted = false;
		int VotedIndex = -1;
		recv(mySocket, (char*)&bVoted, sizeof(bVoted), 0);
		if (bVoted)
		{
			recv(mySocket, (char*)&VotedIndex, sizeof(VotedIndex), 0);
			VotedPannel = VotedIndex;
		}
	}
}

void InGameScene::SendChat(TChatMessage newChatMessage)
{
	SOCKET MySocket = Player->Get_ClientSocket().Socket;

	// 패킷 타입의 send인지 알려주기
	EPacketType PacketType = CHAT_PACKET; // CHAT_PACKET
	send(MySocket, (char*)&PacketType, sizeof(PacketType), 0);

	Sleep(10);

	send(MySocket, (char*)(newChatMessage.MessageOwner), sizeof(newChatMessage.MessageOwner), 0);
	send(MySocket, (char*)(newChatMessage.Message), sizeof(newChatMessage.Message), 0);
}

void InGameScene::SendVote(int VictimIndex)
{
	SOCKET MySocket = Player->Get_ClientSocket().Socket;

	// 패킷 타입의 send인지 알려주기
	EPacketType PacketType = VOTE_PACKET; // VOTE_PACKET
	send(MySocket, (char*)&PacketType, sizeof(PacketType), 0);

	Sleep(10);
	
	int Index = VictimIndex;
	send(MySocket, (char*)&Index, sizeof(Index), 0);
}

void InGameScene::SendTarget(int TargetIndex)
{
	SOCKET MySocket = Player->Get_ClientSocket().Socket;

	// 패킷 타입의 send인지 알려주기
	EPacketType PacketType = TARGET_PACKET; // TARGET_PACKET
	send(MySocket, (char*)&PacketType, sizeof(PacketType), 0);

	Sleep(10);

	int Index = TargetIndex;
	send(MySocket, (char*)&Index, sizeof(Index), 0);
}


void InGameScene::UpdateInGameDay()
{
	// 엔터 누르면 메시지 입력
	if (KeyManager::Get_Instance()->Key_Down(VK_RETURN))
	{
		GetWindowText(g_hEdit, ChatBuffer, 1024);

		if (wcscmp(ChatBuffer, L"") == 0) return;

		TChatMessage newChat = {};
		wcscpy_s(newChat.MessageOwner, 1024, Player->Get_Player_Name());
		wcscpy_s(newChat.Message, 1024, ChatBuffer);

		//SendChat(newChat);
		SOCKET MySocket = Player->Get_ClientSocket().Socket;

		// 패킷 타입의 send인지 알려주기
		EPacketType PacketType = CHAT_PACKET; // CHAT_PACKET
		send(MySocket, (char*)&PacketType, sizeof(PacketType), 0);

		Sleep(10);

		send(MySocket, (char*)(newChat.MessageOwner), sizeof(newChat.MessageOwner), 0);
		send(MySocket, (char*)(newChat.Message), sizeof(newChat.Message), 0);


		SetWindowText(g_hEdit, TEXT(""));
		SetFocus(g_hEdit);
	}

	ObjectManager::Get_Instance()->Update();
}

void InGameScene::UpdateInGameNight()
{
	if (KeyManager::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		auto PannelList = ObjectManager::Get_Instance()->GetObjectList(PANNEL);

		for (auto pannel : *PannelList)
		{
			dynamic_cast<CPannel*>(pannel)->Reset_Pannel_PickState();
		}

		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		for (auto pannel : *PannelList)
		{
			if (PtInRect((pannel->Get_Rect()), pt))
			{
				CPannel* curPannel = dynamic_cast<CPannel*>(pannel);
				if (curPannel->Get_bVisible() && !curPannel->Get_PannelDead())
				{
					int Index = curPannel->Get_PlayerIndex();
					SendTarget(Index);
				}
			}
		}
	}

	ObjectManager::Get_Instance()->Update();
}

void InGameScene::UpdateInGameVote()
{
	// 엔터 누르면 메시지 입력
	if (KeyManager::Get_Instance()->Key_Down(VK_RETURN))
	{
		GetWindowTextW(g_hEdit, ChatBuffer, 1024);

		if (wcscmp(ChatBuffer, L"") == 0) return;

		TChatMessage newChat = {};
		wcscpy_s(newChat.MessageOwner, sizeof(newChat.MessageOwner) / sizeof(WCHAR), Player->Get_Player_Name());
		wcscpy_s(newChat.Message, sizeof(newChat.Message) / sizeof(WCHAR), ChatBuffer);

		//SendChat(newChat);
		SOCKET MySocket = Player->Get_ClientSocket().Socket;

		// 패킷 타입의 send인지 알려주기
		EPacketType PacketType = CHAT_PACKET; // CHAT_PACKET
		send(MySocket, (char*)&PacketType, sizeof(PacketType), 0);

		Sleep(10);

		send(MySocket, (char*)(newChat.MessageOwner), sizeof(newChat.MessageOwner), 0);
		send(MySocket, (char*)(newChat.Message), sizeof(newChat.Message), 0);

		SetWindowText(g_hEdit, TEXT(""));
		SetFocus(g_hEdit);
	}
	
	// 투표
	if (InGameState == INGAME_STATE_VOTE)
	{
		if (KeyManager::Get_Instance()->Key_Down(VK_RBUTTON))
		{
			POINT pt{};
			GetCursorPos(&pt);
			ScreenToClient(g_hWnd, &pt);

			auto PannelList = ObjectManager::Get_Instance()->GetObjectList(PANNEL);

			for (auto pannel : *PannelList)
			{
				if (PtInRect((pannel->Get_Rect()), pt))
				{
					// 패널 인덱스 가져오기
					CPannel* curPannel = dynamic_cast<CPannel*>(pannel);
					if (curPannel->Get_bVisible() && !curPannel->Get_PannelDead())
					{
						int Index = curPannel->Get_PlayerIndex();
						SendVote(Index);
					}
				}
			}
		}
	}
	
	ObjectManager::Get_Instance()->Update();
}

void InGameScene::UpdateInGameStateChanged()
{
	auto PannelList = ObjectManager::Get_Instance()->GetObjectList(PANNEL);

	for (auto pannel : *PannelList)
	{
		dynamic_cast<CPannel*>(pannel)->Reset_Pannel_PickState();
	}

	CSoundMgr::Get_Instance()->StopCurBgm();

	if (InGameState == INGAME_STATE_DAY)
	{
		CSoundMgr::Get_Instance()->PlayBgm(BGM_DAY);
	}
	else if (InGameState == INGAME_STATE_VOTE)
	{
		CSoundMgr::Get_Instance()->PlayBgm(BGM_VOTE);
	}
	else if (InGameState == INGAME_STATE_NIGHT)
	{
		CSoundMgr::Get_Instance()->PlayBgm(BGM_NIGHT);
	}
}

void InGameScene::RenderInGameDay(HDC hDC)
{
	HDC     hBackDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_ETC);
	HDC     hBackGroundDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_INGAME_BACKGROUND_DAY);

	BitBlt(hBackDC, 0, 0, WINCX, WINCY, hBackGroundDC, 0, 0, SRCCOPY);

	m_pPlayer->Render(hBackDC);

	lock_guard<mutex> lock(MutexMessage);

	for (auto message : Messages)
	{
		if (message.bVisible)
		{
			// 입력한 메세지 보여주기
			SetTextColor(hBackDC, RGB(255, 255, 255));    // 글자 색을 흰색으로
			SetBkMode(hBackDC, TRANSPARENT);              // 배경 투명
			RECT nameRect = { message.ChatRect.left, message.ChatRect.top - 20, message.ChatRect.right, message.ChatRect.bottom - 20 };
			DrawText(hBackDC, (message.MessageOwner), -1, &(nameRect), DT_VCENTER | DT_WORDBREAK);
			DrawText(hBackDC, (message.Message), -1, &(message.ChatRect), DT_VCENTER | DT_WORDBREAK);
		}
	}

	ObjectManager::Get_Instance()->Render(hBackDC);

	BitBlt(hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

	if (g_hEdit)
	{
		// 렌더링 영역만 무효화하고 WM_PAINT를 유발
		InvalidateRect(g_hEdit, NULL, FALSE);
		// WM_PAINT를 즉시 처리하도록 강제
		UpdateWindow(g_hEdit);
	}
}

void InGameScene::RenderInGameNight(HDC hDC)
{
	HDC     hBackDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_ETC);
	HDC     hBackGroundDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_INGAME_BACKGROUND_NIGHT);

	BitBlt(hBackDC, 0, 0, WINCX, WINCY, hBackGroundDC, 0, 0, SRCCOPY);

	m_pPlayer->Render(hBackDC);
	
	lock_guard<mutex> lock(MutexMessage);

	for (auto message : Messages)
	{
		if (message.bVisible)
		{
			// 입력한 메세지 보여주기
			SetTextColor(hBackDC, RGB(255, 255, 255));    // 글자 색을 흰색으로
			SetBkMode(hBackDC, TRANSPARENT);              // 배경 투명
			RECT nameRect = { message.ChatRect.left, message.ChatRect.top - 20, message.ChatRect.right, message.ChatRect.bottom - 20 };
			DrawText(hBackDC, (message.MessageOwner), -1, &(nameRect), DT_VCENTER | DT_WORDBREAK);
			DrawText(hBackDC, (message.Message), -1, &(message.ChatRect), DT_VCENTER | DT_WORDBREAK);
		}
	}

	ObjectManager::Get_Instance()->Render(hBackDC);

	BitBlt(hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

	if (g_hEdit)
	{
		// 렌더링 영역만 무효화하고 WM_PAINT를 유발
		InvalidateRect(g_hEdit, NULL, FALSE);
		// WM_PAINT를 즉시 처리하도록 강제
		UpdateWindow(g_hEdit);
	}
}

void InGameScene::RenderInGameVote(HDC hDC)
{
	HDC     hBackDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_ETC);
	HDC     hBackGroundDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_INGAME_BACKGROUND_DAY);

	BitBlt(hBackDC, 0, 0, WINCX, WINCY, hBackGroundDC, 0, 0, SRCCOPY);

	m_pPlayer->Render(hBackDC);

	lock_guard<mutex> lock(MutexMessage);

	for (auto message : Messages)
	{
		if (message.bVisible)
		{
			// 입력한 메세지 보여주기
			SetTextColor(hBackDC, RGB(255, 255, 255));    // 글자 색을 흰색으로
			SetBkMode(hBackDC, TRANSPARENT);              // 배경 투명
			RECT nameRect = { message.ChatRect.left, message.ChatRect.top - 20, message.ChatRect.right, message.ChatRect.bottom - 20 };
			DrawText(hBackDC, (message.MessageOwner), -1, &(nameRect), DT_VCENTER | DT_WORDBREAK);
			DrawText(hBackDC, (message.Message), -1, &(message.ChatRect), DT_VCENTER | DT_WORDBREAK);
		}
	}

	ObjectManager::Get_Instance()->Render(hBackDC);

	BitBlt(hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

	if (g_hEdit)
	{
		// 렌더링 영역만 무효화하고 WM_PAINT를 유발
		InvalidateRect(g_hEdit, NULL, FALSE);
		// WM_PAINT를 즉시 처리하도록 강제
		UpdateWindow(g_hEdit);
	}
}
