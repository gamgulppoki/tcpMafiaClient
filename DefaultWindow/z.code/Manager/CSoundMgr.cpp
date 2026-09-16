#include "../etc/pch.h"
#include "CSoundMgr.h"
#include "../etc/Define.h"

CSoundMgr* CSoundMgr::m_pInstance = nullptr;

void CSoundMgr::LoadAllSound()
{
	for (int i = 0; i < BGM_END; i++)
	{
		Load(BgmDirections[i], BgmDirections[i]);
	}

	for (int i = 0; i < SE_END; i++)
	{
		Load(SoundEffectDirections[i], SoundEffectDirections[i]);
	}
}

CSoundMgr::CSoundMgr()
{
}

CSoundMgr::~CSoundMgr()
{
}
