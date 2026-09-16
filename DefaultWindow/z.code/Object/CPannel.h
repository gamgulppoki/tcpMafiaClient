#pragma once
#include "CObj.h"
#include "CPlayer.h"

class CPannel :
    public CObj
{
public:
    CPannel();
    ~CPannel();

    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void Render_Pannel_State(HDC hDC);
    void Render_Pannel_PickState(HDC hDC);

    void OnClicked();
    void OnVoted();
    void OnTargeted();
    void OnInvestigated();
    void Reset_Pannel_PickState() { PannelPickState = PANNEL_PICK_NONE; }

    void Update_Rect();
    void Set_Pos(float x, float y);
    void Set_Player(CPlayer* player) { m_pPlayer = player; }
    void Set_Visible(bool bvis) { bVisible = bvis; }
    void Set_MyPannel(bool bMy) { bMyPannel = bMy; }
    void Set_MyPannelState(EPlayerJob playerJob);
    void Set_PannelPickState(EPannelPickState state) { PannelPickState = state; }
    void Set_PannelDead(bool boolean) { bDead = boolean; }

    int Get_PlayerIndex() { return m_pPlayer->Get_PlayerIndex(); }
    bool Get_bVisible() { return bVisible; }
    bool Get_PannelDead() { return bDead; }

private:
    bool bVisible;
    bool bDead;
    EPannelState PannelState;
    EPannelPickState PannelPickState;
    HDC PannelHDC;

    RECT IconRect;
    float IconSize;
    float IconTop;
    float IconLeft;

    HDC NoneIconHDC;
    HDC CitizenIconHDC;
    HDC MafiaIconHDC;
    HDC PoliceIconHDC;

    HDC CrossHairHDC;
    HDC CrossMarkHDC;
    HDC InvestMarkHDC;
    HDC DeadMarkHDC;

    RECT NameRect;
    CPlayer* m_pPlayer;
    bool bMyPannel;
};

