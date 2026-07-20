// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/ICGameplayAbilityBase.h"
#include "GA_Traversal.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UGA_Traversal : public UICGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr
		, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void TraversalEnd();

private:
	EMovementMode RawMovementMode;
	uint8 RawCustomMovementMode;;
};
