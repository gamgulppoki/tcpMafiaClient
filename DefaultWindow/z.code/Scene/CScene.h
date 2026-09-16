#pragma once

#include "../etc/pch.h"
#include "../etc/Define.h"

class CObj;

class CScene
{
public:
	CScene() {}
	virtual ~CScene() {}

	virtual void Initialize()		PURE;
	virtual void Update()			PURE;
	virtual void Render(HDC hDC)	PURE;
	virtual void Release()			PURE;
	
	virtual void RefreshUI()			PURE;


	void SetPlayer(CObj* player) { m_pPlayer = player; }

protected:
	CObj* m_pPlayer;

};

