// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "ICGameplayAbilityBase.h"
#include "GameplayAbilityGetHurt.generated.h"


class UAnimMontage;
/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UGameplayAbilityGetHurt : public UICGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* GetHitMontage;
	
};
