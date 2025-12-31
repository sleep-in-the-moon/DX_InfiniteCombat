// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ICWaitInputReleased.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputReleasedDelegate, float, HeldTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFixTimeEvent);

/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UAbilityTask_ICWaitInputReleased : public UAbilityTask
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FInputReleasedDelegate OnReleased;
	UPROPERTY(BlueprintAssignable)
	FFixTimeEvent FixTimeEvent;

	UFUNCTION()
	void OnReleaseCallback(const FGameplayTag& Tag);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	UFUNCTION(BlueprintCallable, Category = "Ability|ICTasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_ICWaitInputReleased* ICWaitInputReleased(UGameplayAbility* OwningAbility, float FixTime=0.2f);

protected:
	float StartTime = 0.f;
	FGameplayTagContainer AbilityTags;
private:
	float FixTime;
	FTimerHandle Timer;
	void CallFixTime();

};
