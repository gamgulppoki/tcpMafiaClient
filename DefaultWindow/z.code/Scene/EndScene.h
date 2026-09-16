#pragma once

#include "CScene.h"

class EndScene final:
    public CScene
{
public:
    EndScene();
    ~EndScene();

    // CScene을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void RefreshUI() override;

private:
    WCHAR EndMessage[1024];

    RECT EndMessageRect;

    HDC MafiaWinHDC;
    HDC CitizenWinHDC;
    HDC ResultHDC;
};

