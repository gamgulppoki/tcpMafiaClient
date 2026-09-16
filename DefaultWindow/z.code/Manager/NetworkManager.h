#pragma once
#include "../etc/pch.h"
#include "../etc/Define.h"
#include "../Network/NetworkDefine.h"

class CPlayer;

/*
	PacketManager
	
	패킷 조립할 때 사용하기 위한 클래스
	혹여나 빠뜨리는 변수가 있지 않을까 해서 ...
*/

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void SetPlayer(CPlayer* player) { Player = player; }

public:
	static NetworkManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new NetworkManager;
		}
		return Instance;
	}

	void Destroy_Instance()
	{
		if (Instance)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

private:
	static NetworkManager* Instance;
	CPlayer* Player;
};

