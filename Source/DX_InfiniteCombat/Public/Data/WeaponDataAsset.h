// Copyright DX_IC

#pragma once

#include "Engine/DataAsset.h"
#include "Data/ICTypes.h"

#include "WeaponDataAsset.generated.h"


/**
 * Weapon config data
 */
UCLASS(BlueprintType, Const)
class DX_INFINITECOMBAT_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	static const UWeaponDataAsset* GetInsByTag(const FGameplayTag& WeaponTag);

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WeaponTag; // Œ®“ª±Í ∂

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EquipSocket = "R_Hand_Socket";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName UnequipSocket = "Belt_Socket";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> AM_EquipWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> AM_UnequipWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UAnimInstance> LinkAnimClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Image;
};
