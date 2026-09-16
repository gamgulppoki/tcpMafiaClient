#pragma once
#include "CScene.h"

class CObj;

class TitleScene final :
    public CScene
{
public:
    TitleScene();
    ~TitleScene();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    void RefreshUI() override;

private:

public:
    //const std::wstring TitleSound = L"C:\\Users\\sarah\\Desktop\\API½Ã¿¬È¸\\Mafia_Client_Last\\x64\\Sound\\Bgm\\TitleTheme.wav";
    //const std::wstring TitleSound = L"../Sound/Bgm/TitleTheme.wav";

};

