// Copyright DX_IC


#include "Data/WeaponDataAsset.h"
#include "Data/ICAssetManager.h"

const UWeaponDataAsset& UWeaponDataAsset::Get()
{
	return UICAssetManager::Get().GetWeaponDataAsset();
}
