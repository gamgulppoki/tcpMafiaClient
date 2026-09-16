#pragma once

#include "../etc/Define.h"

class CMouse;

class KeyManager
{
private:
	KeyManager();
	~KeyManager();

public:
	bool		Key_Pressing(int iKey);
	bool		Key_Up(int iKey);	// 눌렀다가 뗏을 때
	bool		Key_Down(int iKey); // 누르자 마자

public:
	void		Update();


public:
	static KeyManager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new KeyManager;
		}

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static KeyManager* m_pInstance;
	bool			m_bKeyState[VK_MAX];
};


