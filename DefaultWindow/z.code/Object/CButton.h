#pragma once
#include "CObj.h"
#include "../etc/Define.h"

class CAsset;

class CButton final :
    public CObj
{
public:
    CButton();
    CButton(float fcx, float fcy);
    virtual ~CButton();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

    void OnClicked();

    void SetButtonType(EButtonType _buttonType) { ButtonType = _buttonType; }
    void GetButtonAsset();
    void SetPos(float x, float y);

private:
    EButtonType ButtonType;
    EAssetID ButtonAssetID;

    HDC ButtonHDC;

    HDC SoundButtonOnHDC;
    HDC SoundButtonOffHDC;

    int m_iDrawID;
};

