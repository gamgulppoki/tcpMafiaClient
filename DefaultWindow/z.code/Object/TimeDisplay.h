#pragma once
#include "CObj.h"

class TimeDisplay final
	:public CObj
{
public:
	TimeDisplay();
	~TimeDisplay();

	void Initialize() override;
	void Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	void SetPos(float x, float y);
	void SetRemainTime(long long time) { RemainTime = time; }

public:
	long long RemainTime;
};

