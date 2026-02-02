// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/ICGameplayAbilityBase.h"
#include "GA_SwitchWeapon.generated.h"

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UGA_SwitchWeapon : public UICGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual void ReceiveArg(const FInstancedStruct& Arg) override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

private:
	UFUNCTION()
	void OnMontageCompleted();
	UFUNCTION()
	void OnMontageBlendOut();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageCancelled();
	/*UFUNCTION()
	void OnMontageBlendedIn();*/

private:
	FGameplayTag SwitchWeaponTag;

};
