#include "../etc/pch.h"
#include "NetworkManager.h"
#include "../Object/CPlayer.h"
#include "ObjectManager.h"

NetworkManager* NetworkManager::Instance = nullptr;

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

