// Copyright DX_IC

#pragma once

#include "Engine/DataAsset.h"
#include "Data/ICTypes.h"

#include "WeaponDataAsset.generated.h"


/**
 * Weapon config list
 */
UCLASS(BlueprintType, Const)
class DX_INFINITECOMBAT_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	static const UWeaponDataAsset& Get();

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FWeaponStruct> WeaponList;

};
