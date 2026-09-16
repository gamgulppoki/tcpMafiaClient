#include "../etc/pch.h"
#include "KeyManager.h"

KeyManager* KeyManager::m_pInstance = nullptr;

KeyManager::KeyManager()
{
    ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

KeyManager::~KeyManager()
{
}

bool KeyManager::Key_Pressing(int iKey)
{
	if (GetAsyncKeyState(iKey) & 0x8000)
		return true;

	return false;
}

bool KeyManager::Key_Up(int iKey)
{
	if ((m_bKeyState[iKey]) && !(GetAsyncKeyState(iKey) & 0x8000))
	{
		m_bKeyState[iKey] = !m_bKeyState[iKey];
		return true;
	}

	return false;
}

bool KeyManager::Key_Down(int iKey)
{
	// 이전에 누른적이 없고 && 지금 막 눌렀을 때
	if ((!m_bKeyState[iKey]) && (GetAsyncKeyState(iKey) & 0x8000))
	{
		m_bKeyState[iKey] = !m_bKeyState[iKey];
		return true;
	}

	return false;
}

void KeyManager::Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
		{
			m_bKeyState[i] = !m_bKeyState[i];
		}

		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
		{
			m_bKeyState[i] = !m_bKeyState[i];
		}
	}
}
