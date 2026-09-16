#pragma once
#include "CObj.h"

class CAsset;

/*
    사실상 Player Controller 이라고 보면 된다
*/

class CMouse final :
    public CObj
{
public:
    CMouse();
    ~CMouse();

    // CObj을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

    void KeyInput();
};

