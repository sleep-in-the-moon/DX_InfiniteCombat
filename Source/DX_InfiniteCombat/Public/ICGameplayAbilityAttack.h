// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "ICGameplayAbilityBase.h"
#include "ICGameplayAbilityAttack.generated.h"

USTRUCT(BlueprintType)
struct FComboInfo
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextSection;
};

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICGameplayAbilityAttack : public UICGameplayAbilityBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FComboInfo> MontageComboMap;

	UPROPERTY(BlueprintReadWrite)
	FName CurrentComboSection = "FirstCombo";

};
