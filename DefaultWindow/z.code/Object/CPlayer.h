#pragma once

#include "../etc/pch.h"
#include "../etc/Define.h"
#include "CObj.h"

#include "../Network/ClientPacket.h"
#include "../Network/ServerPacket.h"
#include "../Network/ClientSocket.h"

class CPlayer : public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();
	
public:
	void Initialize() override;
	void Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	WCHAR* Get_Player_Name() { return PlayerName; }
	EPlayerJob Get_PlayerJob() { return PlayerJob; }
	bool Get_PlayerDead() { return bPlayerDead; }
	ClientSocket& Get_ClientSocket() { return clientSocket; }
	bool Get_GameStarted() { return bGameStarted; }
	vector<PlayerInfo>& Get_AllClientInfo() { return AllClientInfo_Vec; }
	int Get_PlayerIndex() { return PlayerIndex; }

	EPlayerJob* Get_PlayerJobPointer() { return &PlayerJob; }

	void Set_PlayerName(WCHAR* name) { wcscpy_s(PlayerName, sizeof(PlayerName)/sizeof(WCHAR), name); }
	void Set_PlayerIndex(int index) { PlayerIndex = index; }
	void Set_PlayerJob(EPlayerJob job) { PlayerJob = job; }
	void Set_PlayerDead(bool boolean) { bPlayerDead = boolean; }
	void Set_GameStarted(bool boolean) { bGameStarted = boolean; }

private:
	ClientSocket clientSocket;

public:
	WCHAR PlayerName[1024] = L"Client0";
	int PlayerIndex;
	EPlayerJob PlayerJob;
	bool bPlayerDead;

	bool bGameStarted = false;
	vector<PlayerInfo> AllClientInfo_Vec;
};

