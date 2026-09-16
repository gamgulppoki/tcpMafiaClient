#pragma once

#include "CScene.h"

class CObj;

class LobbyScene final :
    public CScene
{
public:
    LobbyScene();
    ~LobbyScene();

    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void InitNetwork();
    void LobbyNetwork();
    void ReceivePacket();
    void UpdatePacket();
    void SendPacket();

    void PressReady();

    void RefreshUI() override;

public:
    int TotalClientNum;

    bool LobbyReady[10];
    PlayerInfo AllPlayerInfo[10];

    thread LobbyNetworkThread;
};

