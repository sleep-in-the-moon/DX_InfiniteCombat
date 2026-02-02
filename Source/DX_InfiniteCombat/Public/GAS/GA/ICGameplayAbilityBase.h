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

struct FInstancedStruct;
/**
 * 
 */
UCLASS()
class DX_INFINITECOMBAT_API UICGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
	friend class UICAbilitySystemComponent;

public:

	UICGameplayAbilityBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	EAbilityInputTriggerMode GetAbilityInputTriggerMode() const
	{
		return InputTriggerMode;
	}

protected:
	virtual void ReceiveArg(const FInstancedStruct& Arg);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="IC")
	EAbilityInputTriggerMode InputTriggerMode = EAbilityInputTriggerMode::OnClickOnce;

//protected:
//	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo
//		, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
//	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo
//		, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

};
