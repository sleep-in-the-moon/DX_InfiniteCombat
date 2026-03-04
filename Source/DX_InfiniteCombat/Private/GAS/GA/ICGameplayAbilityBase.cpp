// Copyright DX_IC


#include "gas/GA/ICGameplayAbilityBase.h"


//void UICGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
//{
//	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
//	{
//		constexpr bool bReplicateEndAbility = true;
//		constexpr bool bWasCancelled = true;
//		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//		return;
//	}
//  
//  Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
//}


//void UICGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
//{
//	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//}

UICGameplayAbilityBase::UICGameplayAbilityBase(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = false;
}

void UICGameplayAbilityBase::ReceiveArg(const FInstancedStruct& Arg)
{
	//(Arg.IsValid() && Arg.GetPtr<XX>() )
}

FInstancedStruct UICGameplayAbilityBase::ConsumArg() const
{
	UICAbilitySystemComponent* ASC = Cast<UICAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if(ASC)
		return ASC->ConsumAbilityArg();

	return FInstancedStruct();
}
