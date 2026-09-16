#include "../etc/pch.h"
#include "TitleScene.h"
#include "../Manager/AssetManager.h"
#include "../Object/CObj.h"
#include "../Manager/ObjectManager.h"
#include "../Object/CButton.h"
#include "../etc/Define.h"
#include "../Manager/CSoundMgr.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	CObj* StartButton = new CButton(100, 80);
	dynamic_cast<CButton*>(StartButton)->SetButtonType(BUTTON_PLAY);
	StartButton->SetInfoXY(WINCX/2, WINCY-100);
	ObjectManager::Get_Instance()->Add_Object(BUTTON, StartButton);

	ObjectManager::Get_Instance()->Initialize();

	// CSoundMgr::Get_Instance()->Load(TitleSound, TitleSound);
	CSoundMgr::Get_Instance()->PlayBgm(BGM_TITLE);

}

void TitleScene::Update()
{
	ObjectManager::Get_Instance()->Update();
}

void TitleScene::Render(HDC hDC)
{
	HDC     hBackDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_ETC);
	HDC     hBackGroundDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_TITLE_BACKGROUND);

	BitBlt(hBackDC, 0, 0, WINCX, WINCY, hBackGroundDC, 0, 0, SRCCOPY);

	m_pPlayer->Render(hBackDC);

	BitBlt(hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);

	ObjectManager::Get_Instance()->Render(hDC);
}

void TitleScene::Release()
{
}

void TitleScene::RefreshUI()
{
}
