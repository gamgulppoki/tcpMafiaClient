#pragma once
#include "CObj.h"

class CPlayer;

class CLeaderBoardEntry :
    public CObj
{
public:
    CLeaderBoardEntry();
    CLeaderBoardEntry(float fx, float fy);
    virtual ~CLeaderBoardEntry();

    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void Update_Rect() override;
    void Set_PlayerInfo(PlayerInfo& playerInfo) { m_PlayerInfo = playerInfo; }
    void Set_Check_Visible(bool visible) { bCheckVisible = visible; }
    void Set_Visible(bool visible) { bVisible = visible; }

    bool Get_IsReady() { return bCheckVisible; }

private:
    bool bVisible;
    bool bCheckVisible;
    RECT CheckRect;
    RECT NameRect;

    HDC EntryHDC;
    HDC CheckHDC;

    PlayerInfo m_PlayerInfo;
};

