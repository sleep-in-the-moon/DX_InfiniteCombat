// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/ICGameplayAbilityBase.h"
#include "GA_Death.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UGA_Death : public UICGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
