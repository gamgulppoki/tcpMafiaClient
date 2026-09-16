#include "../etc/pch.h"
#include "TimeDisplay.h"

TimeDisplay::TimeDisplay()
{
	__super::Update_Rect();
}

TimeDisplay::~TimeDisplay()
{
}

void TimeDisplay::Initialize()
{
	RemainTime = 0;
	m_tInfo.fCX = 50;
	m_tInfo.fCY = 30;
	__super::Update_Rect();
}

void TimeDisplay::Update()
{
}

void TimeDisplay::Render(HDC hDC)
{
	// 입력한 메세지 보여주기
	SetTextColor(hDC, RGB(255, 255, 255));    // 글자 색을 흰색으로
	SetBkMode(hDC, TRANSPARENT);              // 배경 투명
	wstring timeChar = to_wstring((int)RemainTime);
	DrawText(hDC, timeChar.c_str(), -1, &(m_tRect), DT_VCENTER | DT_WORDBREAK);
}

void TimeDisplay::Release()
{
}

void TimeDisplay::SetPos(float x, float y)
{
	m_tInfo.fX = x;
	m_tInfo.fY = y;
}
