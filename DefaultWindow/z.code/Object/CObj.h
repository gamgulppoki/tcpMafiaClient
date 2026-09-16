#pragma once

#include "../etc/Define.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void	Initialize()	PURE;
	virtual void	Update()		PURE;
	virtual void	Render(HDC hDC)	PURE;
	virtual void	Release()		PURE;

	bool		Get_Dead() { return m_bDead; }

	RECT*		Get_Rect() { return &m_tRect; }
	void		SetInfoXY(float _x, float _y) { m_tInfo.fX = _x; m_tInfo.fY = _y; }

protected:
	virtual void		Update_Rect();

protected:
	INFO		m_tInfo;
	RECT		m_tRect;

	float		m_fSpeed;
	bool		m_bDead;
};

