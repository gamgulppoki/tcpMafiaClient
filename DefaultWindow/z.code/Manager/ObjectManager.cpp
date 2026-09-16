#include "../etc/pch.h"
#include "ObjectManager.h"
#include "../Object/CObj.h"
#include "../Object/CPannel.h"
#include "SceneManager.h"
#include "../etc/Define.h"

ObjectManager* ObjectManager::Instance = nullptr;

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	Release();
}

void ObjectManager::Add_Object(EObjectID eID, CObj* obj)
{
	if ((int)eID >= (int)EObjectID::OBJ_END || obj == nullptr) return;

	ObjectList[eID].push_back(obj);
}

void ObjectManager::Initialize()
{
	for (int i = 0; i < OBJ_END; i++)
	{
		for (auto obj : ObjectList[i])
		{
			if (obj == nullptr) return;
			obj->Initialize();
		}
	}
}

void ObjectManager::Update()
{
	for (int i = 0; i < OBJ_END; i++)
	{
		for (auto iter = ObjectList[i].begin(); iter != ObjectList[i].end();)
		{
			if ((*iter)->Get_Dead())
			{
				delete *iter;
				*iter = nullptr;

				iter = ObjectList[i].erase(iter);
			}
			else
			{
				(*iter)->Update();
				++iter;
			}
		}
	}
}

void ObjectManager::Render(HDC hDC)
{
	for (int i = 0; i < OBJ_END; i++)
	{
		for (auto obj : ObjectList[i])
		{
			if (obj == nullptr) return;
			if (SceneManager::Get_Instance()->GetSceneChanged()) return;
			obj->Render(hDC);
		}
	}
}

void ObjectManager::Release()
{
	// 여기서 오브젝트 전부 삭제해줘야 한다.
	for (int i = 0; i < OBJ_END; i++)
	{
		for (auto tmp : ObjectList[i])
		{
			delete tmp;
			tmp = nullptr;
		}
		ObjectList[i].clear();
	}
}

void ObjectManager::Delete_All_Except_Player()
{
	for (int i = 0; i < OBJ_END; i++)
	{
		if (i == PLAYER || i == MOUSE) continue;
		for (auto tmp : ObjectList[i])
		{
			delete tmp;
			tmp = nullptr;
		}
		ObjectList[i].clear();
	}
}

void ObjectManager::Delete_ID(OBJECTID eID)
{
}

void ObjectManager::Set_Pannel_DeadPlayer(int index)
{
	auto PannelList = ObjectList[PANNEL];
	dynamic_cast<CPannel*>(PannelList[index])->Set_PannelDead(true);
}

vector<CObj*>* ObjectManager::GetObjectList(EObjectID eID)
{
	return &(ObjectList[(int)eID]);
}
