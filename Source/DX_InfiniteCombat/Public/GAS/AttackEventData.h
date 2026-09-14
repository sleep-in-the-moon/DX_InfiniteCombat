// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttackEventData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DX_INFINITECOMBAT_API UAttackEventData : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FVector HitLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
	FVector ImpactNormal = FVector::ZeroVector;
};
