// Copyright DX_IC


#include "Data/WeaponDataAsset.h"
#include "Data/ICAssetManager.h"


const UWeaponDataAsset* UWeaponDataAsset::GetInsByTag(const FGameplayTag& WeaponTag)
{
	return UICAssetManager::Get().GetWeaponDataAssetByTag(WeaponTag);
}

FPrimaryAssetId UWeaponDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(GetClass()->GetFName(), WeaponTag.GetTagName());
}
