#include "../etc/pch.h"
#include "CMouse.h"
#include "../Manager/KeyManager.h"
#include "../Manager/ObjectManager.h"
#include "CButton.h"
#include "CPannel.h"

CMouse::CMouse()
{
}

CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	m_tInfo.fCX = 20.f;
	m_tInfo.fCY = 20.f;
}

void CMouse::Update()
{
	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	
	m_tInfo.fX = (float)ptMouse.x;
	m_tInfo.fY = (float)ptMouse.y;

	__super::Update_Rect();
	
	ShowCursor(true);

	// 키 입력 관련 작업
	KeyInput();
}

void CMouse::Render(HDC hDC)
{
}

void CMouse::Release()
{
}

void CMouse::KeyInput()
{
	if (KeyManager::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		auto ButtonList = ObjectManager::Get_Instance()->GetObjectList(BUTTON);
		auto PannelList = ObjectManager::Get_Instance()->GetObjectList(PANNEL);

		for (auto button : *ButtonList)
		{
			if (PtInRect((button->Get_Rect()), pt))
			{
				dynamic_cast<CButton*>(button)->OnClicked();
			}
		}

		for (auto pannel : *PannelList)
		{
			if (PtInRect((pannel->Get_Rect()), pt))
			{
				dynamic_cast<CPannel*>(pannel)->OnClicked();
			}
		}
	}
}
