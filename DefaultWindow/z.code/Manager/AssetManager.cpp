#include "../etc/pch.h"
#include "AssetManager.h"
#include "../Asset/CAsset.h"

AssetManager* AssetManager::Instance = nullptr;

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
	Release();
}


void AssetManager::LoadAssets()
{
	for (auto direction : AssetDirection)
	{
		CAsset* newAsset = new CAsset;

		newAsset->Load_Bmp(direction.second);

		if (newAsset == nullptr)
			continue;

		LoadedAssets.insert({ direction.first, newAsset });
	}
}

HDC AssetManager::Find_AssetHDC(EAssetID assetID)
{
	auto it = LoadedAssets.find(assetID);
	
	if (it != LoadedAssets.end())
	{
		if (it->second != nullptr)
		{
			return it->second->Get_MemDC();
		}
	}

	return HDC();
}


void AssetManager::Release()
{
	AssetDirection.clear();

	for (auto asset : LoadedAssets)
	{
		auto* tmp = asset.second;
		delete tmp;
		tmp = nullptr;
	}

	LoadedAssets.clear();
}