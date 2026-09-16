#pragma once

#include "../etc/Define.h"
#include "../Object/CPlayer.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void	Initialize();
	void	Update();
	void	Render();
	void	Release();

private:
	HDC		m_hDC;
	CObj* m_pPlayer;

};

