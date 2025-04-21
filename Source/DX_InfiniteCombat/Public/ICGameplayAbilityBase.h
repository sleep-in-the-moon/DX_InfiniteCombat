// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ICGameplayAbilityBase.generated.h"

UENUM(BlueprintType)
enum class EAbilityInputTriggerMode : uint8
{
	OnClickOnce,
	OnHeldActive
};

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	EAbilityInputTriggerMode GetAbilityInputTriggerMode() const
	{
		return InputTriggerMode;
	}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="IC")
	EAbilityInputTriggerMode InputTriggerMode = EAbilityInputTriggerMode::OnClickOnce;

};
