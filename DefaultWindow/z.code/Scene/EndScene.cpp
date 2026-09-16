#include "../etc/pch.h"
#include "EndScene.h"
#include "../Manager/AssetManager.h"
#include "../Manager/CSoundMgr.h"

EndScene::EndScene()
{
}

EndScene::~EndScene()
{
}

void EndScene::Initialize()
{
	MafiaWinHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_MAFIA_WIN);
	CitizenWinHDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_CITIZEN_WIN);

	// 메시지를 띄울 상자 만들기
	EndMessageRect.top = WINCY/2 - 100;
	EndMessageRect.bottom = WINCY/2 + 100;
	EndMessageRect.left = WINCX/2 - 200;
	EndMessageRect.right = WINCX/2 + 200;

	if (bMafiaWin)
	{
		//wcscpy_s(EndMessage, sizeof(EndMessage) / sizeof(WCHAR), L"마피아팀 승리");
		ResultHDC = MafiaWinHDC;
	}
	else
	{
		//wcscpy_s(EndMessage, sizeof(EndMessage) / sizeof(WCHAR), L"시민팀 승리");
		ResultHDC = CitizenWinHDC;
	}

	CSoundMgr::Get_Instance()->StopCurBgm();
}

void EndScene::Update()
{
}

void EndScene::Render(HDC hDC)
{
	HDC     hBackDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_ETC);
	HDC     hBackGroundDC = AssetManager::Get_Instance()->Find_AssetHDC(ASSET_LOBBY_BACKGROUND);

	BitBlt(hBackDC, 0, 0, WINCX, WINCY, hBackGroundDC, 0, 0, SRCCOPY);

	//// 닉네임 렌더링
	//SetTextColor(hBackGroundDC, RGB(255, 255, 255));    // 글자 색을 흰색으로
	//SetBkMode(hBackGroundDC, TRANSPARENT);              // 배경 투명
	//DrawTextW(hBackGroundDC, EndMessage, -1, &EndMessageRect, DT_VCENTER | DT_WORDBREAK);

	GdiTransparentBlt(hBackDC,
		WINCX/2 - 250,
		WINCY/2 - 250,
		500,
		500,
		ResultHDC,
		0,
		0,
		500,   // 복사할 이미지 가로
		500,   // 복사할 이미지 세로
		RGB(255, 255, 255));    // 제거할 픽셀 색상 값

	BitBlt(hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
}

void EndScene::Release()
{
}

void EndScene::RefreshUI()
{
}
