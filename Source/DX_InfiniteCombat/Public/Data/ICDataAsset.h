// Copyright DX_IC

#pragma once

#include "Engine/DataAsset.h"
#include "ICDataAsset.generated.h"


class UGameplayEffect;
class UGameplayAbility;

/**
 * 
 */
UCLASS(BlueprintType, Const)
class DX_INFINITECOMBAT_API UICDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	static const UICDataAsset& Get();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UGameplayEffect> DamageGEClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UGameplayAbility> DeathGAClass;

};
