#pragma once
#include "CScene.h"
#include "../etc/pch.h"

class CPlayer;
class TimeDisplay;

class InGameScene final :
    public CScene
{
public:
    InGameScene();
    ~InGameScene();

    // CScene을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void RefreshUI() override;

    void UpdateInGameDay();
    void UpdateInGameNight();
    void UpdateInGameVote();
    void UpdateInGameStateChanged();

    void RenderInGameDay(HDC hDC);
    void RenderInGameNight(HDC hDC);
    void RenderInGameVote(HDC hDC);

    void PushMessages(TChatMessage newTChatMessage);
    void PushSystemMessages(WCHAR* message);
    EInGameState GetInGameState() { return InGameState; }

    void GetInitPacket();

    void ReceivePacket();

    void SendChat(TChatMessage newChatMessage);
    void SendVote(int VictimIndex);
    void SendTarget(int TargetIndex);


private:
    HWND hEdit;

    WCHAR ChatBuffer[1024];
    vector<TChatMessage> Messages;

    EInGameState PreInGameState;
    EInGameState InGameState;
    bool bPreGameEnd = false;

    bool bCanVote;
    bool bCanKill;
    bool bCanInvest;

    long long RemainTime;

    int VotedPannel = -1;
    int TargetedPannel = -1;
    int VoteResultPannelIndex = -1;

    CPlayer* Player;
    TimeDisplay* pTime;
    int PlayerNum;

    int KilledPlayerIndex = -1;
    int MafiaTargetIndex = -1;
    int PoliceTarget = -1;
    bool PoliceTargetIsMafia = false;

    mutex MutexMessage;
};

