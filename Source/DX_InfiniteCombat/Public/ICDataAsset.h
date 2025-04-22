// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ICDataAsset.generated.h"


class UGameplayEffect;

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	static const UICDataAsset& Get();

public:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UGameplayEffect> DamageGEClass;

};
