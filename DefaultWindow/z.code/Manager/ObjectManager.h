#pragma once

#include "../etc/Define.h"

class CObj;

class ObjectManager
{
public:
	ObjectManager();
	ObjectManager(const ObjectManager& rhs) = delete;
	ObjectManager operator=(ObjectManager& rObj) = delete;
	~ObjectManager();

public:
	void Add_Object(EObjectID eID, CObj* obj);
	void Initialize();
	void Update();
	void Render(HDC hDC);
	void Release();

	void Delete_All_Except_Player();
	void Delete_ID(OBJECTID eID);

	void SetPlayer(CObj* player) { Player = player; }
	CObj* GetPlayer() { return Player; }
	void Set_Pannel_DeadPlayer(int index);

	vector<CObj*>* GetObjectList(EObjectID eID);

public:
	static ObjectManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new ObjectManager;
		}
		return Instance;
	}

	void Destroy_Instance()
	{
		if (Instance)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

private:
	static ObjectManager* Instance;

	vector<CObj*> ObjectList[OBJ_END];
	CObj* Player;
};

